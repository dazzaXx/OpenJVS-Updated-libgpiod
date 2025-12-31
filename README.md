# ModernJVS

ModernJVS is fork of OpenJVS, an emulator for I/O boards in arcade machines that use the JVS protocol. It requires a USB RS485 converter, or an official OpenJVS HAT.

Updated to use libgpiod, with backwards-compatible support for the now deprecated sysfs. Optimized code and new features, as well as support to the Raspberry Pi 5.

Updated using Github Copilot.

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
git clone https://github.com/dazzaXx/ModernJVS
make
sudo make install
```

If using DietPi:

```
sudo apt install build-essential cmake git file libgpiod-dev pkg-config
git clone https://github.com/dazzaXx/ModernJVS
make
sudo make install
```

## Supported Hardware

ModernJVS supports all Raspberry Pi models including:
- Raspberry Pi 1, 2, 3, 4
- Raspberry Pi 5 (with automatic GPIO chip detection)

The software automatically detects the correct GPIO chip for your Raspberry Pi model.

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

## Enabling Bluetooth and UART Simultaneously on Pi 4/5

Raspberry Pi 4 and 5 models have multiple UART interfaces, which allows you to use Bluetooth and hardware UARTs at the same time. This is useful if you want to:
- Use Bluetooth controllers or peripherals while running ModernJVS
- Use a hardware UART for RS485 communication instead of a USB adapter

### Understanding Pi 4/5 UARTs

By default on Raspberry Pi 4 and 5:
- **Primary UART (PL011)**: `/dev/ttyAMA0` - Used by Bluetooth by default
- **Mini UART**: `/dev/ttyS0` - Available for GPIO serial (less reliable, clock-dependent)
- **Additional UARTs**: Can be enabled via device tree overlays (UART2-5)

### Configuration Options

#### Option 1: Using USB RS485 Adapter (Recommended)

If you're using a USB to RS485 converter (the standard setup), **no additional configuration is needed**. The USB adapter appears as `/dev/ttyUSB0` and doesn't conflict with Bluetooth. This is the most common configuration for ModernJVS.

The sense line uses GPIO pins (e.g., GPIO 12), which are completely independent of UART and Bluetooth.

#### Option 2: Using Hardware UART for RS485

If you want to use a hardware UART instead of USB RS485, you'll need to enable an additional UART to keep Bluetooth working.

**Method 2a: Enable UART2 on GPIO pins (Recommended for Pi 4/5)**

Edit `/boot/firmware/config.txt` (or `/boot/config.txt` on older systems) and add:

```ini
# Enable UART
enable_uart=1

# Enable UART2 on GPIO pins 0 and 1
dtoverlay=uart2
```

After rebooting, you'll have:
- Bluetooth still works on `/dev/ttyAMA0`
- UART2 available on `/dev/ttyAMA1` (GPIO 0 = TX, GPIO 1 = RX)

Update your ModernJVS config to use the new UART:
```
DEVICE_PATH /dev/ttyAMA1
```

**Method 2b: Swap Bluetooth to Mini UART (Alternative)**

This method moves Bluetooth to the mini UART, freeing up the primary UART for your RS485 connection.

Edit `/boot/firmware/config.txt` (or `/boot/config.txt` on older systems) and add:

```ini
# Enable UART
enable_uart=1

# Move Bluetooth to mini UART
dtoverlay=miniuart-bt
```

After rebooting:
- Bluetooth uses `/dev/ttyS0` (mini UART)
- Primary UART `/dev/ttyAMA0` is available for RS485

Update your ModernJVS config:
```
DEVICE_PATH /dev/ttyAMA0
```

> **Note**: The mini UART is less reliable for high-speed communication as it's tied to the VPU clock. Bluetooth may have slightly reduced performance, but should work fine for most applications.

**Method 2c: Enable Additional UARTs (Advanced)**

Pi 4 and 5 support up to UART5. You can enable multiple UARTs:

```ini
enable_uart=1
dtoverlay=uart2  # Creates /dev/ttyAMA1 on GPIO 0,1
dtoverlay=uart3  # Creates /dev/ttyAMA2 on GPIO 4,5
dtoverlay=uart4  # Creates /dev/ttyAMA3 on GPIO 8,9
dtoverlay=uart5  # Creates /dev/ttyAMA4 on GPIO 12,13
```

Refer to the [Raspberry Pi documentation](https://www.raspberrypi.com/documentation/computers/configuration.html#configure-uarts) for pin assignments.

### Verifying Your Configuration

After making changes and rebooting, verify available serial devices:

```bash
ls -l /dev/ttyAMA* /dev/ttyS* /dev/ttyUSB*
```

Check that Bluetooth is working:

```bash
systemctl status bluetooth
hcitool dev
```

### Troubleshooting

- If Bluetooth stops working after enabling additional UARTs, make sure you didn't disable it with `dtoverlay=disable-bt`
- If you can't find `/boot/firmware/config.txt`, try `/boot/config.txt` (location varies by OS version)
- Always reboot after changing `/boot/firmware/config.txt` or `/boot/config.txt`
- **Important for sense line users**: The `uart5` overlay uses GPIO 12 and 13, which conflicts with the default sense line pin (GPIO 12). If you're using the sense line, avoid enabling `uart5` or change your sense line pin in the ModernJVS config to a different GPIO pin

## Configuration

ModernJVS can be configured using the following command:
```
sudo nano /etc/modernjvs/config
```
Check the /etc/modernjvs/ios folder to see which I/O boards can be emulated and input the name of it on the EMULATE line.

By default it will emulate the Namco FCA1, as well as the debug level set to 1.


## Controller Deadzone Support

With version 4.6.2, configurable deadzone can now be changed in the config for each players controller. Only affects controllers with analog sticks as it's main use is to eliminate stick drift.

