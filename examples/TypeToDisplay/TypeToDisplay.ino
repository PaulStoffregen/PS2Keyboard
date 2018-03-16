/* TypeToDisplay - PS2Keyboard keystrokes show on a standard LCD Display

  Lcd support added 18/2/2018 D.R.Patterson
    lcd character set:
  http://forum.arduino.cc/index.php?topic=19002.0

  Tested on a mega with a Keypad Shield for Arduino

  PS2Keyboard now requries both pins specified for begin()
  keyboard.begin(data_pin, irq_pin);

  Valid irq pins:
     Arduino Uno:  2, 3
     Arduino Due:  All pins, except 13 (LED)
     Arduino Mega: 2, 3, 18, 19, 20, 21
     Teensy 2.0:   All pins, except 13 (LED)
     Teensy 2.0:   5, 6, 7, 8
     Teensy 1.0:   0, 1, 2, 3, 4, 6, 7, 16
     Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
     Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
     Sanguino:     2, 10, 11

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

  Like the Original library and example this is under LGPL license.

  Modified by Cuninganreset@gmail.com on 2010-03-22
  Modified by Paul Stoffregen <paul@pjrc.com> June 2010
*/

#include <PS2Keyboard.h>
const int DataPin = 19;
const int IRQpin =  18;
PS2Keyboard keyboard;

#include <LiquidCrystal.h>
// LCD Keypad Shield for Arduino
// http://www.hobbytronics.co.uk/lcd/lcd-displays-5v/arduino-lcd-keypad-shield

//set constants for number of rows and columns to match your LCD
const int numRows = 2;
const int numCols = 16;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte x, y;  // track lcd position

// custom pound using https://omerk.github.io/lcdchargen/
byte customPound[8] = {
  0b01100,
  0b10010,
  0b01000,
  0b11100,
  0b01000,
  0b01000,
  0b11110,
  0b00000
};
const char pound = 1;

byte customkey[8] = { // ¬
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b00001,
  0b00001,
  0b00000
};
const char custom = 2;

byte customslash[8] = { // backslash
  0b00000,
  0b00000,
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00000
};
const char backslash = 3;

char c;

// currently un-used:
#define is_printable(c) (!(c&0x80))   // don't print if top bit is set

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(115200);
  while (!Serial) yield();
  lcd.begin(numRows, numCols);
  delay(250);
  lcd.clear();
  // create a new custom character
  lcd.createChar(pound, customPound);
  lcd.createChar(custom, customkey);
  lcd.createChar(backslash, customslash);
  lcd.cursor();   // Enable Cursor
  //lcd.blink();  // Blinking cursor
  lcd.clear();

  lcd.print(F("Keyboard Test:"));
  lcd.setCursor(0, 1);
  lcd.print(F("Esc to clear LCD"));
  Serial.println(F("Keyboard Test:"));
  unsigned long t = millis();
  do {
    if (keyboard.available()) {
      c = keyboard.read();
      if (c == PS2_ESC) break;
    }
  } while ((millis() - t) < 8000);
  lcd.clear();
}

void loop() {

  if (keyboard.available()) {
    // read the next key
    c = keyboard.read();

    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
      x = 0;
      y += 1;
      if (y == numRows) {
        y = 0;
        lcd.clear();
      }
      lcd.setCursor(x, y);

    } else if (c == PS2_TAB) {
      lcdprint(F("[Tab]"));

    } else if (c == PS2_ESC) {
      Serial.println(F("\n[ESC]\n"));
      lcd.clear();
      x = 0; y = 0;
      lcd.setCursor(x, y);

    } else if (c == PS2_PAGEDOWN) {
      lcdprint(F("[PgDn]"));

    } else if (c == PS2_PAGEUP) {
      lcdprint(F("[PgUp]"));

    } else if (c == PS2_LEFTARROW) {
      lcdprint(F("[Left]"));

    } else if (c == PS2_RIGHTARROW) {
      lcdprint(F("[Right]"));

    } else if (c == PS2_UPARROW) {
      lcdprint(F("[Up]"));

    } else if (c == PS2_DOWNARROW) {
      lcdprint(F("[Down]"));

    } else if (c == PS2_DELETE) {
      lcdprint(F("[Del]"));

    } else if (c == PS2_BACKSPACE) {
      lcdprint(F("[Back]"));

    } else {
      // otherwise, just print all normal characters
      lcdprintChar(c);
    }
  }
}

void lcdprintChar(char t) { // display char on lcd and Serial
  byte test = byte(t);
  if (test == 194) return; // this char appears as an indicator of special keys

  if (x > (numCols - 1) ) {
    x = 0;
    if (y == (numRows - 1)) {
      lcd.clear();
      Serial.println();
      y = 0;
    } else y += 1;
    Serial.println();
    lcd.setCursor(x, y);
  }

  if (test == 126) lcd.write(243);            // 126 ~
  else if (test == 163) lcd.write(pound);     // 163 £
  else if (test == 172) lcd.write(custom);    // 194 172 ¬
  else if (test == 92) lcd.write(backslash);  // 92 backslash
  else lcd.print(t);
  Serial.print(t);
  x += 1;
}

void lcdprint(String t) { // display string on lcd and Serial
  byte L = t.length();
  if (x > (numCols - L) ) {
    x = 0;
    if (y == (numRows - 1)) {
      lcd.clear();
      Serial.println();
      y = 0;
    } else y += 1;
    Serial.println();
    lcd.setCursor(x, y);
  }
  lcd.print(t);
  Serial.print(t);
  x += L;
}
