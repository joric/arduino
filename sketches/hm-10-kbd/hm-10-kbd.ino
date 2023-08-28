// Turning HM-10, HM-11 into Bluetooth HID modules
// see http://geekhack.org/index.php?topic=62236
// pushbutton attached to GND/D3
// HM-10 pins connected to Arduino Pro Micro as RXD to TX0, TXD to RX1
// see keycodes at https://github.com/rampadc/cc254x-hidKbdM/blob/master/Example/BLE_KBD_Test/KBD_HUT.h
// Imgur: https://imgur.com/a/KWmz6

#define hutLeftGUI 0xE3
#define BT Serial1

void cmd(const char * s, int timeout=100, bool bSendCR=true) {
  Serial.write("> ");
  Serial.write(s);
  Serial.write("\r\n");

  BT.write(s);
  if (bSendCR) {
    BT.write("\r\n");
  }

  // serial output is tricky, let's use read timeout for now
  long t = millis();
  while(millis() < t + timeout) {
    while (BT.available()) {
      Serial.write(BT.read());
    }
    delay(1);
  }
}

void keyCommand(uint8_t modifiers, uint8_t key1, uint8_t key2 = 0, uint8_t key3 = 0, uint8_t key4 = 0, uint8_t key5 = 0, uint8_t key6 = 0) {
  char buf[16];
  sprintf(buf, "%s%c", modifiers==0x80 ? "KD" : "KU", key1);  
  cmd(buf);
  cmd("KUPDATE");
}

void setup() {  
  Serial.begin(115200); // This pipes to the serial monitor  
  BT.begin(57600); // This is the UART, attached to HM-10   
  pinMode(3, INPUT); // input mode for the button pin (D3)
  digitalWrite(3, HIGH); // set button pin to HIGH (software pullup) so it triggers when pulled to GND
}

bool wasPressed = false;
void loop() {
  int keyCode = hutLeftGUI;  // winkey
  bool pressed = digitalRead(3)==LOW;
  if (pressed != wasPressed) {
    keyCommand(pressed ? 0x80 : keyCode, keyCode);
    wasPressed = pressed;
    delay(25);
  }
}

