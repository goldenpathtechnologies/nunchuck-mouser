# Nunchuck Mouser

![Build status](https://github.com/goldenpathtechnologies/nunchuck-mouser/actions/workflows/build.yml/badge.svg?branch=main)
![Build status dev](https://github.com/goldenpathtechnologies/nunchuck-mouser/actions/workflows/build.yml/badge.svg?branch=dev)
[![Conventional Commits](https://img.shields.io/badge/Conventional%20Commits-1.0.0-yellow.svg)](https://conventionalcommits.org)

An Arduino project that turns a Wii Nunchuck into a multifunction mouse compatible with both right and left-handed
users.

## Prototyping

Nunchuck Mouser receives input from the Wii Nunchuck via I2C. Any Arduino-compatible board that
supports I2C (SDA and SCL pins), and outputs 3V should work with this program. Currently, only the
Adafruit Trinket M0 is supported for now.

### Materials

- Wii Nunchuck
- [Adafruit Trinket M0 dev board](https://www.adafruit.com/product/3500)
- [Nunchuck adapter](https://www.adafruit.com/product/4836)
- Breadboard
- Male-Male jumper wires x4
- Micro USB cable
- Breakaway header pins x15 (comes with Trinket M0 and nunchuck adapter)
- Rosin core solder
- Soldering iron

### Assembly

Solder the header pins to the I/O pins on the Trinket M0 and the nunchuck adapter. You can use the breadboard
to help keep the pins aligned while soldering.

Connect the Trinket M0 and the nunchuck adapter to the breadboard.

Connect the jumper wires as follows:

| Trinket M0 Pins | Nunchuck adapter pins |
|-----------------|-----------------------|
| 3V              | 3V                    |
| Gnd             | Gnd                   |
| 0               | SDA                   |
| 2               | SCL                   |

Connect the Wii Nunchuck to the nunchuck adapter and the Micro USB cable to the Trinket M0. When you're
to load the firmware onto the Trinket, plug the USB cable into your computer.

## Finished product

This is a work in progress. This documentation will be updated when the finished product is completed.

### Materials

TBD

### Assembly

TBD

## Firmware installation

Nunchuck Mouser was developed with the PlatformIO IDE platform. You will need a PlatformIO compatible IDE
to build and upload Nunchuck Mouser to the Trinket M0. Consult the PlatformIO docs on how to accomplish this.

## Usage/modes

This mouse is designed to have multiple modes that are accessible by tilting the nunchuck in different orientations.

### Quick guide

- [Mouse mode](#Mouse): default orientation
- [Scroll mode](#Scroll): side tilted in Mouse mode
- [Keyboard mode](#Keyboard): activated by tilting downward in Mouse mode and pressing C
  - [Navigation mode](#Navigation): default orientation in Keyboard mode
  - [Navigation Plus mode](#Navigation-Plus): side tilted in Keyboard mode
  - [Media mode](#Media): tilted up in Keyboard mode
  - [Exit mode](#Exit): tilted down in Keyboard mode

### Mouse

| Control      | Description                                                                       |
|--------------|-----------------------------------------------------------------------------------|
| Analog stick | Mouse movement                                                                    |
| Z Button     | Left mouse button                                                                 |
| C Button     | Right mouse button (activates Keyboard mode when the nunchuck is tilted downward) |

Mouse movement is designed to feel as natural as possible with an analog stick. Mouse speed increases as the analog
stick is extended.

### Scroll

This mode is activated from the Mouse mode by tilting the nunchuck to the left or right. Tilting the nunchuck back
to its default upright position brings it back to Mouse mode.

| Control       | Description                                                                                       |
|---------------|---------------------------------------------------------------------------------------------------|
| Analog Y axis | Scroll vertically                                                                                 |
| Analog X axis | Scroll horizontally (currently disabled, awaiting updates to HID library)                         |
| C Button      | Middle mouse button                                                                               |
| Z Button      | Opens application switching view in Windows, Linux, and macOS (mapped to Win/Super/Command + Tab) |

### Keyboard

The keyboard mode introduces a set of sub-modes that implement common keyboard keys. Activate this mode by tilting
the nunchuck downwards in Mouse mode and pressing the C Button.

This mode can not be used to type text. It is suggested to use an on-screen keyboard to type with this device
if a physical keyboard is not available.

#### Navigation

This keyboard mode implements common navigation keys and is active in the default upright position, similar
to the Mouse mode.

| Control      | Description |
|--------------|-------------|
| Analog stick | Arrow keys  |
| Z Button     | Enter       |
| C Button     | Backspace   |

#### Navigation Plus

This keyboard mode implements additional keys that are typically used for navigation. This mode is activated
when the nunchuck is tilted to either side and deactivated when no longer tilted.

| Control      | Description |
|--------------|-------------|
| Analog Up    | PgUp        |
| Analog Down  | PgDn        |
| Analog Left  | Home        |
| Analog Right | End         |
| Z Button     | Space       |
| C Button     | Tab         |

#### Media

This keyboard mode turns the nunchuck into a media player controller. This mode is activated when the nunchuck
is tilted upwards and deactivated when no longer tilted.

| Control  | Description     |
|----------|-----------------|
| Analog Y | Volume Up/Down  |
| Analog X | Prev/Next Track |
| Z Button | Play/Pause      |
| C Button | Mute            |

#### Exit

This keyboard mode enables the button that exits this mode, in addition to providing extra helpful keys. This
mode is activated when the nunchuck is tilted downwards and deactivated when no longer tilted.

| Control      | Description                                                   |
|--------------|---------------------------------------------------------------|
| C Button     | Return to Mouse mode                                          |
| Z Button     | Esc                                                           |
| Analog Up    | Windows/Super/Command key                                     |
| Analog Down  | Delete                                                        |
| Analog Left  | Hyper: Shift+Ctrl+Alt+Win (useful for custom keyboard macros) |
| Analog Right | Meh: Shift+Ctrl+Alt (useful for custom keyboard macros)       |

## Supported hardware

- [Adafruit Trinket M0](https://www.adafruit.com/product/3500)

## Background

I'm a huge fan of the Swiftpoint Pen-grip line of ergonomic mice and have used them exclusively since 2015.
Unfortunately, this mouse doesn't have drivers for Linux, rendering the mouse unusable outside of Windows
and macOS. Swiftpoint currently has no incentive to develop Linux drivers nor provide the developer
community with an API to write their own.

This problem inspired me to create an ergonomic mouse that has similar features as the Swiftpoint Pen-grip
series, and is guaranteed to work regardless of the user's choice of operating system. I decided to base
this mouse on the Wii Nunchuck controller since it is one of the best ergonomic devices available at a
low price and is easy to interface with.

Nunchuck Mouser is intended to be an alternative mouse in situations where traditional mice are difficult
to use, but it can also be a daily driver.

### Nunchuck Mouser vs. Swiftpoint Pen-grip mice

|                                 | Nunchuck Mouser                                                                                                      | Swiftpoint Pen-grip Mouse                                                                                                         |
|---------------------------------|----------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| **Handedness**                  | Usable in both left and right hands                                                                                  | Usable in the right hand only                                                                                                     |
| **Accuracy**                    | Limited accuracy due to analog stick control                                                                         | High accuracy due to direct hand control of cursor                                                                                |
| **Speed**                       | Cursor speed limited due to analog stick control                                                                     | High sensitivity enables fast cursor speed with minimal hand movements                                                            |   
| **Flexibility**                 | Doesn't require a surface to operate                                                                                 | Requires a surface to operate                                                                                                     |
| **Comfort**                     | Comfortable in the hands and intuitive to use                                                                        | Has a short learning curve to overcome before it can be used comfortably and can be offputting to people used to traditional mice |
| **Supporting software**         | Fully functional without supporting software                                                                         | Requires the Swiftpoint Control Panel to enable all features                                                                      |
| **Software freedom**            | Mouse software is free and open source (MIT License)                                                                 | Mouse software is closed source and only accessible to Swiftpoint                                                                 |
| **Hardware freedom**            | Nunchuck hardware is proprietary but with a publicly available API, while supporting microcontrollers are fully open | Mouse hardware is proprietary and can only update its firmware via the Swiftpoint Control Panel                                   |
| **Supported operating systems** | All operating systems                                                                                                | Windows and macOS support only                                                                                                    |
| **Price**                       | All parts can be purchased for under $25 CAD, but experience with hardware assembly and soldering required           | Cheapest Pen-grip model is $140 CAD, but doesn't require technical skill to use                                                   |
| **Support**                     | None                                                                                                                 | Excellent customer support                                                                                                        |

## License

Nunchuck Mouser is [MIT](https://github.com/goldenpathtechnologies/nunchuck-mouser/blob/main/LICENSE) licensed.