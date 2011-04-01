APPNAME='geepro'
VERSION='0.0.3'


top = '.'
out = 'build_directory'

def options(opt):
	# command-line options provided by a waf tool
	opt.tool_options('compiler_cxx')


def configure(conf):
  print('→ configuring the project')
  conf.check_tool('gcc g++ intltool')
  conf.env.CPPFLAGS  = ['-O2','-Wall']
  conf.env.CXXFLAGS  = ['-O2','-Wall']
  conf.recurse('gui')
  conf.env.APPNAME     = APPNAME
  conf.env.LIB_C       = 'c'
  conf.env.LIB_Dl      = 'dl'
  conf.env.LINKFLAGS_DL = ['-rdynamic']
  conf.check_cfg(package='gtk+-2.0'  , atleast_version='0.0.0')
  conf.check_cfg(package='cairo'     , atleast_version='0.0.0')
  conf.check_cfg(package='libxml-2.0', atleast_version='0.0.0')
  conf.check_cfg(package='gtk+-2.0'  , args='--cflags --libs')
  conf.check_cfg(package='cairo'     , args='--cflags --libs')
  conf.check_cfg(package='libxml-2.0', args='--cflags --libs')

  conf.define('PACKAGE'           , APPNAME)
  conf.define('DEFAULT_PLUGINS_PATH'       , conf.env.PREFIX+'/lib/geepro/plugins')
  conf.define('DEFAULT_DRIVERS_PATH'       , conf.env.PREFIX+'/lib/geepro/drivers')
  conf.define('DEFAULT_SHARE_DRIVERS_PATH', conf.env.PREFIX+'/share/geepro/drivers')

  conf.write_config_header('src/config.h')



def build(bld):
  print('→ building the project')
  bld.recurse('doc')
  bld.recurse('pixmaps')
  bld.recurse('intl')
  bld.recurse('drivers')
  bld.recurse('gui')
  bld.recurse('plugins')
  bld.recurse('src')
  #bld.recurse('po')
  #bld.use_the_magic()

  bld(
    features     = 'cxx cprogram',
    add_objects  = ['maincode',"main.o"],
    use          = ['gui'],
    uselib       = ['GTK+-2.0','CAIRO','LIBXML-2.0','DL'],
    target       = bld.env.APPNAME,
  )
