THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = Valkyrie.slib

WX_INCLUDE                  = $(shell wx-config --cxxflags)
WX_LINK                     = $(shell wx-config --libs)

FILE_WARNING_FLAGS          = -Wno-ignored-qualifiers -Wno-unknown-pragmas
CXXFLAGS                    += $(WX_INCLUDE)
LDFLAGS                     += $(WX_LINK)

LINK_LIBS                   = ThorsUI ThorsUtil

include $(THORSANVIL_ROOT)/build/tools/Makefile
