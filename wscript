#! /usr/bin/env python
# encoding: utf-8
# James Goppert 

VERSION='0.0.0'
APPNAME='mavsim'
SO_VERSION='0.0.0'

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx qt4')

def configure(conf):
    conf.load('compiler_cxx qt4')
    conf.env.append_value('CXXFLAGS', ['-g']) # test

    conf.define('DATADIR', None)
    conf.write_config_header('src/config.h')

def build(bld):

    bld.shlib(
        source=bld.path.ant_glob('src/communications/*.cpp'),
        target='communications',
        includes='src src/communications',
        export_includes='src',
        vnum=SO_VERSION)

    bld.shlib(
        source=bld.path.ant_glob('src/dynamics/*.cpp'),
        target='dynamics',
        includes='src src/dynamics',
        export_includes='src',
        vnum=SO_VERSION)

    bld.shlib(
        source=bld.path.ant_glob('src/navigation/*.cpp'),
        target='navigation',
        includes='src src/navigation',
        export_includes='src',
        vnum=SO_VERSION)

    bld.shlib(
        source=bld.path.ant_glob('src/visualization/*.cpp'),
        use='QTCORE QTGUI QTOPENGL QTSVG',
        target='visualization',
        includes='src src/visualization',
        defines='WFlags',
        export_includes='src',
        vnum=SO_VERSION)

    bld.shlib(
        source=bld.path.ant_glob('src/scicos/*.cpp'),
        use='QTCORE QTGUI QTOPENGL QTSVG',
        target='scicos',
        includes='src src/scicos',
        defines='WFlags=""',
        export_includes='src',
        vnum=SO_VERSION)

    #bld.program(
        #features = 'qt4 cxx cxxprogram',
        #use   = 'visualization math QTCORE QTGUI QTOPENGL QTSVG',
        #source   = bld.path.ant_glob('app/trim/*.cpp \
            #app/trim*.qrc app/trim/*.ui'),
        #target   = 'mavsim',
        #includes = '.',
        #defines  = 'WAF=1', # test
        #lang     = bld.path.ant_glob('linguist/*.ts'),
        #langname = 'somefile', # include the .qm files from somefile.qrc
    #)
