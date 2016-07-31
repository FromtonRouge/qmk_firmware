# Having a file like this allows you to override Makefile definitions
# for your own particular keymap

SLEEP_LED_ENABLE = no # Breathing sleep LED during USB suspend
MOUSEKEY_ENABLE  = no # Mouse keys(+4700)
COMMAND_ENABLE   = no # Commands for debug and configuration
AZERTY_OS_ENABLE = yes # AZERTY OS layout, set to no if your OS is in QWERTY
PROGRAMMER_COLEMAK_ENABLE   = yes # Programmer Colemak as a base layer, set to no for your default OS layer

ifeq ($(strip $(AZERTY_OS_ENABLE)), yes)
    OPT_DEFS += -DAZERTY_OS_ENABLE
endif

ifeq ($(strip $(PROGRAMMER_COLEMAK_ENABLE)), yes)
    OPT_DEFS += -DPROGRAMMER_COLEMAK_ENABLE
endif
