import os

# SCons options

SetOption('num_jobs', 4)      # Parallelize

# clang build environment

defines = Variables(None, ARGUMENTS)
defines.Add('verbose', 'Set to 1 to display ALL output', 0)
defines.Add('console', 'Set to 1 to display console output', 1)
defines.Add('trclog', 'Set to 1 to display trace log output', 0)
defines.Add('errlog', 'Set to 1 to display error log output', 1)

clang = Environment(
      ### Pass in defines to the C/C++ compiler ###

      variables = defines,
      CPPDEFINES = {
         'VERBOSE' : '${verbose}',
         'SHOW_CONSOLE' : '${console}',
         'SHOW_TRACELOG' : '${trclog}',
         'SHOW_ERRORLOG' : '${errlog}',
         },

      ### Propagate all external environment variables ###
      # Note: this is CRUCIAL or else some apps may not work.
      # For example, SDL will complain about XDG_RUNTIME_DIR not being set.

      ENV = os.environ,

      ### C/C++ ###

      CC = 'clang',
      CXX = 'clang++',

      CPPPATH = Split('include /usr/include /usr/local/include'),

      ### Flags ###

      CPPFLAGS = Split('-Wall -pedantic -std=c++11'),

      ### Libs ###

      LIBS = Split('m SDL2 SDL2_net'),
      LIBPATH = Split('/usr/lib usr/local/lib')
   )
clang['ENV']['LD_LIBRARY_PATH'] = '/usr/local/lib'

clang_debug = clang.Clone()
clang_debug.Append(CPPFLAGS = '-g')

# Vars

exe = 'navalngage'
srcdir = 'src'
objdir = 'obj'

# Separate src files from obj directory

VariantDir(objdir, srcdir, duplicate=0)

# Build away!


prod = clang.Program(exe, Glob(objdir + '/*.cpp') )
# clang_debug.Program(exe + '_debug', Glob(objdir + '/*.cpp') )

run = clang.Command('run', str(prod[0]), prod[0].abspath)
AlwaysBuild(run)