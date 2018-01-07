# Bluetooth

Bluetooth experiments on an ESP32 microcontroller

## Bluetooth Advertising

Sending bluetooth advertising out with a General Access Profile.

### What is a GAP?

The Generic Access Profile controls connections and advertising.

### Advertising

The peripheral sends advertising data at regular interval. The
advertising payload contains:

|Member|Type|Description|
|:----|:----:|----------:|
|set_scan_rsp|bool|Accept scan response or not|
|include_name|bool|include device name or not|
|include_txpower|bool|inlude tx power or not|
|min_interval|int|send at minimum interval|
|max_interval|int|send at maximum interval|
|appearance|int|external appearance of the device|
|manufacturer_len|uint16_t|manufacturer data length|
|p_manufacturer_data|uint8_t *|manufacturer data point|
|service_data_len|uint16_t|service data length|
|p_service_data|uint8_t *|service data point|
|service_uuid_len|uint16_t|service unique user identifier length|
|p_service_uuid|uint8_t *|service unique user identifier array point|
|flag|uint8_t|flag of discovery mode|