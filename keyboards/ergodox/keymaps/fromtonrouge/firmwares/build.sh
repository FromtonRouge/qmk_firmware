#!/usr/bin/env bash
cd ../../../../..
rm -rf .build

# ErgoDox EZ binaries

make ergodox-fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes 
cp ergodox_ez_fromtonrouge.hex "$OLDPWD/ez"/ez-colemak-jackdaw-azerty.hex

make ergodox-fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no
cp ergodox_ez_fromtonrouge.hex "$OLDPWD/ez"/ez-colemak-jackdaw-qwerty.hex

make ergodox-fromtonrouge PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no
cp ergodox_ez_fromtonrouge.hex "$OLDPWD/ez"/ez-qwerty-jackdaw-qwerty.hex

# Infinity ErgoDox binaries

make keyboard=ergodox subproject=infinity keymap=fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes .build/infinity-colemak-jackdaw-azerty-left.bin
make keyboard=ergodox subproject=infinity keymap=fromtonrouge MASTER=right PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes .build/infinity-colemak-jackdaw-azerty-right.bin

make keyboard=ergodox subproject=infinity keymap=fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no .build/infinity-colemak-jackdaw-qwerty-left.bin
make keyboard=ergodox subproject=infinity keymap=fromtonrouge MASTER=right PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=no .build/infinity-colemak-jackdaw-qwerty-right.bin

make keyboard=ergodox subproject=infinity keymap=fromtonrouge PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no .build/infinity-qwerty-jackdaw-qwerty-left.bin
make keyboard=ergodox subproject=infinity keymap=fromtonrouge MASTER=right PROGRAMMER_COLEMAK_ENABLE=no AZERTY_OS_ENABLE=no .build/infinity-qwerty-jackdaw-qwerty-right.bin

cp .build/infinity-*.bin "$OLDPWD"/infinity/

cd "$OLDPWD"
