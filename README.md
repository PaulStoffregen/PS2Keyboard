## PS/2 Keyboard Library

PS2Keyboard allows you to use a keyboard for user input. 

Full documentation: http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

![PS/2 keyboard port plugged into Arduino board](http://www.pjrc.com/teensy/td_libs_PS2Keyboard.jpg)

# Basic Usage

```c++
PS2Keyboard keyboard;
```
Create the keyboard object. Even though you could create multiple objects, only a single PS2 keyboard is supported by this library.

```c++
keyboard.begin(DataPin, IRQpin)
```
Begin receiving keystrokes. `DataPin` and `IRQpin` are the pin numbers to which you connected the PS/2 keyboard's Data and Clock signals.

On most Arduinos, the `IRQpin` must be a pin capable of being used with an interrupt (for example, the Arduino Uno only supports using pins 2 and 3 - INT0 and INT1 - for this purpose). If you use a non-interrupt pin for this, nothing will seem to happen.

```c++
keyboard.available()
```
Returns `true` if at least one keystroke has been received.

```c++
keyboard.read()
```
Read the next keystroke. `-1` is returned if no keystrokes have been received. Keystrokes are returned as ASCII characters; special keys are mapped to their control characters.

# Sample Program
```c++
#include <PS2Keyboard.h>

const int DataPin = 8;
const int IRQpin =  5;

PS2Keyboard keyboard;

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");
}

void loop() {
  if (keyboard.available()) {
    
    // read the next key
    char c = keyboard.read();
    
    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
    } else if (c == PS2_TAB) {
      Serial.print("[Tab]");
    } else if (c == PS2_ESC) {
      Serial.print("[ESC]");
    } else if (c == PS2_PAGEDOWN) {
      Serial.print("[PgDn]");
    } else if (c == PS2_PAGEUP) {
      Serial.print("[PgUp]");
    } else if (c == PS2_LEFTARROW) {
      Serial.print("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      Serial.print("[Right]");
    } else if (c == PS2_UPARROW) {
      Serial.print("[Up]");
    } else if (c == PS2_DOWNARROW) {
      Serial.print("[Down]");
    } else if (c == PS2_DELETE) {
      Serial.print("[Del]");
    } else {
      
      // otherwise, just print all normal characters
      Serial.print(c);
    }
  }
}
```
