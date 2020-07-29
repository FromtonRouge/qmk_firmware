# Kaladrius Steno Keyboard

![Kaladrius](https://i.imgur.com/UF7BSkB.jpg)

The Kaladrius is an orthographic steno keyboard inspired by the [Jackdaw steno theory](https://sites.google.com/site/learnplover/jackdaw)
The main goal is to be able to code with a chorded keyboard. It's still a prototype and hope to use it daily this year (2020) at work.
It's a handwired and printable keyboard and use a Teensy 3.2

# Why this keyboard ?

I'm a C++ Tools programmer in the video game industry and I always wanted to improve my typing speed. I changed several times my keyboard layout in my career :
My original layout was Azerty (with a top speed at 86 wpm on 10fastfingers), in 2014 I changed to Dvorak (with a top speed at 98 wpm) and in 2016 I finally changed to Colemak (top speed at 103 wpm).

I discovered Plover and Steno the same year and was blown away by this [presentation](https://www.youtube.com/watch?v=Wpv-Qb-dB6g)

I decided to learn Plover but found that it might not suitable for C++ because of the phonetic/mnemonic approach.
That's why I got interested in Orthographic Steno Systems and found the "Jackdaw steno theory" on the [Learn Plover site](https://sites.google.com/site/learnplover/jackdaw)

I first implemented an ErgoDox Layout for Jackdaw and learned to type basics words on 10fastfingers.com. After a lot of iterations on the firmware I started to practice more but on April 2018 I injured my left hand after a too long training session.
Actually I got a De Quervain tendinitis because of the very bad ErgoDox thumb positions (too far!). Note: at this time my top speed on the 100 words test was 140 wpm with Jackdaw.

I had a surgery in October 2018...
![Surgery](https://i.imgur.com/xZ3XdZM.jpg)
(just after the surgery)

![Scar](https://i.imgur.com/OIYHb7z.jpg)
(I won a beautiful scar)

So I highly don't recommend the ErgoDox or similar keyboards for Jackdaw.

After the surgery I decided to build a specific keyboard for Jackdaw so I can practice safely. The keyboard needs the following :

- Thumbs keys must be closer to the index column to avoid bad wrists movements
- Thumbs layout must be optimized for Jackdaw
- Because of the 2 previous points the keyboard must 3D Printed and the design must be parametric (that's why I chose OpenSCAD)
- It's possible to switch to a "normal" keyboard layout (for me Colemak)
- The keyboard is easy to build even if it takes time. That's why it's not really splitted
- The development board must have enough memory for several dictionaries (that's why I chose the Teensy 3.2)

I named it "Kaladrius" because the Keyboard looks like a white bird and in Roman mythology the Caladrius is a white bird that can heal someone...

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

