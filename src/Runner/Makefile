THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = ValkyrieRunner.app

WX_INCLUDE                  = $(shell wx-config --cxxflags)
WX_LINK                     = $(shell wx-config --libs)

FILE_WARNING_FLAGS          = -Wno-ignored-qualifiers -Wno-unknown-pragmas
CXXFLAGS                    += $(WX_INCLUDE)
LDFLAGS                     += $(WX_LINK)

LINK_LIBS                   = ThorsUtil ThorsUI Valkyrie

include $(THORSANVIL_ROOT)/build/tools/Makefile
