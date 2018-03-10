#!/usr/bin/env bash
cd ../../../..
rm -rf .build

make ergodox_ez:fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes 
cp ergodox_ez_fromtonrouge.hex "$OLDPWD"/ez-colemak-jackdaw-azerty.hex
make ergodox_ez:fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no
cp ergodox_ez_fromtonrouge.hex "$OLDPWD"/ez-colemak-jackdaw-qwerty.hex
make ergodox_ez:fromtonrouge PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no
cp ergodox_ez_fromtonrouge.hex "$OLDPWD"/ez-qwerty-jackdaw-qwerty.hex

# Note: To flash the firmware just use: sudo dfu-util -D [firmware_file]
# eg: sudo dfu-util -D infinity-colemak-jackdaw-azerty-right.bin 
make ergodox_infinity:fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes
cp ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity-colemak-jackdaw-azerty-left.bin
make ergodox_infinity:fromtonrouge MASTER=right PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes
cp ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity-colemak-jackdaw-azerty-right.bin
make ergodox_infinity:fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no
cp ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity-colemak-jackdaw-qwerty-left.bin
make ergodox_infinity:fromtonrouge MASTER=right PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no
cp ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity-colemak-jackdaw-qwerty-right.bin
make ergodox_infinity:fromtonrouge PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no
cp ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity-qwerty-jackdaw-qwerty-left.bin
make ergodox_infinity:fromtonrouge MASTER=right PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no
cp ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity-qwerty-jackdaw-qwerty-right.bin

cd "$OLDPWD"
