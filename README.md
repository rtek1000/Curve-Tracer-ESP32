# Curve-Tracer-ESP32
Fault locator for electronic circuits, comparing V.I. curves (voltage and current)

Operation based on this project: [Curve Tracer ATmega328](https://github.com/rtek1000/Curve-Tracer-ATmega328)

------------

Note: Work in progress, just demonstrative, not functional, sorry.

------------

### Software

- Visual interface via browser (HTML page)
- IP address location via mDNS ([esp32ct.local](http://esp32ct.local/))
- Automatic/Manual test path selection
- Multiple test impedances (x1, x2, x4, x8)

HTML_page (ESP32 Webserver):

![img](https://raw.githubusercontent.com/rtek1000/Curve-Tracer-ESP32/main/Docs/HTML_page.png)

### Hardware

- Total 160 test tracks for each channel (Ch1: 160 probes, Ch2: 160 probes)
- Analog output via the ESP32's internal DAC

Main:

![img](https://raw.githubusercontent.com/rtek1000/Curve-Tracer-ESP32/main/ESP32CT_Hardware/ESP32_Curve_Tracer_pg1.png)

Expansion of probes:

![img](https://raw.githubusercontent.com/rtek1000/Curve-Tracer-ESP32/main/ESP32CT_Hardware/ESP32_Curve_Tracer_pg2.png)

ESP32 board:

![img](https://raw.githubusercontent.com/rtek1000/Curve-Tracer-ESP32/main/Docs/ESP32_Dev_Module.png)

Uploading the Data Folder to the ESP32 (Webserver):

![img](https://raw.githubusercontent.com/rtek1000/Curve-Tracer-ESP32/main/Docs/Data_folder_upload.png)

------------

## Licence

#### Hardware:
Released under CERN OHL 1.2: https://ohwr.org/cernohl

#### Software:
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
