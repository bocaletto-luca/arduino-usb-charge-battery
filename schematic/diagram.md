# Schematic & Wiring

A universal 1.5 V–12 V USB charger using Arduino Uno, MCP41010 digital pot, LM2596 buck converter, and INA219 sensor.  
All grounds must be common. Decoupling capacitors are recommended at each module’s power pins.

---

## 1. Wiring Overview

- **USB 5 V**  
  Powers Arduino 5 V rail and LM2596 input.  

- **Arduino Uno**  
  • A0 (Analog In): User setpoint from 10 kΩ potentiometer  
  • D10 (Digital Out): CS for MCP41010  
  • D11 (MOSI), D12 (MISO), D13 (SCK): SPI bus for MCP41010  
  • A4 (SDA), A5 (SCL): I²C bus to INA219  
  • D2 (Input_Pullup): Push-button for start/stop  

- **MCP41010 Digital Potentiometer**  
  • VDD → 5 V  
  • GND → GND  
  • CS → Arduino D10  
  • SCK → Arduino D13  
  • SDI (MOSI) → Arduino D11  
  • SDO (MISO) → Arduino D12 (unused)  
  • PW0 (wiper) → LM2596 feedback node  

- **LM2596 Buck Converter Module**  
  • VIN → 5 V USB  
  • GND → GND  
  • Feedback pin → MCP41010 wiper  
  • VOUT → INA219 VIN+ → Battery + terminal  

- **Adafruit INA219 Sensor**  
  • V+ → LM2596 VOUT (before battery)  
  • V– → Battery +  
  • GND → GND  
  • SDA → Arduino A4  
  • SCL → Arduino A5  

- **Push-button**  
  • One side → Arduino D2  
  • Other side → GND (uses INPUT_PULLUP)  

- **Battery Pack**  
  • “+” terminal → INA219 V–  
  • “–” terminal → GND  

---

## 2. ASCII Schematic

```text
            +5 V USB
               │
       ┌───────┴───────┐
       │               │
   ┌───▼───┐       ┌───▼───┐
   │Arduino│       │LM2596 │
   │ Uno   │       │Buck   │
   │       │       │Module │
   └───┬───┘       └───┬───┘
       │               │
       │5 V            │VOUT
       │               │
       │           ┌───▼────────────────────────┐
       │           │     INA219 High-Side       │
       │           │  Voltage/Current Sensor    │
       │           └───┬───────────────────────┘
       │               │
   ┌───▼───┐       ┌───▼───┐
   │MCP41010│      │Battery│
   │  Pot   │      │Pack   │
   └───┬───┘       └───┬───┘
       │PW0            │+
  ┌────▼────────┐      └───┐
  │LM2596 FDBK │          │
  └────────────┘         GND
       │
      GND ←──────────────────────────────────────────────┐
                                                          │
                    ┌────────────────────┐                │
                    │ 10 kΩ Potentiometer│
                    │ (Setpoint → A0)    │
                    └────────────────────┘

[ SPI ]
D10 ── CS ──► MCP41010 ──┬─ PW0 ─► LM2596 FB
D11 ── MOSI ─► SDI       │
D12 ── MISO ─► SDO       │
D13 ── SCK  ─► SCK       │

[ I²C ]
A4 ── SDA ──► INA219
A5 ── SCL ──► INA219

[ Button ]
D2 ──► Push-button ──► GND
