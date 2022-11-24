# Arduino NANO based EEPROM Programmer

Inspired by [Ben Eater](https://github.com/beneater/eeprom-programmer) with minor imporvements to make it faster.

##Changes
1. Direct writes to the ports instead of digitalWrite()
2. Use asm("nop") to adjust delay where necessary
3. Added serial communication statements to load HEX file
  - First two bytes are lower and upper halfs of the total byte count number
  - Writes always start with address 0x0
4. Used zip socket for easier chip removal
![BreadBoard](pic.JPG)
