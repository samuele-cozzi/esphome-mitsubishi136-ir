# Mitsubishi 136 IR Climate Component - Usage Guide

## Overview

This ESPHome component controls Mitsubishi air conditioners using the 136-bit IR protocol. It uses the proven [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) library for reliable IR transmission.

## Quick Start

### 1. Hardware Setup

Connect an IR LED circuit to a GPIO pin:

```
ESP32/ESP8266 GPIO → 470Ω Resistor → IR LED Anode
                                      IR LED Cathode → GND
```

**Common GPIO pins:**
- ESP32: 4, 5, 18, 19, 23
- ESP8266: 4, 5, 12, 13, 14

### 2. ESPHome Configuration

```yaml
external_components:
  - source: github://samuele-cozzi/esphome-mitsubishi136-ir
    components: [mitsubishi136_ir]

climate:
  - platform: mitsubishi136_ir
    id: living_room_ac
    name: "Living Room AC"
    ir_pin: 4  # GPIO pin (default: 4)
```

### 3. Minimal Configuration

```yaml
climate:
  - platform: mitsubishi136_ir
    name: "My AC"
    # Uses GPIO 4 by default
```

## Configuration Reference

### Variables

| Variable | Type | Default | Description |
|----------|------|---------|-------------|
| `id` | string | *required* | Unique identifier |
| `name` | string | *required* | Friendly name for Home Assistant |
| `ir_pin` | integer | 4 | GPIO pin for IR LED (0-39) |

### Example with All Options

```yaml
climate:
  - platform: mitsubishi136_ir
    id: bedroom_ac
    name: "Bedroom Air Conditioner"
    ir_pin: 5
```

## Supported Modes

The component supports the following operating modes:

- **OFF** - Turns off the AC
- **COOL** - Cooling mode
- **HEAT** - Heating mode
- **DRY** - Dehumidification/dry mode
- **FAN_ONLY** - Fan-only mode
- **HEAT_COOL** - Automatic mode (switches between heat and cool)

## Supported Swing Modes

- **OFF** - Louvers fixed
- **VERTICAL** - Vertical louver swing

## Temperature Control

- **Range:** 16-32°C
- **Step:** 0.5°C
- **Default Target:** 24°C
- **Default Current:** 20°C

## Home Assistant Services

Once integrated with Home Assistant, the climate entity supports:

### Turn Off
```yaml
service: climate.turn_off
target:
  entity_id: climate.living_room_ac
```

### Turn On
```yaml
service: climate.turn_on
target:
  entity_id: climate.living_room_ac
```

### Set Target Temperature
```yaml
service: climate.set_temperature
target:
  entity_id: climate.living_room_ac
data:
  temperature: 24
```

### Set HVAC Mode
```yaml
service: climate.set_hvac_mode
target:
  entity_id: climate.living_room_ac
data:
  hvac_mode: cool  # off, heat, cool, dry, fan_only, heat_cool
```

### Set Swing Mode
```yaml
service: climate.set_swing_mode
target:
  entity_id: climate.living_room_ac
data:
  swing_mode: "off"  # "off" or "vertical"
```

## Home Assistant Automations

### Example 1: Morning Cool

```yaml
automation:
  - alias: Morning Cool
    trigger:
      platform: time
      at: "06:00:00"
    action:
      service: climate.set_temperature
      target:
        entity_id: climate.living_room_ac
      data:
        temperature: 22
        hvac_mode: cool
```

### Example 2: Evening Shutdown

```yaml
automation:
  - alias: Evening AC Off
    trigger:
      platform: time
      at: "22:00:00"
    action:
      service: climate.turn_off
      target:
        entity_id: climate.living_room_ac
```

### Example 3: Temperature-Based Control

```yaml
automation:
  - alias: Auto AC Control
    trigger:
      platform: numeric_state
      entity_id: sensor.room_temperature
      above: 28
    action:
      service: climate.set_hvac_mode
      target:
        entity_id: climate.living_room_ac
      data:
        hvac_mode: cool
```

## Advanced Configuration

### With Room Temperature Sensor

```yaml
sensor:
  - platform: dht
    pin: GPIO23
    temperature:
      name: "Room Temperature"

climate:
  - platform: mitsubishi136_ir
    name: "Living Room AC"
    ir_pin: 4
```

### Multiple AC Units

```yaml
climate:
  - platform: mitsubishi136_ir
    id: living_room_ac
    name: "Living Room AC"
    ir_pin: 4

  - platform: mitsubishi136_ir
    id: bedroom_ac
    name: "Bedroom AC"
    ir_pin: 5
```

## Logging

Enable DEBUG logging to see IR command details:

```yaml
logger:
  level: DEBUG
  logs:
    mitsubishi136_ir: DEBUG
```

Output example:
```
[DEBUG] mitsubishi136_ir: Sent AC command - Mode: 3, Temp: 24.0°C, Swing: 0
```

## Troubleshooting

### IR LED Not Working

**Issue:** AC doesn't respond to commands

**Solutions:**
1. Check IR LED polarity (longer leg to GPIO via resistor, short leg to GND)
2. Verify GPIO pin isn't used by other components
3. Test IR with phone camera (IR should be visible)
4. Try a different GPIO pin

### GPIO Pin Conflict

**Issue:** Component won't load or crashes

**Solutions:**
1. Check GPIO isn't used by other components
2. Avoid GPIO 0, 2, 6-11 on ESP8266 (reserved)
3. Use valid pins: ESP32 (0-39), ESP8266 (0-16)

### AC Not Responding

**Issue:** AC receives signal but doesn't change state

**Solutions:**
1. Verify AC model uses 136-bit Mitsubishi protocol
2. Test with original remote control
3. Check AC isn't in a locked state
4. Increase IR LED brightness (larger resistor value)

### Cache Issues

**Issue:** Changes don't take effect after update

**Solutions:**
1. Clear ESPHome cache: `rm -rf .esphome/`
2. Clean build: `rm -rf build/`
3. Restart ESPHome or Home Assistant

## Technical Details

### IR Protocol

- **Format:** Mitsubishi 136-bit
- **Carrier:** 38 kHz modulated
- **Header:** 3.65ms mark, 1.62ms space
- **Bit Rate:** 325μs mark, 1280μs/420μs space
- **Data:** 17 bytes + checksum

### Library

Uses [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) for:
- Hardware IR transmission via GPIO
- Mitsubishi protocol handling
- Carrier frequency management

## References

- [IRremoteESP8266 Library](https://github.com/crankyoldgit/IRremoteESP8266)
- [ESPHome Climate Component](https://esphome.io/components/climate/)
- [Mitsubishi Protocol Documentation](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/ir_Mitsubishi.cpp)

## Support

For issues: https://github.com/samuele-cozzi/esphome-mitsubishi136-ir/issues
