# Kaladrius Steno Keyboard (standard version)

![Kaladrius](https://i.imgur.com/hgo1kl6.jpg)

The Kaladrius is an orthographic steno keyboard inspired by the [Jackdaw steno theory](https://sites.google.com/site/ploverdoc/jackdaw)
The main goal is to be able to code with a chorded keyboard. It's still a prototype and hope to use it daily this year (2019) at work.
It's a handwired and printable keyboard and use a Teensy 3.2

# Hardware

You can't buy this keyboard and have to build it yourself. You'll have to do the electronics and also have to print the case.

You'll need : 

- [1x Teensy 3.2 with pins](https://www.pjrc.com/store/teensy32_pins.html)
- [1x prototype pcb](https://www.amazon.com/ELEGOO-Prototype-Soldering-Compatible-Arduino/dp/B072Z7Y19F)
- 1x 5mm RGB led
- 70x 1N4148 Commutation diodes
- 70x Gateron Clear Keyswitch - Plate Mount (35g)
- 3x 220 Ohms resistors (for the RGB led)
- Wires
- TODO

Tools :

- 1 Hot Glue gun + Glue Sticks
- TODO

# Firmware

By default the keyboard is in full NKRO, you can test the keyboard here http://random.xem.us/rollover.html

- Keyboard Maintainer: [FromtonRouge](https://github.com/FromtonRouge)
- Hardware Supported: Teensy 3.2

Make example for this keyboard (after setting up your build environment):

    make handwired/kaladrius/standard:[your_layout]

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

