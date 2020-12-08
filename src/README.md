PS4 Exploit Host ESP32
================
## What is this?

#### A simple implementation of an HTTP host on the ESP32, using a SPIFFS fliesystem, with a touch of DNS trickery. All PS4 exploits are exclusive to the developers, and hold no bearing on the operation of this simple host. Any complete set of web content containing an index.html will be hosted by the sketch. This is modeled after the same code that runs on the ESP8266 for the same purpose, and should accomplish the same functionality.

## Usage -

Upload the main sketch to your ESP32 device, then upload the data directory to SPIFFS using the ESP32 SPIFFS data uploader. **YOU MUST SET YOUR ESP32 BOARD TO NO OTA 1MB APP/3MB SPIFFS under `Tools > Partition Scheme` inside of the Arduino IDE.** These are bare minimums, if you have a board with larger flash capacity, it will still work, as long as you have **at least 3mb for SPIFFS/1mb for APP**.

**Use the makeSmall.sh script, provided in the src/utility directory, to reduce the size of javascript files, allowing them to fit on the limited 3mb space of the device. This will produce gzipped byte arrays of your standard .js files, that will be decompressed in browser on the PS4. Copy the makeSmall.sh script to the data directory with the files you want to compress, then execute it. It will create gzipped files and delete the source js files for you. Caching is completely functional.**

## Creating a distributable bin -

The only method I've found to create a distributable bin is to dump the entire 4mb flash, this creates a .bin file that can be flashed using [NodeMCU-PyFlasher](https://github.com/marcelstoer/nodemcu-pyflasher/releases).

To **READ FLASH**:
```python esptool.py -b 115200 --port COM3 read_flash 0x00000 0x400000 flash_4M.bin```

To **WRITE FLASH** (or use **PyFlasher**):
```python esptool.py -b 115200 --port COM3 write_flash --flash_freq 80m 0x000000```
