ifeq ("$(BOARD)","psoc5lp")
    _BOARD_FOUND = 1

    DEVICE_DEFINES += MOTATE_CONFIG_HAS_USBSERIAL=1

    CHIP = PSOC5LP
    CROSS_COMPILE = arm-psoc-eabi
    CPU_DEV = cortex-m3

    BOARD_PATH = board/psoc5lp
    SOURCE_DIRS += ${BOARD_PATH} # device/step_dir_driver

    DEVICE_DEFINES += MOTATE_BOARD="nuvotion"
    DEVICE_DEFINES += SETTINGS_FILE=${SETTINGS_FILE}

    DEVICE_LINKER_SCRIPT_PATH = $(BOARD_PATH)
    DEVICE_LINKER_SCRIPT = $(BOARD_PATH)/cm3gcc.ld

    include $(MOTATE_PATH)/arch/arm.mk

    DEVICE_LIBS += :application.a
endif
