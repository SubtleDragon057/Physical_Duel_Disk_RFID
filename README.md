# Project ATEM - Physical Duel Disk (Arduino)


## Overview
This repo contains some WIP code for a Physical Duel Disk, built to pair with the Project ATEM [Smart Duel Gazer](https://github.com/BramDC3/smart_duel_gazer). This project currently focusses on using RFID over an SPI interface to relay card info to the [Smart Duel Server](https://github.com/BramDC3/smart_duel_server). Please note that with the release of the Smart Duel Server V0.4.0 connection to the server is no longer possible. Updates coming soon!

For any questions regarding this project, check us out on [Discord](https://discord.gg/XCcfcbBcjE) or [YouTube](https://www.youtube.com/channel/UCz2Que3btHnGkwrW-emZV5A).

## How it Works - The Basics
For the system to work, a card sleeve needs to be outfitted with an NFC chip. When the reader detects a chip it grabs the serial number of the card (which can be added using the built in '*write mode*' feature), checks a couple sensors to determine the position of the card, then writes the information to serial. Then the ESP8266 reads that information and relays it to the Server.

## Upcoming Changes - Major
1. Move From SPI to I2C => the plan is to move to I2C and sacrifice a little bit of speed for simplicity. SPI requires more wires, which means more areas that are likely to break or fail.
2. Development of a custom PCB board => A custom PCB is under development to streamline the need for multiple breakout boards. This project will use the newly developed board once available

## Upcoming Changes - Minor
1. Improvements to lobby system
2. Removal of LDR sensors to facilitate a more consistent way of determining card position
3. Addition of buttons for various card actions
