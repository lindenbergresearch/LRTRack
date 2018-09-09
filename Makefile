SLUG = LindenbergResearch
VERSION = 0.6.3-beta3
SOURCES += $(wildcard src/*.cpp src/dsp/*.cpp src/widgets/*.cpp)
DISTRIBUTABLES += $(wildcard LICENSE*) res
RACK_DIR ?= ../..

include $(RACK_DIR)/plugin.mk