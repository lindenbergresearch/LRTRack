SLUG = LindenbergResearch
VERSION = 0.6.5-beta3
SOURCES += $(wildcard src/*.cpp src/dsp/*.cpp src/dsp/kiss_fft/*.c src/widgets/*.cpp src/modules/*.cpp)
DISTRIBUTABLES += $(wildcard LICENSE*) res
RACK_DIR ?= ../..

include $(RACK_DIR)/plugin.mk