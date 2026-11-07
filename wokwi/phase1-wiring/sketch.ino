// Wokwi用のPhase 1配線確認スケッチです。
// Arduino NanoをUIAPduinoのピン配置の代替として使用しています。
// 実機のUIAPduinoでは、シリアル出力ではなく別の確認方法が必要です。

const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;
const int JOYSTICK_BUTTON_PIN = 7;
const int BUTTON_PIN = 5;

int centerX = 512;
int centerY = 512;

void setup() {
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);

  delay(300);
  centerX = analogRead(JOYSTICK_X_PIN);
  centerY = analogRead(JOYSTICK_Y_PIN);

  Serial.println("Phase 1 wiring test");
  Serial.print("centerX = ");
  Serial.println(centerX);
  Serial.print("centerY = ");
  Serial.println(centerY);
  Serial.println("Button: press the blue button or press B");
  Serial.println("Joystick button: press the joystick center or Space");
}

void loop() {
  const int x = analogRead(JOYSTICK_X_PIN);
  const int y = analogRead(JOYSTICK_Y_PIN);
  const int button = digitalRead(BUTTON_PIN);
  const int joystickButton = digitalRead(JOYSTICK_BUTTON_PIN);

  Serial.print("X=");
  Serial.print(x);
  Serial.print(" Y=");
  Serial.print(y);
  Serial.print(" Button=");
  Serial.println(button == LOW ? "PRESSED" : "RELEASED");
  Serial.print(" JoystickButton=");
  Serial.println(joystickButton == LOW ? "PRESSED" : "RELEASED");

  delay(200);
}
