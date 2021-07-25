# Project ATEM - Physical Duel Disk (Arduino)


## Overview
This repo contains the WIP code for the Physical Duel Disk, built for [Project ATEM](https://github.com/BramDC3/smart_duel_gazer). This project currently focusses on using RFID over an SPI interface to relay card info to the Smart Duel Server. While this code currently works, there are major changes coming soon.

For any questions regarding this project, check us out on [Discord](https://discord.gg/XCcfcbBcjE) or [YouTube](https://www.youtube.com/channel/UCz2Que3btHnGkwrW-emZV5A).

## How it Works - The Basics
For the system to work, a card sleeve needs to be outfitted with an NFC chip. When the reader detects a chip it grabs the serial number of the card (which can be added using the built in '*write mode*' feature), checks a couple sensors to determine the position of the card, then writes the information to serial. Then the ESP8266 reads that information and relays it to the Server.
