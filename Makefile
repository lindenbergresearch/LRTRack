#
#       __   ___  ______
#      / /  / _ \/_  __/
#     / /__/ , _/ / /    Lindenberg
#    /____/_/|_| /_/  Research Tec.
#
#
#	 https://github.com/lindenbergresearch/LRTRack
#    heapdump@icloud.com
#
#    Sound Modules for VCV Rack
#    Copyright 2017-2019 by Patrick Lindenberg / LRT
#
#    For Redistribution and use in source and binary forms,
#    with or without modification please see LICENSE.
#
#
RACK_DIR ?= ../..

FLAGS +=
CFLAGS +=
CXXFLAGS +=
LDFLAGS +=

SOURCES += $(wildcard src/*.cpp src/dsp/*.cpp src/dsp/kiss_fft/*.c src/widgets/*.cpp src/modules/Type35.cpp src/modules/DiodeVCF.cpp)
SOURCES += $(wildcard src/modules/BlankPanel.cpp src/modules/BlankPanelWood.cpp src/modules/VCO.cpp src/modules/VCO.cpp)
SOURCES += $(wildcard src/modules/SimpleFilter.cpp src/modules/ReShaper.cpp src/modules/BlankPanelEmpty.cpp src/modules/BlankPanelSmall.cpp)
SOURCES += $(wildcard src/modules/AlmaFilter.cpp src/modules/MS20Filter.cpp src/modules/Westcoast.cpp src/modules/QuickMix.cpp src/modules/VULevelMeter.cpp src/modules/TestDriver.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) res

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk