AC_INIT([Valkyrie], [0.1], [YourName@Email.com])
AC_PREREQ([2.65])

# Set up the build system
if [[ ! -e build/Notes ]]; then
    git submodule init
    git submodule update
fi
AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/build])
AX_THOR_FUNC_BUILD


AC_CONFIG_SRCDIR([src/Valkyrie/Runner.cpp])

# Find the C++ Copiler
# The minimumm requirements.
AC_PROG_CXX
AX_CHECK_COMPILE_FLAG([-ansi], [AC_SUBST([CXXFLAGS], ["-O3 -ansi"])])
# Check what version of the C++ language is supported
AX_THOR_FUNC_LANG_FLAG(17)

# Find the code coverage tool.
# All the testing will depend on this.
AX_THOR_PROG_COV

# Add support for shared libraries.
LT_INIT


#
# Add your defintions in here.
# Note there are some predefined macros in build/autotools/m4/
AC_CHECK_LIB([ThorsUI17], [_ZTIN10ThorsAnvil7ThorsUI11AnimateableE], [], AC_MSG_ERROR([

    This library depends on the ThorsUI
    It can be found here: https://github.com/Loki-Astari/ThorsUI
    Install with:
        > git clone git@github.com:Loki-Astari/ThorsUI.git
        > cd ThorsUI
        > ./configure
        > make
        > sudo make install

]))
AC_CHECK_LIB([ThorsUtil17], [_ZN10ThorsAnvil9ThorsUtil6Random7getSeedEv], [], AC_MSG_ERROR([

    This library depends on the ThorsUtil
    It can be found here: https://github.com/Loki-Astari/ThorsUtil
    Install with:
        > git clone git@github.com:Loki-Astari/ThorsUtil.git
        > cd ThorsUtil
        > ./configure
        > make
        > sudo make install

]))



# Build all the Makefiles and configuration files.
# Used by ThorMaker
# Note: you can push the config file to sub directories in the AC_CONFIG_HEADERS macro (see example)
# Note: Local Make variables should be placed in Makefile.config.in
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([ src/Valkyrie/ValkyrieConfig.h src/Maze/ValkyrieMazeConfig.h src/Walker/ValkyrieWalkerConfig.h ])
AC_CONFIG_FILES([Makefile.extra Makefile.config:build/autotools/build/Makefile.config.in:Makefile.config.in])
AC_OUTPUT
