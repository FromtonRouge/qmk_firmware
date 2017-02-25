#!/usr/bin/env bash
cd ../../../../..

# ErgoDox EZ binaries

make ergodox-fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes 
cp ergodox_ez_fromtonrouge.hex "$OLDPWD/ez"/ez-colemak-jackdaw-azerty.hex

make ergodox-fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no
cp ergodox_ez_fromtonrouge.hex "$OLDPWD/ez"/ez-colemak-jackdaw-qwerty.hex

make ergodox-fromtonrouge PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no
cp ergodox_ez_fromtonrouge.hex "$OLDPWD/ez"/ez-qwerty-jackdaw-qwerty.hex

# Infinity ErgoDox binaries

make ergodox-infinity-fromtonrouge-dfu-util PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes
cp .build/ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity/infinity-colemak-jackdaw-azerty-left.bin
make ergodox-infinity-fromtonrouge-dfu-util MASTER=right PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes
cp .build/ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity/infinity-colemak-jackdaw-azerty-right.bin

make ergodox-infinity-fromtonrouge-dfu-util PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no
cp .build/ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity/infinity-colemak-jackdaw-qwerty-left.bin
make ergodox-infinity-fromtonrouge-dfu-util MASTER=right PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no
cp .build/ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity/infinity-colemak-jackdaw-qwerty-right.bin

make ergodox-infinity-fromtonrouge-dfu-util PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no
cp .build/ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity/infinity-qwerty-jackdaw-qwerty-left.bin
make ergodox-infinity-fromtonrouge-dfu-util MASTER=right PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no
cp .build/ergodox_infinity_fromtonrouge.bin "$OLDPWD"/infinity/infinity-qwerty-jackdaw-qwerty-right.bin

cd "$OLDPWD"
