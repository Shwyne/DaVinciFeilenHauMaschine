DaVinci Feilenhaumaschine Code
==============
This is a program to run an electrified version of another invention of the famous engineer and artist Leonardo da Vinci. This is about a machine to manufacture files with a chisel and a clever way of moving the file (aka flat piece of iron in the beginning) along while creating the grooves with the chisel.

Link to a model of the machine (original, so not electrified): https://collection.sciencemuseumgroup.org.uk/objects/co46711/file-cutting-machine-from-design-of-leonardo-da-vinci-model

Dependencies
==============

    <Servo.h> by Michael Margolis
    <Accelstepper.h> by Waspinator


Hardware included:
===
- MP6550 DC-Motor Driver (2x) + V-Tec 19 RPM Gear DC-Motor (2x)
- DRV8825 Stepper Driver + NEMA 17 Stepper-Motor (SM-17HS4023)
- Servo MG996R (2x)
- Hallswitch Sensor A3144 (2x)
- Generic Switches (Used in CNC and such as endstops) (4x)
- Arduino Mega 2560 Rev3
- RGB Button
- Transistors, Resistors, Diodes, ...
