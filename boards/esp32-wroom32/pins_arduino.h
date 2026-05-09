#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

// ─── UART ────────────────────────────────────────────────────────────────────
static const uint8_t TX = 1;
static const uint8_t RX = 3;
static const uint8_t TXD2 = 17;
static const uint8_t RXD2 = 16;

// ─── I2C (SSD1306 OLED + accessory bus) ─────────────────────────────────────
// GPIO21/22 are the hardware I2C pins on ESP32-WROOM-32.
// Both the OLED display and any I2C peripherals share this bus.
#define GROVE_SDA 21
#define GROVE_SCL 22
static const uint8_t SDA = GROVE_SDA;
static const uint8_t SCL = GROVE_SCL;

// ─── SPI (RC522 RFID) ────────────────────────────────────────────────────────
// Hardware VSPI pins.  Shared with any other SPI peripheral added later.
// GPIO5  is also a strapping pin (HIGH → SDIO clock untouched at boot)
//        — RC522 CS idles HIGH, so boot is unaffected.
#define SPI_SCK_PIN  18
#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SS_PIN    5   // RC522 NSS/CS

static const uint8_t SS   = SPI_SS_PIN;
static const uint8_t MOSI = SPI_MOSI_PIN;
static const uint8_t MISO = SPI_MISO_PIN;
static const uint8_t SCK  = SPI_SCK_PIN;

// RC522 reset line.  Bruce does not drive this automatically; tie to 3V3 or
// manage via the MFRC522v2 library's soft-reset in PCD_Init().
#define RFID_RST_PIN  4

// ─── SD CARD ─────────────────────────────────────────────────────────────────
// No SD card on this build — all disabled.
#define SDCARD_CS   -1
#define SDCARD_SCK  -1
#define SDCARD_MISO -1
#define SDCARD_MOSI -1

// ─── NAVIGATION BUTTONS (active LOW) ─────────────────────────────────────────
// ⚠  GPIO34 / GPIO35 are input-only on ESP32-WROOM-32 — NO internal pull-up!
//    Fit a 10 kΩ resistor from each pin to 3V3 on your PCB.
// ⚠  GPIO2 is a strapping pin.  Pressing DOWN while powering on enters
//    UART download mode.  Release before booting for normal operation.
#define HAS_BTN   1
#define HAS_3_BUTTONS
#define SEL_BTN  34   // SELECT — input-only, external 10k pullup required
#define UP_BTN   35   // UP     — input-only, external 10k pullup required
#define DW_BTN    2   // DOWN   — strapping pin, avoid pressing at power-on
#define BTN_ALIAS "\"OK\""
#define BTN_ACT   LOW

// ─── IR TX / RX ──────────────────────────────────────────────────────────────
// ⚠  GPIO12 is a strapping pin for flash voltage (MTDI).  It MUST be LOW
//    during boot.  The PN2222A base is driven through a series resistor only
//    (no pull-up), so GPIO12 is held LOW by ESP32's internal pull-down at boot.
//    Do NOT add a pull-up resistor to the transistor base.
#define TXLED   12   // IR TX via PN2222A — initialised LOW in _setup_gpio()
#define LED_ON  HIGH
#define LED_OFF LOW

#define RXLED   13   // TSOP4838 IR receiver


// IR_TX_PINS / IR_RX_PINS / RF_TX_PINS / RF_RX_PINS defined in esp32-wroom32.ini
// using brace-initializer syntax that requires PlatformIO's -D quoting.

// ─── RF (one-pin OOK modules) ────────────────────────────────────────────────
// No dedicated RF module; expose Grove pins for optional connection.
// ─── CC1101 / NRF24 — not fitted ─────────────────────────────────────────────
#define CC1101_GDO0_PIN -1
#define CC1101_SS_PIN   -1
#define CC1101_MOSI_PIN SPI_MOSI_PIN
#define CC1101_SCK_PIN  SPI_SCK_PIN
#define CC1101_MISO_PIN SPI_MISO_PIN

#define NRF24_CE_PIN   -1
#define NRF24_SS_PIN   -1
#define NRF24_MOSI_PIN SPI_MOSI_PIN
#define NRF24_SCK_PIN  SPI_SCK_PIN
#define NRF24_MISO_PIN SPI_MISO_PIN

// ─── DISPLAY (SSD1306 128×64 OLED via LovyanGFX I²C) ────────────────────────
#define HAS_SCREEN 1
#define ROTATION   1   // 0=0°, 1=90°, 2=180°, 3=270° — adjust to taste

// Font scale factors.  SSD1306 is 128×64; keep fonts small.
#define FP 1
#define FM 1
#define FG 2

#define MINBRIGHT 0    // OLED is self-luminous; no backlight dimming
#define TFT_BL   -1
#define TOUCH_CS -1

// LovyanGFX I²C bus
// If display shows all pixels lit, try Panel_SH1107 (SH1106 modules are
// commonly mislabeled SSD1306 and have different pixel addressing).
// If colours are inverted, set TFT_INVERTION to 1.
#define USE_LOVYANGFX  1
#define LOVYAN_PANEL   Panel_SSD1306
#define LOVYAN_BUS     Bus_I2C
#define LOVYAN_I2C_BUS 1

#define TFT_I2C_PORT  0         // ESP32 I²C port 0
#define TFT_I2C_WRITE 400000    // 400 kHz fast-mode
#define TFT_I2C_READ  400000
#define TFT_SDA       GROVE_SDA // GPIO21
#define TFT_SCL       GROVE_SCL // GPIO22
#define TFT_ADDR      0x3C      // SSD1306/SH1106 default (0x3D if SA0 pulled HIGH)

// Panel geometry
#define TFT_CS       -1
#define TFT_RST      -1
#define TFT_BUSY_PIN -1
#define TFT_WIDTH    128
#define TFT_HEIGHT    64
#define TFT_OFFSET_X   0
#define TFT_OFFSET_Y   0
#define TFT_INVERTION  1
#define TFT_RGB_ORDER  0
#define TFT_MEM_WIDTH  128
#define TFT_MEM_HEIGHT  64

// ─── MISC ─────────────────────────────────────────────────────────────────────
#define BAD_TX GROVE_SDA
#define BAD_RX GROVE_SCL

#endif /* Pins_Arduino_h */
