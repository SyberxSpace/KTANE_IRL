---
title: Module - Simon Says
date: 2021-06-04 12:00:00 -0500
categories: [Dev Logs, Modules]
tags: [timeline, simon-says, module]
toc: true
---

## Introduction

Simon Says was one of the first modules I planned when starting this project.

In theory, it's a simple module. 4 buttons with lights and a beeper. Over a year later I realize I didn't account for how difficult that could be and how much I would need to learn in the process, but the time was well-spent.

## Bill of Materials

| **Item** | **Quantity** | **Source** |
|:--|--:|:--|
| ESP32 Module | 1 | [Amazon](https://smile.amazon.com/gp/product/B07QCP2451/) |
| 44mm Arcade Button (R,B,G,Y) | 4 | [Amazon](https://smile.amazon.com/gp/product/B07WQTFV26/) |
| 1.5" 4Ω Speaker | 1 | [Amazon](https://smile.amazon.com/gp/product/B01LN8ONG4/) |
| PAM8403 Amplifier | 1 | [Amazon](https://smile.amazon.com/gp/product/B00LODGV64/) |
| TJA1050 CAN Bus Transceiver | 1 | [Amazon](https://smile.amazon.com/gp/product/B07W4VZ2F2/) |
| Logic Level Shifter | 1 | [Amazon](https://smile.amazon.com/dp/B07LG646VS/) |
| Short M3 Heat Insert | 12 | [Ebay](https://www.ebay.com/itm/292175924519) |
| 16mm M3 Standoff | 11 | [Amazon](https://smile.amazon.com/gp/product/B07H3QY54G/) |
| 60×10×3mm Magnet | 4 | [Amazon](https://smile.amazon.com/gp/product/B078KTLWQ9/) |
| Single WS2812B LED | 1 | [Amazon](https://smile.amazon.com/gp/product/B01DC0J0WS/) |


## Printed Parts

The primary faceplate and electronics supports were printed using Fusion Filaments Sievert Silver HTPLA+.
The dark section of the faceplate and backing rim were printed using Prusament Galaxy Black PLA.

## Development Logs

#### *05/11/2020*
Obtained some standard 12v 33mm square arcade buttons for testing. An initial test was promising, but after printing a scaled mount I realized they might be a bit small for the 15cm planned module size.

![5/11/2020-01](/assets/img/resources/posts/simonsayslog/2020-05-11-0001.jpg){: height="400"}
![5/11/2020-02](/assets/img/resources/posts/simonsayslog/2020-05-11-0002.jpg){: height="400"}

#### *05/12/2020*
After some frame counting and investigation, a I was able to generate the first draft of what would become the timing table used for programming. Most of this was correct, but some details like frequency would later be revised.

![5/12/2020-01](/assets/img/resources/posts/simonsayslog/2020-05-12-0001.jpg){: width="600"}

#### *07/14/2020*
After more investigation and a bit of searching through game files, I created the first proper module breakdown on github, a timing and behavior document for Simon Says. [Repo Link](https://github.com/SyberxSpace/KTANE_IRL/tree/master/Modules/01_Vanilla/04_Simon_Says)

#### *07/17/2020*
Finally got in an order of uncommonly large 44mm square arcade buttons from AliExpress! Sadly they messed up and I got two white buttons and no blue buttons...

![7/17/2020-01](/assets/img/resources/posts/simonsayslog/2020-07-17-0001.jpg){: height="400"}

#### *10/19/2020*
Designed and printed a module for the 33mm buttons at proper scale. Definitely looks janky. 44mm buttons it is! Luckily I found some (overpriced) on Amazon I can get a bit faster.

![10/19/2020-01](/assets/img/resources/posts/simonsayslog/2020-10-19-0001.jpg){: height="400"}

#### *10/22/2020*
Much better. The gap looks a bit odd though, so I need to figure out something there.

![10/22/2020-01](/assets/img/resources/posts/simonsayslog/2020-10-22-0001.jpg){: height="400"}

#### *10/23/2020*
Stuck some black paper under the buttons and it cleans up the look a lot. I'll definitely want to have a dark backing between the gaps to make the buttons look a little more like a single unit. I've also swapped the nicer switches from the 33mm buttons into the cheap ones that came with the 44mm ones. They have a much nicer click now.

![10/23/2020-01](/assets/img/resources/posts/simonsayslog/2020-10-23-0001.jpg){: height="400"}

#### *05/09/2021*
First test-fit with the new docking system (though not a new module design). Looking promising!

![5/9/2020-01](/assets/img/resources/posts/simonsayslog/2021-05-09-0001.jpg){: height="400"}

#### *05/17/2021*
Thanks to `@bcorwen`'s demo code I've gotten [XTronical's ESP32 DAC audio library](https://www.xtronical.com/the-dacaudio-library-download-and-installation/) running with a custom sine-wave instrument. It can't quite replicate the sounds perfectly, but it's more than good enough for now.

{% youtube pt-YMAyQspw %}

#### *05/18/2021*
And with audio out of the way I've been able to finally confirm a wiring diagram (more or less).

![5/18/2020-01](/assets/img/resources/posts/simonsayslog/2021-05-18-0001.png){: height="400"}

#### *05/28/2021*
Created the circuit on perfboards and soldered it all together. Only one board lost in the process (forgot the CAN module and had to re-start it) so not bad. Fitting all the parts inside the footprint will be a challenge however.

![5/28/2020-01](/assets/img/resources/posts/simonsayslog/2021-05-28-0001.jpg){: height="400"}
![5/28/2020-02](/assets/img/resources/posts/simonsayslog/2021-05-28-0002.jpg){: height="400"}

#### *05/31/2021*
Spent most of the day programming and finally got Simon Says coded and working! First time using enum and maybe third time using struct in a program, so big thanks to `@PurkkaKoodari (peeloS)`!

{% youtube vaSswVuD3_A %}

#### *06/01/2021*
Worked on the mounting and improvements to the CAD for the final print. I decided to mount the boards vertically between/around the buttons to get everything compact, so the parts got fairly complex pretty quick.

![6/1/2020-01](/assets/img/resources/posts/simonsayslog/2021-06-01-0001.png){: height="400"}

#### *06/02/2021*
Test fits of the two first prints failed (I had a board sideways and forgot about the tolerance around the button nuts) so reprints are needed. The rest of the CAD was finished, including the weird stilted platform for the CAN connector, so another around of printing is in order.

![6/2/2020-01](/assets/img/resources/posts/simonsayslog/2021-06-02-0001.png){: height="400"}
![6/2/2020-02](/assets/img/resources/posts/simonsayslog/2021-06-02-0002.png){: height="400"}

#### *06/03/2021*
**Morning** - Prints came out well and fit fine. Faceplate is the last part. I've used FusionFilaments silver like the rest of the prints for the main color, and Prusament galaxy black for the dark insert. 12 color changes, so not bad. Should be done by around 4pm.

![6/3/2020-01](/assets/img/resources/posts/simonsayslog/2021-06-03-0001.png){: height="400"}

**Afternoon** - And with that print complete assembly can occur! Wiring was a bit of a challenge but only required lengthening the speaker wires in the end. Glad I added zip-tie points to the model, but also blocked one by accident (need to be more careful in future). Now that it's in the frame, the scale is really starting to show!

![6/3/2020-02](/assets/img/resources/posts/simonsayslog/2021-06-03-0002.jpg){: height="400"}
![6/3/2020-03](/assets/img/resources/posts/simonsayslog/2021-06-03-0003.jpg){: height="400"}
![SimonImage](/assets/img/resources/posts/simonsayslog/SimonImage.jpg){: height="400"}
![6/3/2020-04](/assets/img/resources/posts/simonsayslog/2021-06-03-0004.jpg){: height="400"}