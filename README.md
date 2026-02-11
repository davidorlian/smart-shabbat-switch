# Smart Shabbat Switch

Remote Shabbat switch unit designed for Sabbath-safe operation: disables physical switch state changes during Shabbat while keeping the load state stable. Communicates with the Smart Shabbat Clock via HC-12 RF.

## Project Book
- Hebrew (PDF): `docs/project-book/Smart_Shabbat_Switch_Project_Book_HE.pdf`

## Hardware (End Unit)
- MCU: ATmega328P
- RF module: HC-12
- AC-sense: opto-isolated load-line detection
- Relays: 2x mains-rated relays (Relay1: switch bypass, Relay2: Shabbat hold)

## Repository Structure
- `firmware/` – MCU firmware (Arduino / AVR)
- `docs/` – documentation and project book

## Key Features
- Shabbat bypass mode (switch neutralization)
- RF link (HC-12) to central clock/controller
- Stable load state during Shabbat
- Designed for wall-box installation (end-unit)

## Quick Start (Arduino IDE)
1. Open the firmware project under `firmware/` (main `.ino` file).
2. Select the correct board/package:
   - ATmega328P: Arduino AVR Boards / MiniCore (depending on your setup)
   - ATtiny85: ATTinyCore (if used)
4. Connect programmer (USBasp / Arduino as ISP / etc.) and select the correct COM port.
5. Upload.
6. Verify communication with the clock/controller and validate Shabbat bypass behavior.

## Dependencies
Typical AVR/Arduino dependencies (may vary by MCU/build):
- `SoftwareSerial` (commonly used for HC-12 on AVR)
- `EEPROM` (optional, for storing settings)
- Core Arduino AVR libraries

## Safety Note
Mains voltage is dangerous. Use proper isolation, creepage/clearance rules, and certified mains-rated components. Validate the design carefully before installation in a wall box.

## Authors
- Barak Ashwal
- David Orlian
