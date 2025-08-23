# User Guide

## Contents

- Introduction  
- Assembly Overview  
- Safety Warnings  
- Operating Procedures  
- Battery Chemistry Considerations  
- Maintenance  
- Troubleshooting  

---

## Introduction

This universal USB charger lets you programmatically regulate output voltage between 1.5 V and 12 V and monitor current and voltage via the INA219 sensor.  

It uses an Arduino Uno to drive an MCP41010 digital potentiometer that sets the feedback on an LM2596 buck converter.  

Follow this guide to assemble, operate, and maintain your charger safely and effectively.  

---

## Assembly Overview

1. Gather all components and verify wiring as shown in the schematic.  
2. Mount modules on a protoboard or custom PCB, keeping power rails and ground planes neat.  
3. Install decoupling caps: 10 µF electrolytic on LM2596 VIN/VOUT, 0.1 µF ceramic on MCP41010 and INA219.  
4. Route the MCP41010 wiper wire to the LM2596 feedback pin with minimal length.  
5. Place the pushbutton on your enclosure and wire to Arduino D2 and ground.  

Refer to the `docs/images` folder for step-by-step photos.  

---

## Safety Warnings

- Always disconnect input power before touching wiring or modules.  
- Do not exceed the current rating of your LM2596 module; most handle up to 2 A with proper heatsinking.  
- Keep fingers and metal objects away from exposed terminals under power.  
- Never charge batteries unattended; monitor voltage and temperature throughout.  
- Use protective eyewear when testing new configurations under load.  

---

## Operating Procedures

1. Plug the USB 5 V source into the charger’s input.  
2. Press the button to start regulation; the Arduino will print telemetry to the serial monitor at 115 200 baud.  
3. Adjust the front-panel 10 kΩ potentiometer to the desired voltage setpoint (0–12 V).  
4. Observe the printed “Set”, “Act”, and “I” values to confirm proper regulation.  
5. Press the button again to stop charging and safely disconnect your battery.  

---

## Battery Chemistry Considerations

| Chemistry  | Nominal Voltage | Charge Profile | Recommended Max Voltage |
|------------|-----------------|----------------|-------------------------|
| Lead-Acid  | 12 V            | CC then CV     | 14.4 V                  |
| Li-Ion     | 3.7 V/cell      | CC then CV     | 4.20 V/cell            |
| NiMH/NiCd  | 1.2 V/cell      | ΔV detection   | 1.45 V/cell            |

- For multi-cell packs, multiply per-cell voltages by cell count and never exceed the pack’s rated maximum.  
- Li-ion requires precise CV termination; avoid float charging above 4.2 V per cell.  
- Lead-acid benefit from occasional equalization cycles; this charger supports only standard CC/CV.  

---

## Maintenance

- Inspect wiring and solder joints every 3 months for corrosion or loose connections.  
- Clean dust from modules and heatsinks with compressed air.  
- Verify INA219 calibration quarterly by comparing readings to a bench multimeter.  
- Replace electrolytic capacitors after 5 years or if capacitance droops.  

---

## Troubleshooting

- Charger won’t start when button is pressed  
  - Ensure D2 is pulled up and the button grounds it on press.  
  - Check for debounce delay in code (50 ms).  

- Output voltage drifts or oscillates  
  - Shorten the feedback trace from MCP41010 to LM2596 FB.  
  - Add a small RC filter (10 Ω/100 nF) on the feedback node.  

- INA219 reads zero or erratic values  
  - Confirm SDA/SCL wiring and pull-ups.  
  - Swap I²C address if using multiple sensors on the bus.  

---
