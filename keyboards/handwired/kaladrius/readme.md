# Kaladrius Steno Keyboard

![Kaladrius](https://i.imgur.com/YgC8tji.jpg)

The Kaladrius is an orthographic steno keyboard inspired by the [Jackdaw steno theory](https://sites.google.com/site/ploverdoc/jackdaw)
The main goal is to be able to code with a chorded keyboard. It's still a prototype and hope to use it daily this year (2019) at work.
It's a handwired and printable keyboard and use a Teensy 3.2

The keyboard was designed with OpenSCAD
![LinkSystem](https://i.imgur.com/hR2zoJJ.png)
![Case](https://i.imgur.com/a4Rtx1p.png)
![Plate](https://i.imgur.com/dIb330e.png)

# Hardware

You'll need : 

- [1x Teensy 3.2 with pins](https://www.pjrc.com/store/teensy32_pins.html)
- [1x (4cm x 6cm) prototype pcb](https://www.amazon.com/ELEGOO-Prototype-Soldering-Compatible-Arduino/dp/B072Z7Y19F)
- 1x 5mm Common Cathode RGB led
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

    make handwired/kaladrius:[your_layout]

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

