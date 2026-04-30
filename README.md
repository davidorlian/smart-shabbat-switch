# Smart Shabbat Switch

Remote switch unit for the Smart Shabbat system.

This repository contains the end-unit firmware for the load-side switch module.  
It is designed to work with the central ESP32-based controller from the [`smart-shabbat-clock`](https://github.com/davidorlian/smart-shabbat-clock) repository.

The switch unit receives commands over an HC-12 RF serial link, reads the local AC-sense input, and controls two relays used for Shabbat-mode switching behavior.

---

## System Context

The full Smart Shabbat system is split across two repositories:

- [`smart-shabbat-clock`](https://github.com/davidorlian/smart-shabbat-clock) — central ESP32 clock/controller
- `smart-shabbat-switch` — remote ATmega328P switch unit, documented in this repository

The central controller handles scheduling, timekeeping, user configuration, and RF command transmission.  
This repository contains the firmware for the remote unit that acts near the controlled load.

Basic system flow:

```text
Smart Shabbat Clock
        |
        | HC-12 RF serial command
        v
Smart Shabbat Switch
        |
        | relay control + AC sensing
        v
Local load / switch circuit
```

---

## Firmware Behavior

The firmware listens for textual commands over the serial link used by the HC-12 module.

Main commands:

- `shabbat` — enter Shabbat mode
- `week` — return to normal weekday mode

When entering Shabbat mode, the firmware:

1. Reads the AC-sense input.
2. Enables the Shabbat relay path.
3. Sets the second relay according to the sensed state.
4. Saves the relay state to EEPROM.
5. Sends `ACK` back to the controller.

When returning to weekday mode, the firmware:

1. Disables the Shabbat relay path.
2. Resets the second relay to its weekday default state.
3. Saves the relay state to EEPROM.
4. Sends `ACK` back to the controller.

---

## Hardware

Main hardware used by the end unit:

| Part | Role |
|---|---|
| ATmega328P | Main microcontroller |
| HC-12 | RF serial communication with the central controller |
| Opto-isolated AC sensing circuit | Reads the local AC/load state |
| Relay 1 | Shabbat bypass / switch-neutralization path |
| Relay 2 | Load hold / controlled state path |

---

## Repository Structure

```text
smart-shabbat-switch/
├── firmware/
│   └── Smart_Shabbat_Switch.ino
├── docs/
│   ├── demo/
│   └── project-book/
└── README.md
```

- `firmware/` — Arduino / AVR firmware for the remote switch unit
- `docs/demo/` — demo-related files
- `docs/project-book/` — Hebrew project documentation

---

## Project Book

Hebrew project book:

```text
docs/project-book/Smart_Shabbat_Switch_Project_Book_HE.pdf
```

---

## Quick Start

1. Open `firmware/Smart_Shabbat_Switch.ino` in the Arduino IDE.
2. Select the correct ATmega328P / Arduino AVR board configuration.
3. Select the correct programmer or serial upload method for the target board.
4. Upload the firmware.
5. Verify serial communication with the central controller.
6. Test the `shabbat` and `week` command behavior in a safe low-voltage test setup before connecting to any mains-related hardware.

---

## Dependencies

Typical Arduino / AVR dependencies:

- Arduino AVR core
- `EEPROM` library

The HC-12 module is used as a serial communication link.

---

## Safety Note

This repository documents a prototype involving mains-adjacent switching hardware.

Mains voltage is dangerous. This README is not an installation guide.  
Any real-world installation must use proper isolation, creepage and clearance practices, certified mains-rated components, enclosure protection, strain relief, fusing/protection where required, and review by a qualified person before use.

Do not validate relay or AC-sense behavior directly on mains hardware before testing the firmware and logic in a safe low-voltage setup.

---

## Authors

- Barak Ashwal
- David Orlian
