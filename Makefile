MONO_PATH=$(subst \,/,$(shell monomake path --bare))
include $(MONO_PATH)/predefines.mk

TARGET=chores

OBJECTS = \
	$(patsubst %.cpp,%.o,$(wildcard lib/*.cpp)) \
	$(patsubst %.cpp,%.o,$(wildcard *.cpp))

include $(MONO_PATH)/mono.mk
