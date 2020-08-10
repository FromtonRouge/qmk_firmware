# Kaladrius Steno Keyboard

![Kaladrius](https://i.imgur.com/UF7BSkB.jpg)

The Kaladrius is an orthographic steno keyboard that uses a modified [Jackdaw steno theory](https://sites.google.com/site/learnplover/jackdaw) created by **Jennifer Brien**.
Dictionaries are stored in the firmware and Plover is not needed.
The main goal for me is to be able to code with a chorded keyboard. It's still a prototype and hope to use it daily this year (2020) at work.
It's a handwired and printable keyboard and use a Teensy 3.2

# Why this keyboard ?

I'm a C++ Tools programmer in the video game industry and I always wanted to improve my typing speed. I changed several times my keyboard layout in my career :
My original layout was Azerty (with a top speed at 86 wpm on 10fastfingers), in 2014 I changed to Dvorak (with a top speed at 98 wpm) and in 2016 I finally changed to Colemak (top speed at 103 wpm).

I discovered Plover and Steno in 2016 too and was blown away by this [presentation](https://www.youtube.com/watch?v=Wpv-Qb-dB6g)

I decided to learn Plover but found that it might not suitable for C++ because of the phonetic/mnemonic approach.
That's why I got interested in Orthographic Steno Systems and found the "Jackdaw steno theory" on the [Learn Plover site](https://sites.google.com/site/learnplover/jackdaw)

I first implemented an ErgoDox Layout for Jackdaw and learned to type basics words on 10fastfingers.com. After a lot of iterations on the firmware I started to practice more but on April 2018 I injured my left hand after a too long training session.
Actually I got a De Quervain tendinitis because of the very bad ErgoDox thumb positions. Note: at this time my top speed on the 200 words test was 140 wpm with Jackdaw.

I had a surgery in October 2018...
![Surgery](https://i.imgur.com/xZ3XdZM.jpg)
(just after the surgery)

![Scar](https://i.imgur.com/OIYHb7z.jpg)
(I won a scar)

**So I highly don't recommend the ErgoDox or similar keyboards for Jackdaw. I'll delete the ErgoDox firmware soon!**

After the surgery I decided to build a specific keyboard for Jackdaw so I can practice safely (I DON'T GIVE UP!). The keyboard needs the following :

- Thumbs keys must be closer to the index column to avoid bad wrists movements
- Thumbs layout must be optimized for Jackdaw
- Because of the 2 previous points the keyboard must be 3D Printed and the design must be parametric (that's why I chose OpenSCAD for the conception)
- It's possible to switch to a "normal" keyboard layout
- The keyboard is easy to build even if it takes time. That's why it's not really a splitted keyboard
- The development board must have enough memory for several dictionaries (that's why I chose the Teensy 3.2)

I named it "Kaladrius" because the Keyboard looks like a white bird and in Roman mythology the Caladrius is a white bird that can heal someone...

# Before continuing...

If you are interested by using Jackdaw and the Kaladrius here are some things to know before continuing :

- First it's an orthographic system, it's slower than steno (but faster than touch typing)
- It appears that by learning Jackdaw you won't loose your touch typing speed
- It's only for the english language but...
- ... english is not my native language I may not be the right person to validate the system...
- I made some modifications to the Jackdaw theory and added/removed some features. I'll list them later
- I may add/remove features, change dictionaries and/or change the fingers layout and the keyboard design to improve the keyboard.
- If I change the dictionary or the design you may need to relearn some things (maybe a lots of things for major dictionary changes)
- The build of the keyboard takes time...
- At the moment there is no real proof that Jackdaw will allow me to code faster. I have some intuitions but I need to really practice to find the limits or other problems. My last typing test was 145 wpm but it was on a 200 common words test and I need to do measurements on real text and C++ code, I can't do that now because I still don't know to type properly with this...
- **So to really learn to type with this keyboard I'm working on an application called [KaladriusTrainer](https://github.com/FromtonRouge/KaladriusTrainer)**

![KaladriusTrainer](https://i.imgur.com/YECGVEx.png)

# How to build this keyboard ?

To build the Kaladrius you have to :

- 3D print 6 different parts. If you don't have a 3D printer you can print them via an online 3D print service (like https://www.3dhubs.com for example) but it's very expensive.
- Do the electronic by yourself...

## Parts

You'll need : 

- [1x Teensy 3.2 with pins](https://www.pjrc.com/store/teensy32_pins.html)
- [1x (4cm x 6cm) prototype pcb](https://www.amazon.com/ELEGOO-Prototype-Soldering-Compatible-Arduino/dp/B072Z7Y19F)
- 1x 5mm Common Cathode RGB led
- 3x 220 Ohms resistors (for the RGB led)
- 72x 1N4148 Commutation diodes
- 72x Gateron Clear Keyswitch - Plate Mount (35g)
- 64x [1 space G20 keycaps](https://pimpmykeyboard.com/g20-1-space-pack-of-10)
- 4x [1.5 space G20 keycaps)](https://pimpmykeyboard.com/g20-1-5-pack-of-4)
- 4x [2 space G20 keycaps](https://pimpmykeyboard.com/g20-2-space-pack-of-4)
- Electrical Wires 28 AWG
- 26x M3 nuts
- 18x M3 screws of 12mm
- 8x M3 screws of 10mm
- 4x M2 screws of 6 mm
- 4x M2 rings
- 4x M2 nuts
- 1 micro usb magnetic cable

Tools :

- Soldering iron
- Solder wire
- Electronic tweezers
- Electronic third hand
- 1 Hot Glue gun + Glue Sticks
- 1 mini electric screw driver (very useful for this build)
- A marker
- A ruler

## 3D Printed parts

You can find the stl files to print [here](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls).

There are 4 files but you'll need to print 6 objects :

1) Left Case : use the file [LeftCase.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/LeftCase.stl)
2) Right Case : use the file [LeftCase.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/LeftCase.stl) but in your slicer mirror the object along the X-Axis
3) Link Case : use the file [Link.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/Link.stl)
4) Link Plate : use the file [LinkPlate.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/LinkPlate.stl)
5) Left Plate : use the file [RightPlate.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/RightPlate.stl) but in your slicer mirror the object along the X-Axis. Print this object with 2 perimeters
6) Right Plate : use the file [RightPlate.stl](https://github.com/FromtonRouge/qmk_firmware/tree/master/keyboards/handwired/kaladrius/design/Stls/RightPlate.stl) Print this object with 2 perimeters

For all files an infill of 20% is enough. About perimeters, except for 5) and 6), print all objects with 3 perimeters.
For the right and left plates I personally print them on a glass surface.

About print times, it depends on your printer and print settings. For my settings I have the following :

- Left Plate : 2h59
- Right Plate : 2h59
- Left Case : 7h01
- Right Case : 7h01
- Link : 7h25
- Link Plate : 58 minutes

After this step you can assemble the case without the plates :

You need :

- 8 M3 screws of 10 mm (8 and 12mm also works)
- 8 M3 nuts
- (optional) 1 M3 ring

Try to put a M3 nut in this slot on the case :

![Nut in the slot](https://i.imgur.com/OvC76Wg.jpg)

If you can't push the nut by hand use a M3 screw with a ring and use it to insert the nut :

![Screw with ring](https://i.imgur.com/QkL1VnF.jpg)

Screw until the nut is fully inserted
![Insert with screw](https://i.imgur.com/Os5aCzP.jpg)

Once the nut is fully inserted remove the screw, at the end you should have something like this :

![Nuts inserted](https://i.imgur.com/mios9k6.jpg)

![All nuts inserted](https://i.imgur.com/SjaDlf2.jpg)

## Teensy 3.2 soldering

Put the Teensy on the 4cm x 6cm pcb like the image below :
![Teensy](https://i.imgur.com/ybeFHPk.png)

Here the left side of the Teensy is on the 4th column of the pcb

Solder the Teensy and cut the legs :

![Cut the legs](https://i.imgur.com/1XaKx59.jpg)

Here is the schematic of a RGB Led (Common Cathod) :

![RGB](https://i.imgur.com/EY7nPWp.png)

Put the led on the 7th column like this :

![RGB on pcb](https://i.imgur.com/W5KEs76.png)

Another view : 

![RGB lateral view](https://i.imgur.com/HGEKV9w.png)

Now insert the cathod in the 4th column of the pcb like this :

![Cathod](https://i.imgur.com/CYTEYB8.png)

and push the led down until the top of the led is <= 13mm. If the led is too high you won't be able to put the Link Plate on the Link part :

![RGB height](https://i.imgur.com/QFUjTAA.png)

Solder the RGB led on the pcb, after that it's time to put the 220 Ohm resistors like this :

![RGB led soldered to Teensy](https://i.imgur.com/u2v6jMu.png)

- 1 resistor is soldered between the Pin 13 of the Teensy and the red leg of the led
- 1 resistor is soldered between the Pin 14 of the Teensy and the green leg of the led
- 1 resistor is soldered between the Pin 15 of the Teensy and the blue leg of the led

As a memo here is the Teensy 3.2 pinout :

![Teensy 3.2 pinout](https://i.imgur.com/R1SdE5f.png)

Now link the Teensy GND pin and the Led cathod with a wire like this :

![GND to Led](https://i.imgur.com/5hwMAk3.png)

Now you can do a basic test :

- Compile and flash the Kaladrius firmware on the Teensy
- After the reboot you should see the RGB led blinking like in this video : [Kaladrius boot blinking test](https://youtu.be/ky0wmrFHCOY)

## Switches

Puth the switches on the plate :

![Switches on plate](https://i.imgur.com/NWSrIQq.jpg)

To be sure that they won't pop out glue them with a Hot Glue Gun :

![Hot glue gun](https://i.imgur.com/BxH20qP.jpg)

![Switches glued](https://i.imgur.com/KcKHWaS.jpg)

As you can see on the photo you can glue just one side, it's enough.
**If you have to remove the switches from the plate you can use a Heat Gun**

Here is the result when it's done :

![All switches](https://i.imgur.com/s6p6Aqh.jpg)

## Soldering the rows (diodes)

As an example we are working on the Right Plate here.

For the diodes notice the black line on them :

![Diode](https://i.imgur.com/znROXWL.png)

Put the diode on the switch with the black line oriented like this :

![Diode on Switch](https://i.imgur.com/LW8NQsJ.jpg)

Solder it then bend the leg at 90 degrees, do the same steps with another diode and solder them together like this :

![Diodes](https://i.imgur.com/0SJblpn.jpg)

Pay attention to the switch that is oriented at 90 degrees :

![Switch at 90](https://i.imgur.com/2HXYQFO.png)

Continue until you reach the thumb cluster, I solder them like the following photos (always pay attention to the diode's black line):

![Thumb 1](https://i.imgur.com/HtfrcPo.jpg)
![Thumb 2](https://i.imgur.com/qMZVlOw.jpg)
![Thumb 3](https://i.imgur.com/mmnyIIg.jpg)

And finally cut the legs :

![Thumb 4](https://i.imgur.com/Matmlyz.jpg)

The result :

![Result](https://i.imgur.com/M9vMjZZ.jpg)

That was the Right Plate, do the same withe Left Plate :

![Left Plate thumb closeup](https://i.imgur.com/CcFFiGm.jpg)

![Left Plate](https://i.imgur.com/Ld75Rol.jpg)

## Soldering the columns

Here we'll work with the Right Plate.

Put marks on the wire at pin locations :

![Marks on wire](https://i.imgur.com/IiwuTTP.jpg)

Then with a cutter or a scalpel make 3 incisions along the white lines on the photo (don't cut completely the wire). You'll be able to remove a portion of the wire sheath :

![Remove sheath](https://i.imgur.com/WNOdzX7.png)

You can now solder the wire on the pins :

![Solder on pins](https://i.imgur.com/zY1d8zs.jpg)

Don't cut the portion marked on the photo, we will need this after :

![Don't cut](https://i.imgur.com/tG4cSaV.png)

Do the same thing with other columns, as a helper use also different wire colors it will be easier to solder wires on the Teensy after. Also leave the last switch alone :

![Result](https://i.imgur.com/tAuRmg3.png)

Do the same thing with the Left Plate

## Connect columns and rows to the Teensy

Now that the columns and rows are done we can connect them to the Teensy. Here is a recap of columns/rows of the keyboard with the colors I used :

![Keyboard Matrix](https://i.imgur.com/8CxN5bD.png)

Same for the Teensy :

![Teensy matrix](https://i.imgur.com/drTIG04.png)

Use wires with the following lengths and solder them on the Teensy first **At the moment don't solder wires on the plate**:

- Col 0 and 15 (red) : 25 cm
- Col 1 and 14 (red) : 23 cm
- Col 2 and 13 (red) : 21 cm
- Col 3 and 12 (yellow) : 20 cm
- Col 4 and 11 (yellow) : 19 cm
- Col 5 and 10 (yellow) : 18 cm
- Col 6 and 9 (white) : 17 cm
- Col 7 and 8 (white) : 30 cm
- Row 0 (green) : 20 cm
- Row 1 (blue) : 22 cm
- Row 2 (black) : 24 cm
- Row 3 (red) : 26 cm
- Row 4 (yellow) : 28 cm

Connect and solder the wires to the Teensy :

![Wires on Teensy](https://i.imgur.com/ELaBAtf.jpg)

And here is a closer look of the back of the Teensy, I added some hot glue for the row wires that go to the right side :

![Back of the Teensy](https://i.imgur.com/C1qFQGj.jpg)

When it's done put the Teensy in the case and pass the wires to the left and right sides.

Now you can solder wires to columns and rows on the plate :

![First column wiring](https://i.imgur.com/dAK7bq2.jpg)

![All columns wired](https://i.imgur.com/pFF9Nbf.jpg)

![Columns and Rows wired](https://i.imgur.com/uhn7Y1h.jpg)

You should have something like this :

![Final result](https://i.imgur.com/S4a6mhf.jpg)

## Final assembly

You can put all plates on the case, you'll need :

- 18 M3 of 12mm screws for left and right plates
- 18 M3 nuts

![Final assembly](https://i.imgur.com/H7YYoBu.jpg)

With G20 keycags :

![With keycaps](https://i.imgur.com/UWdOisa.jpg)

# Firmware

In your keymap.c file, create a layer for the Jackdaw layout, for example :

```C
    [LAYER_JACKDAW] = LAYOUT(
        KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,     KC_F6,     KC_LGUI,      KC_TRNS,   KC_F7,     KC_F8,     KC_F9,     KC_F10,    KC_F11,    KC_F12, 
        KC_NO,     CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO,    CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, 
        CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO,    CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, 
        CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO,                          CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO, 
        CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO,                                                                      CKC_STENO, CKC_STENO, CKC_STENO, CKC_STENO,
                                                               CKC_STENO,                          CKC_STENO, 
                                         CKC_STENO, CKC_STENO, CKC_STENO,                          CKC_STENO, CKC_STENO, CKC_STENO, 
                                                               CKC_STENO,                          CKC_STENO ),
```

and add also this:

``` C
// Steno keymap
const uint32_t PROGMEM g_steno_layout[MATRIX_ROWS][MATRIX_COLS] = LAYOUT(
        0,  0,   0,   0,   0,   0,  0,             0,      0,   0,   0,   0,   0,    0, 
        0,  0,   0,  L2,  L1,  L0,  S_ENT,         SC_SEP, R0,  R1,  R2,  0,   0,    0,
        0,  0,  L3,  L_C, L_W, L_N, SC_STAR,       SC_SEP, R_R, R_L, R_C, R3,  R4,   0,
        0,  L4, L_A, L_T, L_H, L_R,                        R_N, R_G, R_H, R_T, RP_E, 0,
        0,  L5, L_S, 0,                                              0,   R_S, RP_Y, 0,
                                   SC_STAR,        SC_SEP, 
                        SC_LPLUS, T_E, T_O,        T_Y, T_I, SC_RPLUS, 
                                       T_A,        T_U );

steno_layout_t* get_steno_layout(void) { return g_steno_layout; }
```

You'll find an example in my personal keymap [here](https://github.com/FromtonRouge/qmk_firmware/blob/master/keyboards/handwired/kaladrius/keymaps/fromtonrouge/keymap.c)

By default the keyboard is in full NKRO, you can test the keyboard here http://random.xem.us/rollover.html

- Keyboard Maintainer: [FromtonRouge](https://github.com/FromtonRouge)
- Hardware Supported: Teensy 3.2

Make example for this keyboard (after setting up your build environment):

    make handwired/kaladrius:[your_layout]

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

