# Kaladrius Steno Keyboard

![Kaladrius](https://i.imgur.com/hgo1kl6.jpg)

The Kaladrius is an orthographic steno keyboard inspired by the [Jackdaw steno theory](https://sites.google.com/site/ploverdoc/jackdaw)
The main goal is to be able to code with a chorded keyboard. It's still a prototype and hope to use it daily this year (2019) at work.
It's a handwired and printable keyboard and use a Teensy++ 2.0

# Hardware

You can't buy this keyboard and have to build it yourself. You'll have to do the electronics and also have to print the case.

You'll need :

    * [1x Teensy++ 2.0 with pins][1]
    * 1x mcp23018
    * 2x (5cm x 7cm) prototype pcb, I took it from this kit [Amazon][2]
    * 2x TRRS 3.5 mm Jack Breakout Board [Amazon][3]
    * 1x male 2.54mm header pin
    * 2x 5mm RGB leds
    * 70x 1N4148 Commutation diodes
    * 70x Gateron Clear Keyswitch - Plate Mount (35g)
    * 2x 2.2 kOhms resistors
    * 3x ??? kOhms resistors
    * 6x ??? kOhms resistors
    * Wires
    * TODO

[1]: https://www.pjrc.com/store/teensypp_pins.html
[2]: https://www.amazon.com/ELEGOO-Prototype-Soldering-Compatible-Arduino/dp/B072Z7Y19F
[3]: https://www.amazon.com/dp/B01N7NDCVI/ref=cm_sw_r_tw_dp_U_x_TXwCCb46PW6FF

Tools :

    * 1 Hot Glue gun + Glue Sticks
    * TODO

# Firmware
    By default the keyboard is in full NKRO, you can test the keyboard here http://random.xem.us/rollover.html

    * Keyboard Maintainer: [FromtonRouge](https://github.com/FromtonRouge)
    * Hardware Supported: Teensy++ 2.0

Make example for this keyboard (after setting up your build environment):

    make handwired/kaladrius:[your_layout]

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
