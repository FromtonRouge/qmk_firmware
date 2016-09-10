# Having a file like this allows you to override Makefile definitions
# for your own particular keymap

BOOTMAGIC_ENABLE = yes # Needed for activating NKRO (maintain Space+N for 3 seconds on boot)
NKRO_ENABLE      = yes # Maintain Space+N for 3 seconds on boot (toggle between NKRO and 6KRO), go here to test NKRO https://www.microsoft.com/appliedsciences/KeyboardGhostingDemo.mspx
SLEEP_LED_ENABLE = no # Breathing sleep LED during USB suspend
MOUSEKEY_ENABLE  = no # Mouse keys(+4700)
COMMAND_ENABLE   = no # Commands for debug and configuration
AUTO_SPACE_ENABLE = yes # In steno mode add automatically a space after each strokes except if the no-space key has been pressed
AZERTY_OS_ENABLE = yes # AZERTY OS layout, set to no if your OS is in QWERTY
PROGRAMMER_COLEMAK_ENABLE   = yes # Programmer Colemak as a base layer, set to no for your default OS layer

ifeq ($(strip $(AUTO_SPACE_ENABLE)), yes)
    OPT_DEFS += -DAUTO_SPACE_ENABLE
endif

ifeq ($(strip $(AZERTY_OS_ENABLE)), yes)
    OPT_DEFS += -DAZERTY_OS_ENABLE
endif

ifeq ($(strip $(PROGRAMMER_COLEMAK_ENABLE)), yes)
    OPT_DEFS += -DPROGRAMMER_COLEMAK_ENABLE
endif
