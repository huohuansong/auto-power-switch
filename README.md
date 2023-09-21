# Background

When we test some small embedded devices, such as Router, Set-top box or small Switch, etc., sometimes it is necessary to perform periodic or configurable power-on and power-off operations on these devices in order to accelerate the triggering of some probabilistic failures.
The more formal name for this auxiliary device is called power supply sequencer.
Auto-power-switch is a simple, miniature, inexpensive, low-voltage power supply sequencer.

# Introduction

It is usually possible to use the Microcontroller Unit (MCU) IO and the necessary peripheral circuits to control the power supply.
This project uses an 8-bit MCU model [STC8G1K17A](http://www.stcmcudata.com/STC8F-DATASHEET/STC8G-EN.pdf), and designs several simple command line interfaces (CLI) to control the high and low level of IO.
Auto-power-switch provides a serial port connection to the computer, where we control the MCU through the operating commands provided by the CLI.
Further, considering that some computers do not have an obvious serial port, this project also provides a USB-to-serial port instead of the native serial port.

# How to compile

This project uses the [SDCC](https://sdcc.sourceforge.net/) compilation tool.
The following is the reference compilation procedure in Ubuntu 18.04 environment.

**Step 1. Install necessary software and tools**

`sudo apt install make git sdcc`

**Step 2. Get the project code**

Suppose we use the `work` directory of the current user directory as the working directory.

`mkdir -p ~/work`

`cd ~/work`

`git clone https://github.com/huohuansong/auto-power-switch.git`

**Step 3. Compile**

`cd auto-power-switch/src/`

`make`

After successful compilation, the output file is `fw_***.hex`, and compile temporary files are in the `build` directory.

# How to use

See the [document](docs/user_manual/user_manual.md) for details.

# Known problem
