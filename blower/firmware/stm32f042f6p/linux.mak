ifneq ($(OS),Windows_NT)
	CC := arm-none-eabi-gcc
	CXX := arm-none-eabi-g++
	LD := $(CXX)
	AR := arm-none-eabi-ar
	OBJCOPY := arm-none-eabi-objcopy
	USE_DEL_TO_CLEAN := 0
	INCLUDE_DIRS := $(subst \,/,$(INCLUDE_DIRS))
endif