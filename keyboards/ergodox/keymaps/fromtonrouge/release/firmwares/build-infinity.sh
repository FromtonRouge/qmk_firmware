#!/usr/bin/env bash
# Note: To flash the firmware just use: sudo dfu-util -D [firmware_file]
# eg: sudo dfu-util -D infinity-colemak-jackdaw-azerty-right.bin 
cd ../../../../../..
rm -f .build/infinity-*.bin
make keyboard=ergodox subproject=infinity keymap=fromtonrouge PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes .build/infinity-colemak-jackdaw-azerty-left.bin
make keyboard=ergodox subproject=infinity keymap=fromtonrouge MASTER=right PROGRAMMER_COLEMAK_ENABLE=yes AZERTY_OS_ENABLE=yes .build/infinity-colemak-jackdaw-azerty-right.bin
cd "$OLDPWD"
