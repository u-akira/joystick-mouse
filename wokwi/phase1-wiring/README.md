# Phase 1配線確認用Wokwi図

## 重要な注意

WokwiにはUIAPduino Pro Micro CH32V003 V1.4の標準部品がないため、図ではArduino Nanoをピン配置の代替として使用している。

この図で確認できるのは、次の配線関係である。

- 5V
- GND
- A0（KY-023のVRX相当）
- A1（KY-023のVRY相当）
- D7（KY-023のSW、予備入力）
- D5（外付けタクトスイッチ）

実機のUIAPduinoとArduino Nanoでは、マイコンやUSB HIDの動作は同じではない。Wokwiでは、配線と入力値の確認用として使用する。

## OLED参考構成

I2C接続のSSD1306 OLEDを追加した参考図も用意している。

- [OLED参考配線図（diagram.oled-reference.json）](./diagram.oled-reference.json)
- [OLED参考スケッチ（sketch.oled-reference.ino）](./sketch.oled-reference.ino)
- [Wokwiライブラリ一覧](./libraries.txt)

この参考構成では、I2C OLEDを使うため、入力ピンを次のように変更している。

| 用途 | UIAPduino | WokwiのArduino Nano代替 |
|---|---|---|
| OLED SDA | D3 / PC1 | A4 / SDA |
| OLED SCL | D4 / PC2 | A5 / SCL |
| 外付けタクトスイッチ | D5 | D5 |
| KY-023押し込みスイッチ | D7 | D7 |

WokwiのArduino NanoではI2CがA4/A5であるため、UIAPduinoのD3/D4とは表示上のピン番号が異なる。実機ではArduinoコアの標準`Wire`設定により、SDAがD3、SCLがD4になる。

OLED参考図を表示する場合は、`diagram.oled-reference.json`の内容を`diagram.json`へ、`sketch.oled-reference.ino`の内容を`sketch.ino`へ置き換え、`libraries.txt`も同じプロジェクトへ配置する。

## 表示方法

1. WokwiでArduino Nanoの新規プロジェクトを開く
2. `diagram.json`の内容を、このフォルダーの`diagram.json`で置き換える
3. `sketch.ino`の内容を、このフォルダーの`sketch.ino`で置き換える
4. シミュレーションを開始する
5. ジョイスティックを操作し、Serial MonitorでX/Y値を確認する
6. 青いタクトスイッチを押すか、キーボードの`B`キーを押してButtonの状態を確認する
7. ジョイスティック中央を押すか、キーボードの`Space`キーを押してJoystickButtonの状態を確認する

## 実機との対応

| Wokwi | 実機 |
|---|---|
| Arduino Nano 5V | UIAPduino 5V |
| Arduino Nano GND | UIAPduino GND |
| Arduino Nano A0 | UIAPduino A0 |
| Arduino Nano A1 | UIAPduino A1 |
| Arduino Nano D7 | UIAPduino D7 |
| Arduino Nano D5 | UIAPduino D5 |
| Wokwi Analog Joystick VCC | KY-023 +5V |
| Wokwi Analog Joystick GND | KY-023 GND |
| Wokwi Analog Joystick HORZ | KY-023 VRX |
| Wokwi Analog Joystick VERT | KY-023 VRY |
| Wokwi Analog Joystick SEL | KY-023 SW |
| Wokwi Pushbutton | 2ピンタクトスイッチ |
