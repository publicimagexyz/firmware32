#include "core/powerSave.h"
#include <interface.h>

// GPIO34 and GPIO35 are input-only; INPUT_PULLUP is silently ignored by ESP32.
// External 10 kΩ pull-ups to 3V3 are required on both pins.
// GPIO12 (IR TX) must be driven LOW before any other setup because it is a
// flash-voltage strapping pin; a high level at boot corrupts OTP settings.

void _setup_gpio() {
    // IR TX — force LOW immediately to satisfy GPIO12 strapping requirement
    pinMode(TXLED, OUTPUT);
    digitalWrite(TXLED, LOW);

    // Navigation buttons
    // GPIO2 supports INPUT_PULLUP; GPIO34/35 do not — use INPUT only.
    pinMode(UP_BTN,  INPUT);   // external 10k pullup required
    pinMode(SEL_BTN, INPUT);   // external 10k pullup required
    pinMode(DW_BTN,  INPUT_PULLUP);

    // IR RX — configure as input (no pullup needed; TSOP4838 has open-collector output)
    pinMode(RXLED, INPUT);

    // Default RFID module to RC522 on the hardware SPI bus
    bruceConfigPins.rfidModule = RC522_SPI_MODULE;

    // Use hardware I²C for accessory bus
    bruceConfigPins.i2c_bus.sda = (gpio_num_t)GROVE_SDA;
    bruceConfigPins.i2c_bus.scl = (gpio_num_t)GROVE_SCL;
}

void _post_setup_gpio() {}

int getBattery() { return 0; }

bool isCharging() { return false; }

// SSD1306 is self-luminous; brightness control is a no-op here.
void _setBrightness(uint8_t brightval) { (void)brightval; }

void InputHandler(void) {
    static unsigned long tm = 0;
    if (millis() - tm < 200 && !LongPress) return;

    // Read all three buttons (LOW = pressed for each)
    bool u = digitalRead(UP_BTN)  == LOW;
    bool s = digitalRead(SEL_BTN) == LOW;
    bool d = digitalRead(DW_BTN)  == LOW;

    if (u || s || d) {
        tm = millis();
        if (!wakeUpScreen()) AnyKeyPress = true;
        else return;
    }

    if (u) PrevPress = true;
    if (d) NextPress = true;
    if (s) SelPress  = true;

    // Hold UP + SELECT simultaneously to escape / go back
    if (u && s) {
        EscPress  = true;
        PrevPress = false;
        SelPress  = false;
    }
}

void powerOff() {
    // Wake on SELECT press
    esp_sleep_enable_ext0_wakeup((gpio_num_t)SEL_BTN, LOW);
    esp_deep_sleep_start();
}

void checkReboot() {
    // Hold UP + DOWN for 3 seconds to reboot
    if (digitalRead(UP_BTN) == LOW && digitalRead(DW_BTN) == LOW) {
        uint32_t t = millis();
        while (digitalRead(UP_BTN) == LOW && digitalRead(DW_BTN) == LOW) {
            if (millis() - t > 3000) {
                esp_restart();
            }
        }
    }
}
