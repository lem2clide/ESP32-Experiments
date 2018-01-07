#include <Arduino.h>
#include <iostream>
#include "bt.h"
#include "bta_api.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"

/*
** Bluetooth advertising configuration data
*/
static esp_ble_adv_data_t ble_advertising_data = {
    .set_scan_rsp        = false,
    .include_name        = true,
    .include_txpower     = true,
    .min_interval        = 512,
    .max_interval        = 1024,
    .appearance          = 0,
    .manufacturer_len    = 0,
    .p_manufacturer_data = NULL,
    .service_data_len    = 0,
    .p_service_data      = NULL,
    .service_uuid_len    = 0,
    .p_service_uuid      = NULL,
    .flag                = (ESP_BLE_ADV_FLAG_GEN_DISC|ESP_BLE_ADV_FLAG_BREDR_NOT_SPT)
};

/*
** Bluetooth advertising parameters
*/
static esp_ble_adv_params_t ble_advertising_parameters = {
    .adv_int_min         = 512,
    .adv_int_max         = 1024,
    .adv_type            = ADV_TYPE_NONCONN_IND,
    .own_addr_type       = BLE_ADDR_TYPE_PUBLIC,
    .peer_addr           = {0x00, },
    .peer_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map         = ADV_CHNL_ALL,
    .adv_filter_policy   = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

/*
** Starts advertising when the event ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT is
** received.
*/
static void CallbackEventDataSet(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *) {
    if (event == ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT) {
        esp_ble_gap_start_advertising(&ble_advertising_parameters);
    }
}

/*
** Initializes the Bluetooth generic access profile
*/
static bool InitializeGAP(const char* name) {
    if (!btStarted() && !btStart()) {
        Serial.println("Cannot start bluetooth");
        return false;
    }

    esp_bluedroid_status_t bluetooth_state = esp_bluedroid_get_status();
    if(bluetooth_state == ESP_BLUEDROID_STATUS_UNINITIALIZED){
        if (esp_bluedroid_init()) {
            Serial.println("Cannot initalize bluedroid");
            return false;
        }
    }

    /* Checks if bluetooth is enabled */
    if(bluetooth_state != ESP_BLUEDROID_STATUS_ENABLED){
        if (esp_bluedroid_enable()) {
            Serial.println("Cannot enable bluedroid");
            return false;
        }
    }

    /* Sets the GAP device name */
    if(esp_ble_gap_set_device_name(name)){
        Serial.println("Cannot set the GAP device name");
        return false;
    }

    /* Configures the advertising data */
    if(esp_ble_gap_config_adv_data(&ble_advertising_data)){
        Serial.println("Cannot configure the GAP advertising data");
        return false;
    }

    /* Registers a callback for GAP */
    if(esp_ble_gap_register_callback(CallbackEventDataSet)) {
        Serial.println("Cannot register GAP callback");
        return false;
    }
    return true;
}

/*
** Stops advertising bluetooth
*/
static bool StopAdvertisingGAP() {
    if (btStarted()) {
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
        btStop();
    }
    return true;
}

/* Bluetooth low energy */
class BluetoothLE {
public:
  BluetoothLE(const char* name)
  : ble_name(name) {}

  ~BluetoothLE(void) {
      StopAdvertisingGAP();
  }

  /* Starts advertising */
  bool start(void) const {
      return InitializeGAP(ble_name);
  }

  /* Stops advertising */
  void stop(void) const {
      StopAdvertisingGAP();
  }

  const char* GetName() const {
      return ble_name;
  }

private:
  const char* ble_name;
};

BluetoothLE bluetooth("Foo");

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
}

void loop() {
    delay(3000);
    static bool BLEStatus = bluetooth.start();
    if (BLEStatus == true) {
        Serial.println("Advertising");
    } else {
        Serial.println("Cannot Advertise");
    }
}
