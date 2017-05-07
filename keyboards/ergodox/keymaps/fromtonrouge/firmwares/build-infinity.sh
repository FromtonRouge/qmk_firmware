#!/usr/bin/env bash
cd ../../../../..
rm -f .build/infinity-*.bin

# Infinity ErgoDox binaries

make keyboard=ergodox subproject=infinity keymap=fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes .build/infinity-colemak-jackdaw-azerty-left.bin
make keyboard=ergodox subproject=infinity keymap=fromtonrouge MASTER=right PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes .build/infinity-colemak-jackdaw-azerty-right.bin

cp .build/infinity-*.bin "$OLDPWD"/infinity/

cd "$OLDPWD"
