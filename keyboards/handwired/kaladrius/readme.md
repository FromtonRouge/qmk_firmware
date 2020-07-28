# Kaladrius Steno Keyboard

![Kaladrius](https://i.imgur.com/UF7BSkB.jpg)

The Kaladrius is an orthographic steno keyboard inspired by the [Jackdaw steno theory](https://sites.google.com/site/learnplover/jackdaw)
The main goal is to be able to code with a chorded keyboard. It's still a prototype and hope to use it daily this year (2020) at work.
It's a handwired and printable keyboard and use a Teensy 3.2

# Why this keyboard ?

I'm a C++ Tools programmer in the video game industry and I changed several times my keyboard layout in my career.
My first layout was Azerty (with a top speed at 86 wpm on 10fastfingers), in 2014 I changed to Dvorak (with a top speed at 98 wpm) and in 2016 I finally changed to Colemak (top speed at 103 wpm).
I discovered Plover and Steno the same year and was blown away by this [presentation](https://www.youtube.com/watch?v=Wpv-Qb-dB6g)
I decided to learn Plover but found that it might not suitable for C++ because of the phonetic/mnemonic approach.
That's why I got interested in Orthographic Steno Systems and found the "Jackdaw steno theory" on the [Learn Plover site](https://sites.google.com/site/learnplover/jackdaw)

I first implemented an ErgoDox Layout for Jackdaw and learned to type basics words on 10fastfingers.com. After a lot of iterations on the firmware I started to practice more but on April 2018 I got a severe injurie on the left hand.
I got a De Quervain tendinitis because of the very bad ErgoDox thumb positions (too far!). Note: at this time my top speed on the 100 words test was 140 wpm.

I had a surgery in October 2018
![Surgery](https://photos.google.com/share/AF1QipPt-c5Y7qMSv_zQ-_LJV8rm6kJ9OSU4TSBbU2JUNNVW1be8bX53Igr6izPweGMGnQ/photo/AF1QipP60jy0aZQ4pIS2sJnVX_9ydRzBIb99NYLA7_OK?key=Tml0alhHWmgxNk9zZDZ6R2NOR3ZfRmkyYkVNRjJR)

After the surgery I decided to build a specific keyboard for Jackdaw to avoid the pain. I named it "Kaladrius" because the Keyboard looks like a white bird and in Roman mythology the Caladrius is a white bird that can heal someone :D

![Kaladrius](https://i.imgur.com/UF7BSkB.jpg)


# 3D Printed parts

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

