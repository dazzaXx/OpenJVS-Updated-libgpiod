# OpenJVS-Updated-libgpiod

OpenJVS is an emulator for I/O boards in arcade machines that use the JVS protocol. It requires a USB RS485 converter, or an official OpenJVS HAT.



##

As using sysfs is now deprecated in newer linux kernels, this software no longer worked properly on them. It has now been updated to use the new modern libgpiod library for the sense pin, with backwards-compatible support for sysfs if needed. The overall code has also been optimized. Updated with help from Github Copilot.

~dazzaXx

##

The following arcade boards are supported:

- Naomi 1/2
- Triforce
- Chihiro
- Hikaru
- Lindbergh
- Ringedge 1/2
- Namco System 22/23
- Namco System 2x6
- Taito Type X+
- Taito Type X2
- exA-Arcadia

## Installation

Installation is done from the git repository as follows, using RaspiOS Lite:

```
sudo apt install build-essential cmake git file libgpiod-dev
git clone https://github.com/dazzaXx/OpenJVS-Updated-libgpiod
make
sudo make install
```

If using DietPi:

```
sudo apt install build-essential cmake git file libgpiod-dev pkg-config
git clone https://github.com/dazzaXx/OpenJVS-Updated-libgpiod
make
sudo make install
```

On games that require a sense line, the following has to be wired up:

```

|          GND   (BLACK) |-------------| GND           |                 |                        |
| ARCADE   A+    (GREEN) |-------------| A+  RS485 USB |-----------------| USB  RASPBERRY PI > 1  |
|          B-    (WHITE) |-------------| B-            |                 |                        |
|                        |                                               |                        |
|          SENSE (RED)   |----------+------------------------------------| GPIO 12                |
                                    |
                                    +---- (1kOhm Resistor or 4 Signal Diodes) ---- GND
```

A 1KOhm resistor or 4 signal diodes are known to work properly, the purpose of these is to create a 2.5 volt drop.

> Warning: A 1KOhm resistor will not work with the Triforce system, please use the 4 signal diodes for this purpose.

When buying a USB to RS485 dongle be sure to buy one with an FTDI chip inside. The CP2102 and other chips have been found to not work well.

I have tested CH340E USB RS485 converters personally and they work. ~dazzaXx

## Configuration

OpenJVS can be configured using the following command:
```
sudo nano /etc/openjvs/config
```
Check the /etc/openjvs/ios folder to see which I/O boards can be emulated and input the name of it on the EMULATE line.

By default it will emulate the Namco FCA1, as well as the debug level set to 2.

### Analog Stick Deadzone

OpenJVS supports configurable deadzone for analog sticks to reduce sensitivity and prevent drift. This can be configured in `/etc/openjvs/config`:

```
# Analog Stick Deadzone
# Sets the deadzone for analog sticks on joystick controllers (0.0 to 1.0)
# Values within the deadzone are treated as centered (no input)
# Typical values: 0.0 (no deadzone), 0.1 (10%), 0.15 (15%), 0.2 (20%)
# This only applies to analog stick axes (X, Y, Z), not triggers, pedals, or light guns
# Default: 0.0 (no deadzone)
ANALOG_DEADZONE 0.15
```

**Important Notes:**
- The deadzone only applies to **analog stick axes** (X, Y, Z) from **joystick devices**
- It does NOT apply to:
  - Light gun inputs (even though they use analog axes)
  - Triggers or pedals (R, L, T axes)
  - Keyboard or mouse devices
- Typical deadzone values range from 0.1 (10%) to 0.2 (20%)
- Use higher values if your controller has significant drift or is too sensitive
- Values outside the deadzone are automatically scaled to use the full output range

##

Make sure to check the original guide.md in the docs folder to find out how to configure it in more detail.

