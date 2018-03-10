# Removing useless things to reduce the firmware size
API_SYSEX_ENABLE = no # Useless for us
RGBLIGHT_ENABLE  = no # Useless for us
NKRO_ENABLE      = yes # FORCE_NKRO is defined in config.h, go here to test NKRO https://www.microsoft.com/appliedsciences/KeyboardGhostingDemo.mspx
MOUSEKEY_ENABLE  = no # Mouse keys(+4700)
COMMAND_ENABLE   = no # Commands for debug and configuration
EXTRAKEY_ENABLE = no
SLEEP_LED_ENABLE = no
UNICODE_ENABLE = no
ONEHAND_ENABLE = no

# Personal keymap configuration variables
AZERTY_OS_ENABLE = yes # AZERTY OS layout, set to no if your OS is in QWERTY
PROGRAMMER_COLEMAK_ENABLE = yes # Programmer Colemak as a base layer, set to no for your default OS layer

ifeq ($(strip $(AZERTY_OS_ENABLE)), yes)
    OPT_DEFS += -DAZERTY_OS_ENABLE
endif

ifeq ($(strip $(PROGRAMMER_COLEMAK_ENABLE)), yes)
    OPT_DEFS += -DPROGRAMMER_COLEMAK_ENABLE
endif

SRC += undo.c \
       stroke.c \
       lookup_tables/tables.c \
       lookup_tables/shelton_tables.c \
       lookup_tables/user_tables.c
