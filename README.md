**mx3100drv** is a Unix (Linux & Mac) driver for the Perixx MX-3100 gaming mouse. It allows you to change all associated settings, and save to the onboard memory of the mouse.

Installation
----------------

**Dependencies**: **[HIDAPI](http://www.signal11.us/oss/hidapi/)** for communicating with the mouse. 

On Mac, you must install a very recent version of HIDAPI, probably using the [github](https://github.com/signal11/hidapi) version and installing from source. v0.8.0 does not include the changes necessary to attach to the right USB interface of the mouse.

On Linux, it is recommended to use the libusb backend for HIDAPI, so libusb1.0+ should also be installed. But it is not required. If you do not want to use the libusb backend for HIDAPI, you may change the Makefile at the relevant Linux section from

```
        LIBS = -lhidapi-libusb
```

to

```
        LIBS = -lhidapi-hidraw
```

1. `make`
2. `sudo make install`

To use this right after installing, you should do *one* of the following (to use the driver without being root):

* Unplug and re-plug-in the mouse
* Restart
* run:

```
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Usage
-----

**mx3100drv** [OPTIONS] COMMAND [ARGUMENTS]

## Options
- `-h`, `--help`
- `-v`, `--version`

## Commands
### Configuration Commands
inspecting or changing mouse settings. Provide all arguments to change a setting to be that value. Leave off the final optional argument to retrieve the current value of that setting.
 
 - **snap** [`ON`|`OFF`]
    Controls the in-mouse Angle Correction feature to change slight curve movements of the mouse to straighter lines
- **angle** [`LEVEL`]
    Controls the in-mouse Gesture correction feature to adjust the mouse movement angle. Valid values are -2,-1,0,1,2 (for -30 degrees, -15, 0, 15, 30, respectively).
- **led** [`MODE`]
    Determines LED mode. Valid `MODE`s are: `standard`, `breathe`, `neon`, and `off`. Standard mode is a solid LED color (color determined by DPI color), dimmable via the **brightness** setting. Breathe mode pulses the LED, with timing controlled by **speed** option, color determined by DPI color. Neon mode cycles between all colors, the speed of that cycle controlled by **speed** command, DPI color is ignored. Off turns the LED off.
- **speed** [`TIME`]
    Determines the speed of the LED pulsing when in Breathe mode, or cycling in Neon mode. Smaller number is faster. Min: 1, Max: 15
- **brightness** [`VALUE`]
   Determines the intensity of the LED in standard mode. Min: 0, Max: 10. A setting of 0 does not turn the LED off. Use the **led** command set to `off` to do that.

### Bulk Commands

- **reset**  
    Resets all mouse configurations, settings, macros, and profiles to factory default. Similar to a recovery mode.
- **dump** `FILE`  
    Save the entire mouse configuration including all settings, macros, and profiles to a `FILE`. `-` may be provided in place of a file name for stdout.
- **load** `FILE`  
    Load the entire mouse configuration including all settings, macros, and profiles from a `FILE` into memory. `-` may be provided in place of a file name for stdin.

## Examples


Notes
---------

* This is a userspace driver, not for use as part of the kernel
* This driver is only for settings configuration. It does not interact with the pointer or keyboard interfaces of the mouse. These will continue to use the default HID driver.

Contributing
--------------

Please feel free to open issues with improvements, or suggestions, bugs, etc. Pull requests are also welcome. Opening issues to discuss patches and features first are encouraged.


License
------------
```
MX3100DRV - portable driver for the MX-3100 gaming mouse
Copyright (C) 2017  Dan Panzarella

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
```


This software and all files included are licensed under the GPL v2. See `doc/LICENSE` for the full terms.