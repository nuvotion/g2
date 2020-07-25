
ifeq ("$(BOARD)","psoc5lp")
    _BOARD_FOUND = 1

    DEVICE_DEFINES += MOTATE_CONFIG_HAS_USBSERIAL=1

    #FIRST_LINK_SOURCES += $(sort $(wildcard ${MOTATE_PATH}/Atmel_sam_common/*.cpp)) $(sort $(wildcard ${MOTATE_PATH}/Atmel_sam3x/*.cpp))

    CHIP = PSOC5LP
    #export CHIP
    #CHIP_LOWERCASE = psoc5lp
    CROSS_COMPILE = arm-psoc-eabi
    CPU_DEV = cortex-m3

    #CPPFLAGS += -std=c++11

    # Note: we call it "g2core-due" instead of "due" since the Motate built-in provides
    # a "due" BASE_BOARD.
    BOARD_PATH = board/psoc5lp
    SOURCE_DIRS += ${BOARD_PATH} device/step_dir_driver

    DEVICE_DEFINES += MOTATE_BOARD="nuvotion"


    DEVICE_LINKER_SCRIPT_PATH = $(BOARD_PATH)
    DEVICE_LINKER_SCRIPT = $(BOARD_PATH)/cm3gcc.ld

    include $(MOTATE_PATH)/arch/arm.mk

    DEVICE_LIBS += :application.a
    #DEVICE_LIB_DIRS += $(BOARD_PATH)
    #PLATFORM_BASE = ${MOTATE_PATH}/platform/atmel_sam

    #include $(PLATFORM_BASE).mk
endif
