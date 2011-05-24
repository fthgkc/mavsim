#!BPY
"""
Name: 'FreeCAD (.fcstd)...'
Blender: 248
Group: 'Import'
Tooltip: 'Imports a FreeCAD file'
"""
__author__ = ["Yorik van Havre"]
__url__ = ("blender", "blenderartists", "http://yorik.orgfree.com", "http://free-cad.sourceforge.net")
__version__ = "0.1.0"
__bpydoc__ = "This scripts imports the contents of a FreeCAD file in Blender."

# History
# 0.0.1 - first version
# 0.1.0 - added Mesh support

# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# ***** END GPL LICENCE BLOCK *****


# ***** CONFIG *****

FREECADPATH = '/usr/lib/freecad/lib' # path to your FreeCAD.la file
ARCSEGMENTS = 16 # number of segments to use for drawing arcs

# ***** END CONFIG *****

from Blender import Scene, Mesh, Object, Draw, Window, Mathutils, Text3d
import Blender, BPyMessages, sys, math
# from import_dxf import calcArc # using the arc segmentation code from Migius
sys.path.append(FREECADPATH)

def calcArc(center, radius, start, end, arc_res, triples):
	"""calculate Points (or BezierTriples) for ARC/CIRCLEs representation.
	
	Given parameters of the ARC/CIRCLE,
	returns points/segments (or BezierTriples) and centerPoint
	"""
	# center is currently set by object
	# if start > end: start = start - 360
	if end > 360: end = end % 360.0

	startmatrix = Mathutils.RotationMatrix(-start, 3, "Z")
	startpoint = startmatrix * Mathutils.Vector(radius, 0, 0)
	endmatrix = Mathutils.RotationMatrix(-end, 3, "Z")
	endpoint = endmatrix * Mathutils.Vector(radius, 0, 0)

	if end < start: end +=360.0
	angle = end - start
	#length = radians(angle) * radius

	if not triples: #IF mesh-representation -----------
		print 'mesh-representation\n'
		if arc_res > 1024: arc_res = 1024 
		elif arc_res < 4: arc_res = 4 
		pieces = int(abs(angle)/(360.0/arc_res)) # set a fixed step of ARC_RESOLUTION
		if pieces < 3: pieces = 3
		step = angle/pieces # set step so pieces * step = degrees in arc
		stepmatrix = Mathutils.RotationMatrix(-step, 3, "Z")

		points = [startpoint]
		point = startpoint
		for i in xrange(int(pieces)-1):
			point = stepmatrix * point
			points.append(point)
		points.append(endpoint)
	
		if center:
			centerVec = Mathutils.Vector(center)
			#points = [point + centerVec for point in points()]
			points = [point + centerVec for point in points]
		# vector to point convertion:
		points = [list(point) for point in points]
		return points

	else:  #IF curve-representation ---------------
		if arc_res > 32: arc_res = 32
		elif arc_res < 3: arc_res = 3 
		pieces = int(abs(angle)/(360.0/arc_res)) # set a fixed step of ARC_RESOLUTION
		if pieces < 2: pieces = 2
		step = angle/pieces # set step so pieces * step = degrees in arc
		stepmatrix = Mathutils.RotationMatrix(-step, 3, "Z")

		# correct Bezier curves representation for free segmented circles/arcs
		step2 = radians(step * 0.5)
		bulg = radius * (1 - cos(step2))
		deltaY = 4.0 * bulg / (3.0 * sin(step2) )
		print 'deb:calcArcCurve: bulg, deltaY:\n',  bulg, deltaY  #---------
		print 'deb:calcArcCurve: step:\n',  step  #---------
		handler0 = Mathutils.Vector(0.0, -deltaY, 0.0)
	
		points = [startpoint]
		handler = startmatrix * handler0
		endhandler = endmatrix * handler0
		handlers1 = [startpoint + handler]
		handlers2 = [startpoint - handler]
		point = Mathutils.Vector(startpoint)
		for i in xrange(int(pieces)-1):
			point = stepmatrix * point
			handler = stepmatrix * handler
			handler1 = point + handler
			handler2 = point - handler
			points.append(point)
			handlers1.append(handler1)
			handlers2.append(handler2)
		points.append(endpoint)
		handlers1.append(endpoint + endhandler)
		handlers2.append(endpoint - endhandler)
		VectorTriples = [list(h1)+list(p)+list(h2) for h1,p,h2 in zip(handlers1, points, handlers2)]
		print 'deb:calcArcCurve: handlers1:\n', handlers1  #---------
		print 'deb:calcArcCurve: points:\n', points  #---------
		print 'deb:calcArcCurve: handlers2:\n', handlers2  #---------
		print 'deb:calcArcCurve: VectorTriples:\n', VectorTriples  #---------
		return VectorTriples

