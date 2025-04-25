/*  M5Stack_Byte_Switch_Byte_Button_Single_Oled_Combined.ino
* ESP32-CYD (ESP32 Dev Module)
* Grove 3 Port Hub - Breadboard would do to daisy chain i2c bus
* 
* M5Unified V0.2.5
* M5UnitOLED from M5GFX Library V0.2.6
* Arduino IDE 2.3.2
* Boards Manager - ESP32 by Espressif V3.1.0
* Iain MacMillan
* 25/04/2025
*/
#include "unit_byte.hpp"
#include <M5Unified.h>
#include <Wire.h>
#include <M5UnitOLED.h>

UnitByte deviceSwitch;  // ByteSwitch at 0x46
UnitByte deviceButton;  // ByteButton at 0x47

const uint8_t switchId = 0x46;
const uint8_t buttonId = 0x47;

M5UnitOLED display(27, 22, 400000); // SDA=27, SCL=22

void setup() {
    Serial.begin(115200);
    Wire.begin(27, 22);
      // Initialize OLED
    display.init();
    display.setRotation(3); 
    display.setFont(&fonts::Font0);   // Pixel Height 8                   https://m5stack.lang-ship.com/howto/m5gfx/font/
    //display.setFont(&fonts::Font2);   // Pixel Height 16
    //display.setFont(&fonts::Font4);   // Pixel Height 26
    //display.setFont(&fonts::FreeMono9pt7b);   // Pixel Height 9
    //display.setFont(&fonts::FreeMono12pt7b);   // Pixel Height  12
    //display.setFont(&fonts::FreeMono18pt7b;   // Pixel Height 18
    //display.setFont(&fonts::Orbitron_Light_32);   // Pixel Height 32
    //display.setFont(&fonts::TomThumb);   // Pixel Height  6
    display.setTextSize(0);
    deviceSwitch.begin(&Wire, switchId, 27, 22, 400000);
    deviceSwitch.setLEDShowMode(BYTE_LED_MODE_DEFAULT);
    for (int i = 0; i < 8; i++) {
        deviceSwitch.setSwitchOffRGB888(i, 0xFF0000);
        deviceSwitch.setSwitchOnRGB888(i, 0x00FF00);
    }
    deviceSwitch.setFlashWriteBack();
    deviceButton.begin(&Wire, buttonId, 27, 22, 400000);
    deviceButton.setLEDShowMode(BYTE_LED_MODE_DEFAULT);
    for (int i = 0; i < 8; i++) {
        deviceButton.setSwitchOffRGB888(i, 0xFF0000);
        deviceButton.setSwitchOnRGB888(i, 0x0000FF);
    }
    deviceButton.setFlashWriteBack();    
}

void OLEDprint_ByteSwitchByteButton() {
  // Create status strings
  String switchStatus = "";
  String buttonStatus = "";
  // Read switch and button states
  for (uint8_t i = 0; i < 8; i++) {
    // Get switch state (0 or 1)
    switchStatus += deviceSwitch.getSwitchStatus(i) ? "1 " : "0 ";
    
    // Get inverted button state (1 when pressed, 0 when released)
    uint8_t btnState = 1 - deviceButton.getSwitchStatus(i);
    buttonStatus += btnState ? "1 " : "0 ";
  }  
  
  // OLED Display
  display.startWrite();
  display.clear();
  // Switch status display
  display.setCursor(0, 0);
  display.setFont(&fonts::Font2);
  display.print("    BYTE-SWITCH");

  display.setCursor(0, 20);
  display.setFont(&fonts::Font0);
  display.print(switchStatus);

  // Button status display
  display.setCursor(0, 32);
  display.setFont(&fonts::Font2);
  display.print("    BYTE-BUTTON");

  display.setCursor(0, 52);
  display.setFont(&fonts::Font0);
  display.print(buttonStatus);
 
  display.endWrite();
  delay(5); // Display On OLED 
}


void Serialprint_ByteSwitch() {
    // Read Switch statuses (include I2C address)
    Serial.printf("Switch Statuses (I2C 0x%02X): 0=Off, 1=On:\n", switchId);
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t status = deviceSwitch.getSwitchStatus(i);
        Serial.printf("SW%d: %d\t", i, status);
    }
    Serial.println();  
}

void Serialprint_ByteButton() {
    // Read Button statuses (include I2C address)
    Serial.printf("Button Statuses (I2C 0x%02X, INVERTED): 1=Pressed, 0=Released:\n", buttonId);
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t rawStatus = deviceButton.getSwitchStatus(i);
        uint8_t invertedStatus = 1 - rawStatus;
        Serial.printf("BTN%d: %d\t", i, invertedStatus);
    }
    Serial.println("\n-----------------------");
}

void loop() {
    Serialprint_ByteSwitch();
    Serialprint_ByteButton();
    OLEDprint_ByteSwitchByteButton();
    
}


