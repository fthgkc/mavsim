#! /usr/bin/env python
# encoding: utf-8
# James Goppert 

VERSION = '0.0.2'
APPNAME = 'mavsim'
SO_VERSION = '0.0.2'

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx qt4 boost')

def configure(conf):

    def check_pkg (name, version='', mandatory=True, var=None, args=''):
        if not var:
            var = name.split ('-')[0].upper ()
        conf.check_cfg (package=name, uselib_store=var, args='--cflags --libs ' + args,
            atleast_version=version, mandatory=mandatory)
        return conf.env['HAVE_' + var]

    # general
    conf.load('compiler_cxx qt4 boost')
    conf.check_boost('system thread', mt=True)
    conf.env.append_value('CXXFLAGS', '-g') # test
    conf.define('DATADIR', 'None')

    # mavlink
    conf.env.INCLUDES_MAVLINK = [
        'thirdParty/mavlink',
        'thirdParty/mavlink/include']

    # jsbsim
    conf.env.INCLUDES_JSBSIM = 'thirdParty/jsbsim/src'

    # scicos
    conf.env.INCLUDES_SCICOS = [
        '/usr/lib/scicoslab-gtk-4.4.1/routines',
        '/usr/lib/scicoslab-gtk-4.4/routines',
        '/usr/lib/scicoslab-gtk-4.4b7/routines']

    # pkg-config packages
    check_pkg('x11')
    check_pkg('openscenegraph','2.8')

    conf.write_config_header('src/config.h')

def build(bld):

    bld(
        name = 'common_includes',
        export_includes = 'src'
    )

    bld.stlib(
        target = 'mavsimCommunications',
        source = bld.path.ant_glob('src/communications/*.cpp'),
        includes = 'src/communications',
        use = 'common_includes'
    )

    bld.stlib(
        target = 'mavsimDynamics',
        source = bld.path.ant_glob('src/dynamics/*.cpp'),
        includes = 'src/dynamics',
        use = 'common_includes'
    )

    bld.stlib(
        target = 'mavsimNavigation',
        source = bld.path.ant_glob('src/navigation/*.cpp'),
        includes = 'src/navigation',
        use = 'common_includes'
    )

    bld.stlib(
        target = 'mavsimVisualization',
        source = bld.path.ant_glob('src/visualization/*.cpp'),
        includes = 'src/visualization',
        use = ['common_includes','BOOST','QTCORE','QTGUI','QTOPENGL','QTSVG'],
        defines = 'USE_QT4'
    )

    bld.stlib(
        target = 'mavsimScicos',
        source = bld.path.ant_glob('src/scicos/*.cpp'),
        includes = 'src/scicos',
        use = ['common_includes','MAVLINK','JSBSIM','SCICOS',
               'mavsimCommuication','mavsimDynamics',
               'mavsimNavigation','mavsimVisualization'],
        defines = 'USE_QT4'
    )

    #bld.program(
        #target   = 'mavsim',
        #source   = bld.path.ant_glob([
            #'app/trim/*.cpp',
            #'app/trim*.qrc',
            #'app/trim/*.ui']),
        #includes = '.',
        #use   = ['mavsimVisualization','mavsimMath','mavsimCommunication',
            #'JSBSIM','QTCORE','QTGUI','QTOPENGL','QTSVG','X11','OPENSCENEGRAPH'],
        #defines  = ['WAF=1','USE_QT4'], # test
        #lang     = bld.path.ant_glob('linguist/*.ts'),
        #langname = 'somefile', # include the .qm files from somefile.qrc
    #)
