# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= ../..

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS +=

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
LDFLAGS +=

SOURCES += $(wildcard src/*.cpp src/dsp/*.cpp src/dsp/kiss_fft/*.c src/widgets/*.cpp src/modules/Type35.cpp src/modules/DiodeVCF.cpp)
SOURCES += $(wildcard src/modules/BlankPanel.cpp src/modules/BlankPanelWood.cpp src/modules/VCO.cpp src/modules/VCO.cpp)
SOURCES += $(wildcard src/modules/SimpleFilter.cpp src/modules/ReShaper.cpp src/modules/BlankPanelEmpty.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) res

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk