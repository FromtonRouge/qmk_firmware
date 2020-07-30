# Kaladrius Steno Keyboard

![Kaladrius](https://i.imgur.com/UF7BSkB.jpg)

The Kaladrius is an orthographic steno keyboard inspired by the [Jackdaw steno theory](https://sites.google.com/site/learnplover/jackdaw).
Dictionaries are stored in the firmware and Plover is not needed.
The main goal is to be able to code with a chorded keyboard. It's still a prototype and hope to use it daily this year (2020) at work.
It's a handwired and printable keyboard and use a Teensy 3.2

# Why this keyboard ?

I'm a C++ Tools programmer in the video game industry and I always wanted to improve my typing speed. I changed several times my keyboard layout in my career :
My original layout was Azerty (with a top speed at 86 wpm on 10fastfingers), in 2014 I changed to Dvorak (with a top speed at 98 wpm) and in 2016 I finally changed to Colemak (top speed at 103 wpm).

I discovered Plover and Steno in 2016 too and was blown away by this [presentation](https://www.youtube.com/watch?v=Wpv-Qb-dB6g)

I decided to learn Plover but found that it might not suitable for C++ because of the phonetic/mnemonic approach.
That's why I got interested in Orthographic Steno Systems and found the "Jackdaw steno theory" on the [Learn Plover site](https://sites.google.com/site/learnplover/jackdaw)

I first implemented an ErgoDox Layout for Jackdaw and learned to type basics words on 10fastfingers.com. After a lot of iterations on the firmware I started to practice more but on April 2018 I injured my left hand after a too long training session.
Actually I got a De Quervain tendinitis because of the very bad ErgoDox thumb positions. Note: at this time my top speed on the 100 words test was 140 wpm with Jackdaw.

I had a surgery in October 2018...
![Surgery](https://i.imgur.com/xZ3XdZM.jpg)
(just after the surgery)

![Scar](https://i.imgur.com/OIYHb7z.jpg)
(I won a scar)

So I highly don't recommend the ErgoDox or similar keyboards for Jackdaw.

After the surgery I decided to build a specific keyboard for Jackdaw so I can practice safely. The keyboard needs the following :

- Thumbs keys must be closer to the index column to avoid bad wrists movements
- Thumbs layout must be optimized for Jackdaw
- Because of the 2 previous points the keyboard must be 3D Printed and the design must be parametric (that's why I chose OpenSCAD)
- It's possible to switch to a "normal" keyboard layout
- The keyboard is easy to build even if it takes time. That's why it's not really a splitted keyboard
- The development board must have enough memory for several dictionaries (that's why I chose the Teensy 3.2)

I named it "Kaladrius" because the Keyboard looks like a white bird and in Roman mythology the Caladrius is a white bird that can heal someone...

# Before continuing...

If you are interested by using Jackdaw and the Kaladrius here are some things to know before continuing :

- First it's an orthographic system, it's slower than steno
- It appears that by learning Jackdaw you won't loose your touch typing speed
- It's only for the english language but...
- ... english is not my first language I may not be the right person to validate the system...
- I may add/remove features, change dictionaries and/or change the fingers layout and the keyboard design to improve the keyboard.
- If I change the dictionary or the design you may need to relearn some things (maybe a lots of things for major dictionary changes)
- The build of the keyboard is easy but very very long
- At the moment there is no real proof that Jackdaw will allow me to code faster. I have some intuitions but I need to really practice to find the limits or other problems. My last typing test was 145 wpm but it was on a 100 common words test and I need to do measurements on real text and C++ code, I can't do that now because I still don't know to type properly with this...
- So to really learn Jackdaw I'm working on an application called [KaladriusTrainer](https://github.com/FromtonRouge/KaladriusTrainer)

# How to build this keyboard ?

To build the Kaladrius you have to :

- 3D print 6 different parts. If you don't have a 3D printer you can print them via an online 3D print service (like https://www.3dhubs.com for example) but it's very expensive.
- Do the electronic by yourself. (I'll try to design later a small pcb)

## 3D Printed parts

You can find the stl files to print [here](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls).

There are 4 files but you'll need to print 6 objects :

1) Left Case : use the file [LeftCase.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/LeftCase.stl)
2) Right Case : use the file [LeftCase.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/LeftCase.stl) but in your slicer mirror the object along the X-Axis
3) Link Case : use the file [Link.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/Link.stl)
4) Link Plate : use the file [LinkPlate.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/LinkPlate.stl)
5) Left Plate : use the file [RightPlate.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/RightPlate.stl) but in your slicer mirror the object along the X-Axis. Print this object with 2 perimeters
6) Right Plate : use the file [RightPlate.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/RightPlate.stl) Print this object with 2 perimeters

For all files an infill of 20% is enough. About perimeters, except for 5) and 6), print all objects with 3 perimeters

## Electronic

You'll need : 

- [1x Teensy 3.2 with pins](https://www.pjrc.com/store/teensy32_pins.html)
- [1x (4cm x 6cm) prototype pcb](https://www.amazon.com/ELEGOO-Prototype-Soldering-Compatible-Arduino/dp/B072Z7Y19F)
- 1x 5mm Common Cathode RGB led
- 3x 220 Ohms resistors (for the RGB led)
- 70x 1N4148 Commutation diodes
- 70x Gateron Clear Keyswitch - Plate Mount (35g)
- Electrical Wires 28 AWG

Tools :

- 1 Hot Glue gun + Glue Sticks

### Teensy 3.2 soldering

Put the Teensy on the 4cm x 6cm pcb like the image below :
![Teensy](https://i.imgur.com/ModAFe9.jpg)

Here the left side of the Teensy is on the 4th column of the pcb

Solder the Teensy and cut the legs :

![Legs](https://i.imgur.com/1XaKx59.jpg)

Here is the schematic of a RGB Led (Common Cathod) :
![RGB](https://i.imgur.com/EY7nPWp.png)

Put the led on the 7th column like this :

![RGB on pcb](https://i.imgur.com/W5KEs76.png)

Another view : 

![RGB lateral view](https://i.imgur.com/HGEKV9w.png)

Now insert the cathod in the 4th column of the pcb like this :

![Cathod](https://i.imgur.com/CYTEYB8.png)

and push the led down until the top of the led is under 12mm. If the led is too high you won't be able to put the Link Plate on the Link part :

![RGB height](https://i.imgur.com/yHM8WzK.png)

Solder the RGB led on the pcb after that it's time to put the 220 Ohm resistors like this :

![RGB led soldered to Teensy](https://i.imgur.com/u2v6jMu.png)

- 1 resistor is soldered between the Pin 13 of the Teensy and the red leg of the led
- 1 resistor is soldered between the Pin 14 of the Teensy and the green leg of the led
- 1 resistor is soldered between the Pin 15 of the Teensy and the blue leg of the led

As a memo here is the Teensy 3.2 pinout :
![Teensy 3.2 pinout](https://i.imgur.com/R1SdE5f.png)

Now link the Teensy GND pin and the Led cathod with a wire like this :

![GND to Led](https://i.imgur.com/daBkKMe.png)

### Switches soldering

TODO

# Firmware

By default the keyboard is in full NKRO, you can test the keyboard here http://random.xem.us/rollover.html

- Keyboard Maintainer: [FromtonRouge](https://github.com/FromtonRouge)
- Hardware Supported: Teensy 3.2

Make example for this keyboard (after setting up your build environment):

    make handwired/kaladrius:[your_layout]

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

