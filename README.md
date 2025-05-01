# esphome-zendure-bridge

A lightweight ESPHome-based Bluetooth bridge for controlling Zendure devices locally with Home Assistant — no cloud required. This project enables secure, low-latency communication between your Zendure device and Home Assistant using ESPHome and a compatible ESP device, bypassing the Zendure cloud and improving privacy, speed, and reliability.

## Supported Zendure Devices

- Hub 1200
- Hub 2000

## Setup

### 🔍 Finding Your Zendure Device's Bluetooth MAC Address

Before connecting your ESP32 to your Zendure Hub, you need to identify its Bluetooth MAC address. There are two simple ways to do this:

#### Option 1: Use a mobile App
 1. Install `nRF Connect for Mobile` (or any similar BLE Scanner app) on your phone (available on Android & iOS).
 2. Open the app and scan for nearby Bluetooth devices.
 3. Turn on your Zendure device and wait for it to appear.
 4. Look for a device that has a name starting with `Zen` and note the MAC address (e.g., C3:7A:7F:9D:1A:B4).
 5. Use this MAC in the ESPHome configuration later.

#### Option 2: Use an ESP32 BLE Scanner with ESPHome
If you prefer using your ESP32:

1. Create a new file with this content and name it `ble_scanner.yaml`:

```yaml
esphome:
  name: blescanner

esp32:
  board: esp32dev

logger:

esp32_ble_tracker:
  on_ble_advertise:
    - then:
        - lambda: |-
            ESP_LOGI("BLE", "BLE Device: %s (%s)", x.get_name().c_str(), x.address_str().c_str());
```

2. Connect your ESP Device via USB
3. Flash the program to your device (`esphome run ble_scanner.yaml`)
4. Power on your Zendure device and look for its MAC address in the logs. Its name will start with `Zen`

Once you have the MAC address, copy it — you'll need it in the main `esphome-zendure-bridge` config!
