# Arduino USB Charge Battery

A universal USB-powered battery charger adjustable from 1.5 V to 12 V, built around an Arduino Uno.  
Monitor real-time voltage and current via I²C, set your charge voltage with a knob, and enjoy automatic safety cutoffs for over-voltage and over-current protection.

Repository: https://github.com/bocaletto-luca/arduino-usb-charge-battery

---

## Table of Contents

1. [Features](#features)  
2. [Hardware Overview](#hardware-overview)  
3. [Bill of Materials](#bill-of-materials)  
4. [Schematic & Wiring](#schematic--wiring)  
5. [Software Setup](#software-setup)  
6. [Usage](#usage)  
7. [Code Structure](#code-structure)  
8. [Calibration & Tuning](#calibration--tuning)  
9. [Troubleshooting](#troubleshooting)  
10. [Contributing](#contributing)  
11. [License](#license)  
12. [Author](#author)

---

## Features

- Adjustable output from **1.5 V** to **12 V**  
- Constant-voltage regulation using an MCP41010 digital potentiometer and an LM2596 buck module  
- Real-time voltage & current sensing via Adafruit INA219 sensor over I²C  
- USB-powered (5 V supply from any standard USB port)  
- Start/stop control through a push-button or serial command  
- Automatic safety shutdown on over-current (> 2 A) or sensor fault  
- Clean separation of hardware configuration (schematic) and firmware (Arduino sketch)

---

## Hardware Overview

This project integrates four main modules:

1. **Arduino Uno**  
   Acts as the brain: reads user setpoint, programs the digital pot, and polls the INA219 sensor.

2. **MCP41010 Digital Potentiometer**  
   Controlled via SPI to adjust the feedback pin on the LM2596, setting the buck converter’s output voltage.

3. **LM2596 Buck Converter Module**  
   Steps down USB 5 V to the desired charge voltage (1.5 V–12 V) based on the digital potentiometer feedback.

4. **Adafruit INA219 Sensor**  
   Measures both bus voltage and current draw on the battery line, communicating results via I²C (A4=SDA, A5=SCL).

A momentary push-button on pin D2 toggles the charging state on and off. All grounds are common.

---

## Bill of Materials

| Qty | Component                       | Description                                           |
|-----|---------------------------------|-------------------------------------------------------|
| 1   | Arduino Uno R3                  | 5 V USB-powered ATmega328P development board          |
| 1   | MCP41010 Digital Potentiometer  | 10 kΩ SPI-controlled pot breakout                     |
| 1   | INA219 Sensor Module            | High-side voltage & current monitoring over I²C       |
| 1   | LM2596 Buck Converter Module    | Adjustable output, remove stock pot for feedback tie-in |
| 1   | 10 kΩ Potentiometer             | User setpoint input to Arduino A0                     |
| 1   | Push-button                     | Momentary switch for D2 start/stop                    |
| 1   | USB A→B Cable                   | Provides 5 V power from PC or USB charger             |
| 1   | Perfboard/PCB                   | To mount modules and wire connections                 |
| 10  | Jumper Wires (M/F)              | For SPI, I²C, power, and signal routing               |
| 2   | 10 µF Electrolytic Capacitors   | Input/output decoupling for LM2596                    |
| 2   | 0.1 µF Ceramic Capacitors       | Bypass decoupling for digital pot & sensor VCC        |
| 1   | 2-Pin Terminal Block            | Battery output connection                             |
| 1   | (Optional) DC Barrel Jack       | Alternative to USB power input                        |

---

## Schematic & Wiring

Refer to `schematic/diagram.md` for a detailed ASCII-style schematic and wiring instructions.  
Key connections:

- **Arduino A0** → Potentiometer wiper (setpoint)  
- **Arduino SPI** (D11 MOSI, D12 MISO, D13 SCK, D10 CS) → MCP41010  
- **MCP41010 Wiper** → LM2596 feedback pin  
- **INA219 Vin+** → LM2596 output; VIN– to battery +  
- **INA219 SDA/SCL** → Arduino A4/A5  
- **Push-button** → D2 (INPUT_PULLUP)  

---

## Software Setup

1. **Clone the repository**

   ```bash
   git clone https://github.com/bocaletto-luca/arduino-usb-charge-battery.git
   cd arduino-usb-charge-battery
   ```

2. **Install Arduino IDE** (v1.8+ recommended)  
3. **Add Libraries**  
   - Open **Library Manager** (Sketch → Include Library → Manage Libraries…)  
   - Search for and install **Adafruit INA219**

4. **Open and Upload Sketch**  
   - Open `src/universal_charger.ino` in the Arduino IDE  
   - Select **Arduino Uno** board and correct COM port  
   - Click **Upload**

---

## Usage

1. **Power** the Arduino via USB (5 V)  
2. **Press** the push-button or send serial command to toggle charging  
3. **Turn** the external potentiometer to set the desired charge voltage  
4. **Monitor** serial output at **115200 baud** for:
   - SET voltage (what the Arduino is commanding)  
   - VOUT voltage (actual measured by INA219)  
   - Current in mA  

Charging will automatically stop if current exceeds 2 A or if the INA219 reports a fault.

---

## Code Structure

- **`src/universal_charger.ino`**  
  Main Arduino sketch: initialization, loop, setpoint reading, pot programming, sensor polling, safety logic.

- **Helper Functions**  
  - `readSetVoltage()` → maps A0 reading to 1.5–12 V  
  - `mapVoltageToPot()` → converts target voltage to 0–255 pot code  
  - `writeDigitalPot(value)` → SPI transfer to MCP41010  
  - `handleButton()` → debounced toggle of charging state

- **Libraries**  
  - `<SPI.h>` for digital pot control  
  - `<Wire.h>` & `<Adafruit_INA219.h>` for current/voltage sensing

---

## Calibration & Tuning

- Verify the LM2596 feedback range on your specific module—adjust mapping constants in `mapVoltageToPot()` if necessary.  
- If your INA219 module is configured for low-side sensing, rewire accordingly and adjust initialization.

---

## Troubleshooting

- **No serial output**: Ensure correct baud rate (115200) and USB cable.  
- **INA219 errors**: Check I²C wiring (A4/A5), pull-up resistors, and module address.  
- **Pot not driving LM2596**: Confirm SPI CS pin and wiring to feedback node.  
- **Output range incorrect**: Recalibrate voltage mapping constants or verify module potentiometer removal.

---

## Contributing

Contributions are welcome! Please:

1. Fork the repository  
2. Create a feature branch (`git checkout -b feat/your-feature`)  
3. Commit your changes (`git commit -m "feat: add …"`)  
4. Push to your branch (`git push origin feat/your-feature`)  
5. Open a Pull Request

---

## License

Distributed under the MIT License. See [LICENSE](https://opensource.org/licenses/MIT) for details.

---

## Author

**Bocaletto Luca**  
GitHub: [bocaletto-luca](https://github.com/bocaletto-luca)  

---
