SLUG = LindenbergResearch
VERSION = 0.6.1_beta

SOURCES += $(wildcard src/*.cpp src/dsp/*.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) res

RACK_DIR ?= ../..
include $(RACK_DIR)/plugin.mk
