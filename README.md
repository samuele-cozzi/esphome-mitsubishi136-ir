# ESPHome Mitsubishi 136 IR Climate Component

An [ESPHome](https://esphome.io/) external component that adds IR control for **Mitsubishi 136-bit protocol** air conditioners (e.g. MSZ-SF series) via the [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) library.

## Features

- Full climate entity integration with Home Assistant
- Supported modes: Off, Cool, Heat, Dry, Fan Only, Auto
- Supported fan speeds: Auto, Low, Medium, High
- Visual temperature range: 16 °C – 30 °C (1 °C steps)
- Current temperature reporting support

## Requirements

- ESPHome ≥ 2024.x
- An ESP8266 or ESP32 board with an IR LED connected to a GPIO pin
- Home Assistant (optional, for UI control)

## Installation

Add the external component to your ESPHome YAML configuration:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/samuele-cozzi/esphome-mitsubishi136-ir
    components: [mitsubishi136_ir]
```

> [!WARNING]
> Only Arduino Framework is supported

## Configuration

```yaml
mitsubishi136_ir:
  - platform: mitsubishi136_ir
    name: "Living Room AC"
    ir_pin: 4          # GPIO pin connected to the IR LED (default: 4)
```

### Full example with ESPHome

```yaml
esphome:
  name: esp-thermostat

esp32:
  board: esp32dev
  framework:
    type: arduino

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

api:
ota:

external_components:
  - source:
      type: git
      url: https://github.com/samuele-cozzi/esphome-mitsubishi136-ir
    components: [mitsubishi136_ir]

mitsubishi136_ir:
  - platform: mitsubishi136_ir
    name: "Living Room AC"
    ir_pin: 4
```

## Wiring

Connect a standard 940 nm IR LED to the chosen GPIO pin through a current-limiting resistor (~100 Ω). For longer range, use a transistor driver circuit as described in the [IRremoteESP8266 wiring guide](https://github.com/crankyoldgit/IRremoteESP8266/wiki).

```
ESP GPIO4 ──[ 100Ω ]──► IR LED ── GND
```

## Fan Speed Mapping

| Home Assistant | Mitsubishi IR |
|---|---|
| Auto | Medium (fallback) |
| Low | Min |
| Medium | Med |
| High | Max |

## Mode Mapping

| Home Assistant | Mitsubishi IR |
|---|---|
| Off | Power Off |
| Cool | Cool |
| Heat | Heat |
| Dry | Dry |
| Fan Only | Fan |

## Dependencies

- [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) — IR protocol library by crankyoldgit

## References

- [mistic100](https://github.com/mistic100/ESPHome-IRremoteESP8266)
- [smazurov](https://github.com/smazurov/esphome-ykh531e/blob/main/esphome/components/ykh531e/climate.py)
- [mazurov.dev](https://mazurov.dev/posts/esphome-ykh531e-climate-component/)

## License

This project is licensed under the [MIT License](LICENSE).
