# DaVinci Feilenhaumaschine Code

This is a program to run an electrified version of another invention of the famous engineer and artist Leonardo da Vinci. This is about a machine to manufacture files with a chisel and a clever way of moving the file (aka flat piece of iron in the beginning) along while creating the grooves with the chisel.

[Link to a model of the machine](https://collection.sciencemuseumgroup.org.uk/objects/co46711/file-cutting-machine-from-design-of-leonardo-da-vinci-model) (original, so not electrified) 

## Table of Contents
- [DaVinci Feilenhaumaschine Code](#davinci-feilenhaumaschine-code)
  - [Table of Contents](#table-of-contents)
  - [Dependencies](#dependencies)
  - [Included Hardware](#included-hardware)
  - [Getting Started](#getting-started)
  - [Usage](#usage)
  - [Functions in main.cpp](#functions-in-maincpp)
  - [License](#license)

## Dependencies

    <Servo.h> by Michael Margolis
    <Accelstepper.h> by Waspinator

## Included Hardware
- Driving the shafts:
  - 2x Modelcraft IG320516-4AC21R ([Datasheet](https://asset.conrad.com/media10/add/160267/c1/-/en/000234348DS01/datablad-234348-modelcraft-ig320516-41c01-vaexelmotor-12-v-5161.pdf))
  - 2x MP6550 DC-Motor Driver ([Product page](https://www.pololu.com/product/4733))
- Driving the sign:
  - SM-17HS4023 Stepper Motor ([Datasheet](https://datasheetspdf.com/datasheet-pdf/1328258/SM-17HS4023.html))
  - DRV8825 Stepper Driver ([Product Page](https://www.pololu.com/product/2133))
- Driving Coupling and Hammerstop:
  - 2x MG996R Servo Motor ([Product Page](https://www.waveshare.com/mg996r-servo.htm))
- Detecting rotational Positions:
  - 2x Hallswitch A3144 ([Datasheet](https://www.elecrow.com/download/A3141-2-3-4-Datasheet.pdf))
- Detecting Endpositons (linear):
  - 2x Reference/Limit Switch with Roller ([e.g. this one](https://cnc-plus.de/en/CNC-Electronics/Switches---Relays/Reference-Switch-and-Limit-Switch-with-roller---HIGHLY-VT16061C-.html))
- Microcontroller:
  - e.g. Arduino Mega 2560 Rev3 ([documentation](https://docs.arduino.cc/hardware/mega-2560/))
- User Input:
  - RGB-Button ([e.g. this one](https://metzler-electronics.com/metzler-push-button-momentary-19mm-led-circular-illumination-rgb-ip67-ik10-stainless-steel-flat-soldering-contacts))
- Other parts for setting the circuit up:
  - some Resistors (1 k&ohm; ... 1M&ohm;)
  - some Transistors (5 V and 10 V - max. 2A)
  - Connectors
  - Soldering Board for extra components
  - Soldering Shield for Arduino
  - If u have to buy these components have a look [here](https://www.reddit.com/r/PrintedCircuitBoard/wiki/starter/).

## Getting Started

The Program was written in [VSCode](https://code.visualstudio.com/) with several Extensions:
| Extension | Purpose | Comment |
| ------ | ------- | ------- |
| [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)  | Compiler for C/C++ | needed |
| [Platformio IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) | Compiler for Arduino | needed |
| [Wokwi](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode) | Simulator for Arduino for Prototyping | nice to have |
| [Better Comments](https://marketplace.visualstudio.com/items?itemName=aaron-bond.better-comments) | Gives more options and visuals for comments | nice to have |
| [Markdown AIO](https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one) | Helps creating Documentarty | nice to have |
| [Markdown Preview Enhanced](https://marketplace.visualstudio.com/items?itemName=shd101wyy.markdown-preview-enhanced) | Preview of Documentary | nice to have

After you've installed the VSCode extensions click on `Clone Git Repository...` in the `Welcome page`. If there is no welcome window, just go under `File > New Window` and in the new window there will be the welcome page.

Then paste the [github-URL](https://github.com/Shwyne/DaVinciFeilenHauMaschine.git) of this repository into the `Text field` that just opened at the top. After pressing `Enter` and choosing a `directory` for the project on your local machine, you are ready to compile the project. For further details visit the [documentation](https://docs.platformio.org/en/stable/integration/ide/vscode.html#platformio-toolbar) of the Platformio IDE. 

## Usage

The configuration for the machine (geometry, settings, timings, technical data of compoents, etc.) can be found in the `config.hpp`, which is located in `libs\config` subfolder. The pins can be configured in the `pins.hpp`, which is in the same folder as config.hpp. Also if you have to change enumators (for the endstops or LED-Light settings), you can find a `Enumerator.hpp` there as well.

Everything for the machine to function is written in the `main.cpp`.

Everything regarding functions of the components is either found in the public libraries used `Accelstepper.h` `Servo.h`, under `libs\drivers` or under `libs\sensors`.

Exception-Handling is defined in `libs\ExceptHandling` and for Testing Purposes (Components or Machine Functions) there is a folder `libs\TestFunctions`.

## Functions in main.cpp
| Return-Type | Function | Input-Parameter | Description |
| --- | --- | --- | --- |
| void | `setup()` | none | Initializes the machine |
| void | `loop()` | none | Main Loop of the machine (Idle -> Run -> Reset -> Idle -> ...) |
| void | `initStateOfMachine()` | none | Initializes the state of the machine (after start or clearing an error)
| void | `checkState()` | StatusClass status | Checks the state of the machine `status` and, if status != SUCCESS, print error and endless error-loop |
| void | `step::home()` | none | Homes the Stepper (Sign pos 1) |
| void | `serv::hammerstop()` | none | Activates the Hammerstop |
| void | `serv::hammergo()` | none | Deactivates the Hammerstop |
## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.