def import_fcstd(filename):
    "main loop through FreeCAD objects"
    try:
        import FreeCAD
    except ValueError:
        Draw.PupMenu('Error%t|FreeCAD library not found.')
    else:
        sce = Scene.GetCurrent()
        import Part
        from draftlibs import fcvec
        Window.WaitCursor(1)
        t = Blender.sys.time()
        doc = FreeCAD.open(filename)
        objects = doc.Objects
        for ob in objects:
            meshdata={}
            meshdata['name']=ob.Name
            meshdata['verts']=[]
            meshdata['edges']=[]
            meshdata['faces']=[]
        
            if ob.Type[:4] == 'Part':
                shape = ob.Shape
                if shape.Faces:
		    print 'faces\n'
                    rawdata = shape.tessellate(1)
                    for v in rawdata[0]:
                        meshdata['verts'].append((v.x,v.y,v.z))
                    for f in rawdata[1]:
                        meshdata['faces'].append(f)
                elif shape.Edges:
                    print 'edges\n'
                    verts = []
                    for v in shape.Vertexes:
                        verts.append(v.Point)
                        meshdata['verts'].append((v.X,v.Y,v.Z))
                    for e in shape.Edges:
                        if isinstance(e.Curve,Part.Line):
                            i1 = fcvec.find(e.Vertexes[0].Point,verts)
                            i2 = fcvec.find(e.Vertexes[-1].Point,verts)
                            if (i1 != None) and (i2 != None):
                                meshdata['edges'].append((i1,i2))
                        elif isinstance(e.Curve,Part.Circle):
                            # provisorily, if we found an arc, we'll draw it as a new object
                            cen = e.Curve.Center
                            rad = e.Curve.Radius
                            if len(e.Vertexes) > 1:
                                sta = -math.degrees(fcvec.angle(e.Vertexes[0].Point.sub(cen)))
                                end = -math.degrees(fcvec.angle(e.Vertexes[-1].Point.sub(cen)))
                            else:
                                sta = 0
                                end = 360
                            center=(cen.x,cen.y,cen.z)
                            arcverts = calcArc(center, rad, sta, end, ARCSEGMENTS, None)
                            arcdata={}
                            arcdata['name']="Arc"
                            arcdata['verts']=arcverts
                            arcdata['edges']=[]
                            for i in range(len(arcverts)-1):
                                arcdata['edges'].append((i,i+1))
                            arcdata['faces']=[]
                            drawmesh(arcdata,sce)

                drawmesh(meshdata,sce)

            elif ob.Type[:4] == 'Mesh':
                print 'mesh\n'
                facets = ob.Mesh.Facets
                verts = []
                faces = []
                for f in facets:
                    index = []
                    for p in f.Points:
                        v = FreeCAD.Vector(p[0],p[1],p[2])
                        id = fcvec.find(v,verts)
                        if id != None: index.append(id)
                        else:
                            verts.append(v)
                            index.append(len(verts)-1)
                    faces.append(index)
                for v in verts:
                    meshdata['verts'].append((v.x,v.y,v.z))
                meshdata['faces'] = faces
                drawmesh(meshdata,sce)

            elif ob.Type == 'App::Annotation':
		print 'annotation\n'
                pos = (ob.Position.x,ob.Position.y,ob.Position.z)
                str = ''
                for l in ob.LabelText:
                    if str: str += "\n"
                    str += l
                txt = Text3d.New()
                txt.setSize(.5)
                txt.setText(str)
                obj = sce.objects.new(txt,ob.Name)
                obj.loc = pos
           
        # Timing the script is a good way to be aware on any speed hits when scripting
        print 'Import finished in %.2f seconds' % (Blender.sys.time()-t)
        Window.WaitCursor(0)
        Blender.Redraw()

def drawmesh(meshdata,sce):
    "draws meshes in blender"
    mes = Mesh.New()
    mes.verts.extend(meshdata['verts'])
    if meshdata['edges']: mes.edges.extend(meshdata['edges'])
    if meshdata['faces']: mes.faces.extend(meshdata['faces'])
    sce.objects.new(mes,meshdata['name'])
    
def main():
    Window.FileSelector(import_fcstd, 'IMPORT FCSTD', 
                        Blender.sys.makename(ext='.fcstd'))    

# This lets you import the script without running it
if __name__=='__main__':
    main()
