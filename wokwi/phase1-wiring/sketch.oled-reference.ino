// OLED追加時の配線確認用Wokwiスケッチです。
// WokwiではArduino NanoのA4/A5をI2Cに使用します。
// UIAPduinoの標準Wire設定では、SDA=D3、SCL=D4です。

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;
const int JOYSTICK_BUTTON_PIN = 7;
const int BUTTON_PIN = 5;

void setup() {
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true) {
      delay(1000);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Pocket 3D Mouse");
  display.println("OLED reference");
  display.display();
  delay(500);
}

void loop() {
  const int x = analogRead(JOYSTICK_X_PIN);
  const int y = analogRead(JOYSTICK_Y_PIN);
  const bool joystickPressed = digitalRead(JOYSTICK_BUTTON_PIN) == LOW;
  const bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Pocket 3D Mouse");
  display.print("X: ");
  display.println(x);
  display.print("Y: ");
  display.println(y);
  display.print("Joy: ");
  display.println(joystickPressed ? "PRESSED" : "RELEASED");
  display.print("Btn: ");
  display.println(buttonPressed ? "PRESSED" : "RELEASED");
  display.display();

  delay(100);
}
