# esphome-zendure-bridge

A lightweight ESPHome-based Bluetooth bridge for controlling Zendure devices locally with Home Assistant — no cloud required. This project enables secure, low-latency communication between your Zendure device and Home Assistant using ESPHome and a compatible ESP device, bypassing the Zendure cloud and improving privacy, speed, and reliability.

![zendure-bridge-esphome](https://github.com/user-attachments/assets/81600a2d-d515-4b36-9899-149ddc910802)

## Supported Zendure Devices

- Hub 1200
- Hub 2000
- AB1000
- AB1000S
- AB2000
- AB2000S
- AB2000X

## Setup

> Once ESPHome is running and connecting to your device via BLE, the device will no longer send telemetry data to the cloud. The device will still be in the App, but will not receive updated live data and the energy modes in the app will stop working. You need to create automations in HA to control the device.

  1. Find the MAC address of your Zendure device(s). (see under Guides below)
  2. Find the serial numbers of your batteries.
  3. Create a main esphome configuration similar to one of the files in examples/
     Refer to https://esphome.io/guides/getting_started_hassio to see how to get started
  4. Configure the Zendure package correctly in your esphome yaml file
```yaml
time:
  - platform: sntp
    id: esptime
    timezone: "UTC"
    servers:
      - 0.pool.ntp.org

packages:
  zendure:
    url: https://github.com/danez/esphome-zendure-bridge
    files:
      - path: packages/hub2000.yaml # choose your device: hub2000, hub1200
        vars:
          mac_address: 00:00:00:00:00:00 # set the MAC address you looked for before here
          time_id: esptime # the id of the time component above
          name: "Hub 2000" # choose how you want your device to be named, can be any name and will also be used in the Home Assistant ID and name. Make sure that every device has a different name.
          id: "hub_2000" # the internal ID of the device, if you have multiple devices the id needs to be different
      - path: packages/ab2000.yaml # choose your battery: ab1000, ab2000
        vars:
          name: "Hub 2000 Battery 1" # choose how you want your battery to be named, can be any name and will also be used in the Home Assistant ID and name
          device_id: "hub_2000" # Important!! Needs to be exactly the same as the id of your device where the battery is connected
          serial_number: "CO4ABCDEF123456" # The serial number of your battery
      - path: packages/ab2000.yaml
        vars:
          name: "Hub 2000 Battery 2" # Ensure when having multiple devices/batteries that they have different names
          device_id: "hub_2000"
          serial_number: "CO4XXXXXX654321"
    ref: v1
    refresh: 1d # Update the package once a day
```
  5. Follow the steps of ESPHome Device Build to get your ESP device flashed.

## Guides

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
4. Power on your Zendure device and look for its MAC address (e.g., C3:7A:7F:9D:1A:B4) in the logs. Its name will start with `Zen`

Once you have the MAC address, copy it — you'll need it in the main `esphome-zendure-bridge` config!
