// UIAPduino Pocket 3D Mouse
//
// UIAPduino Pro Micro CH32V003 V1.4 用の Fusion 360 ビューポート操作スケッチ。
//
// 配線:
//   KY-023 VRX -> A0
//   KY-023 VRY -> A1
//   KY-023 SW  -> D7（予備入力。現在は未使用）
//   外付けタクトスイッチ -> D5 と GND
//
// 書き込み時のボード設定:
//   HID ProMicro CH32V003
//   Board Version: V1.4
//   USB: Keyboard+Mouse
//   Upload method: minichlink
//
// 起動時にジョイスティックの中央値を取得します。
// USB接続時はジョイスティックを中央に置いてください。

#include <Keyboard.h>
#include <Mouse.h>

constexpr uint8_t JOYSTICK_X_PIN = A0;
constexpr uint8_t JOYSTICK_Y_PIN = A1;
constexpr uint8_t JOYSTICK_BUTTON_PIN = 7;
constexpr uint8_t ACTION_BUTTON_PIN = 5;

// ジョイスティック中央付近の無視幅。ADC 0〜1023 の約10%。
constexpr int DEADZONE = 100;

// 10msごとに送る最大移動量。最初は安全側の速度にしている。
constexpr int MAX_MOTION = 8;
constexpr unsigned long UPDATE_INTERVAL_MS = 10;
constexpr unsigned long DEBOUNCE_MS = 25;
constexpr unsigned long HID_REPORT_SETTLE_MS = 20;

// Fusion 360で方向が逆に感じる場合は、該当値をtrueに変更する。
constexpr bool INVERT_X = false;
constexpr bool INVERT_Y = true;
constexpr bool SWAP_XY = true;

int centerX = 512;
int centerY = 512;

bool rawActionButton = HIGH;
bool stableActionButton = HIGH;
unsigned long actionButtonChangedAt = 0;

bool navigationActive = false;
bool orbitModeActive = false;
unsigned long lastUpdateAt = 0;

int readCenteredAxis(uint8_t pin) {
  long sum = 0;
  constexpr int SAMPLES = 16;

  for (int i = 0; i < SAMPLES; ++i) {
    sum += analogRead(pin);
    delay(2);
  }

  return static_cast<int>(sum / SAMPLES);
}

int axisToMotion(int rawValue, int centerValue, bool invert) {
  const int delta = rawValue - centerValue;
  const int magnitude = abs(delta);

  if (magnitude <= DEADZONE) {
    return 0;
  }

  // 中央から不感帯を超えた分だけを、左右のADC範囲に応じて速度へ変換する。
  const int availableRange = delta > 0
                               ? 1023 - centerValue - DEADZONE
                               : centerValue - DEADZONE;
  const int usableRange = max(1, availableRange);
  int motion = ((magnitude - DEADZONE) * MAX_MOTION) / usableRange;
  motion = min(MAX_MOTION, max(1, motion));

  if (delta < 0) {
    motion = -motion;
  }
  if (invert) {
    motion = -motion;
  }

  return motion;
}

void updateActionButton() {
  const bool currentRaw = digitalRead(ACTION_BUTTON_PIN);
  const unsigned long now = millis();

  if (currentRaw != rawActionButton) {
    rawActionButton = currentRaw;
    actionButtonChangedAt = now;
  }

  if (stableActionButton != rawActionButton &&
      now - actionButtonChangedAt >= DEBOUNCE_MS) {
    stableActionButton = rawActionButton;
  }
}

bool actionButtonPressed() {
  return stableActionButton == LOW;
}

void releaseNavigation() {
  if (navigationActive) {
    Mouse.release(MOUSE_MIDDLE);
    navigationActive = false;
  }

  if (orbitModeActive) {
    Keyboard.release(KEY_LEFT_SHIFT);
    orbitModeActive = false;
  }
}

void beginNavigation(bool orbitMode) {
  if (orbitMode) {
    Keyboard.press(KEY_LEFT_SHIFT);
    orbitModeActive = true;
    delay(HID_REPORT_SETTLE_MS);
  }

  Mouse.press(MOUSE_MIDDLE);
  navigationActive = true;
}

void changeNavigationMode(bool orbitMode) {
  if (orbitMode == orbitModeActive) {
    return;
  }

  releaseNavigation();
  if (!orbitMode) {
    delay(HID_REPORT_SETTLE_MS);
  }
  beginNavigation(orbitMode);
}

void setup() {
  pinMode(ACTION_BUTTON_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);

  // USB HIDを開始する。Keyboard+Mouseモードで書き込むこと。
  Keyboard.begin();
  Mouse.begin();

  // PC側のUSB列挙と、電源投入直後の入力安定を待つ。
  delay(500);

  // この間、ジョイスティックは中央に置く。
  centerX = readCenteredAxis(JOYSTICK_X_PIN);
  centerY = readCenteredAxis(JOYSTICK_Y_PIN);

  rawActionButton = digitalRead(ACTION_BUTTON_PIN);
  stableActionButton = rawActionButton;
  actionButtonChangedAt = millis();
  lastUpdateAt = millis();
}

void loop() {
  updateActionButton();

  const unsigned long now = millis();
  if (now - lastUpdateAt < UPDATE_INTERVAL_MS) {
    return;
  }
  lastUpdateAt = now;

  const int xMotion = axisToMotion(analogRead(JOYSTICK_X_PIN), centerX, INVERT_X);
  const int yMotion = axisToMotion(analogRead(JOYSTICK_Y_PIN), centerY, INVERT_Y);
  const bool moving = xMotion != 0 || yMotion != 0;

  if (!moving) {
    // 中央へ戻ったら、必ず中ボタンとShiftを解放する。
    releaseNavigation();
    return;
  }

  const bool orbitMode = actionButtonPressed();
  if (!navigationActive) {
    beginNavigation(orbitMode);
  } else {
    changeNavigationMode(orbitMode);
  }

  const int mouseXMotion = SWAP_XY ? yMotion : xMotion;
  const int mouseYMotion = SWAP_XY ? xMotion : yMotion;
  Mouse.move(mouseXMotion, mouseYMotion);
}
