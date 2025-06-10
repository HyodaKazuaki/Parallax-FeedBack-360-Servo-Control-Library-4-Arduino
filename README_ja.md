# Parallax FeedBack 360 Servo Control Library 4 Arduino

このライブラリは[Parallax FeedBack 360° High Speed Servo](https://www.parallax.com/product/900-00360)の制御を容易にします。このサーボモーターは PWM 信号によるフィードバックが行われます。このライブラリを使用して、サーボの回転角度を制御できる他、サーボの回転角度を読み取ることができます。

English README is [`README.md`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/README.md).

## お知らせ

このライブラリは、メジャーバージョン 2.0.0 以降、非同期制御と複数のサーボモーター制御のため破壊的変更を導入しました。
詳細は関数定義や例をご確認ください。

## サポート

[`SUPPORTED.md`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/SUPPORTED.md)をご覧ください。

## インストール方法

[Release](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/releases)より zip ファイルをダウンロードし、Arduino IDE のメニューにある`スケッチ`→`ライブラリをインクルード`→`.zip形式のライブラリをインストール`を選択してダウンロードした zip ファイルをインストールします。

## ピン番号

フィードバック信号ピンは外部割り込みができるピンを指定し、そこにサーボモーターのフィードバック信号線を割り当ててください。
制御ピンにはサーボモーターの制御線を割り当てたピンを指定してください。

## サンプル

このライブラリにはいくつかのサンプルが含まれており、`ファイル`→`スケッチ例`より確認することができます。

### Rotation

```cpp
#include "FeedBackServo.h"
// フィードバック信号ピンとサーボモータ制御ピンを定義
#define FEEDBACK_PIN 2
#define SERVO_PIN 3

// フィードバック信号ピンをセット
FeedBackServo servo = FeedBackServo(FEEDBACK_PIN);

void setup() {
    // サーボモータ制御ピンをセット
    servo.setServoControl(SERVO_PIN);
    // P制御用のKp値をセット
    servo.setKp(1.0);
}

void loop() {
    // サーボモータを1秒ごとに270度、-180度回転(+-4度の誤差を含む)
    servo.setTarget(270);
    servo.update(4);
    delay(1000);
    servo.setTarget(-180);
    servo.update(4);
    delay(1000);
}

```

### Read

```cpp
#include "FeedBackServo.h"
// フィードバック信号ピンを定義
#define FEEDBACK_PIN 2

// フィードバック信号ピンをセット
FeedBackServo Servo = FeedBackServo(FEEDBACK_PIN);

void setup() {
    // シリアル通信を115200bpsで開始
    Serial.begin(115200);
}

void loop() {
    Serial.print("Now angle: ");
    Serial.println(servo.getAngle());
}
```

### Multi motor controls

```cpp
#include "FeedBackServo.h"
// フィードバック信号ピンを定義
#define FEEDBACK_PIN1 2
#define FEEDBACK_PIN2 3
// サーボモータ制御ピンを定義
#define SERVO_PIN1 9
#define SERVO_PIN2 10

// フィードバック信号ピンをセット
FeedBackServo servo1 = FeedBackServo(FEEDBACK_PIN1);
FeedBackServo servo2 = FeedBackServo(FEEDBACK_PIN2);

void setup()
{
    Serial.begin(115200);

    servo1.setServoControl(SERVO_PIN1);
    servo2.setServoControl(SERVO_PIN2);

    servo1.setTarget(300);
    servo2.setTarget(300);

    servo1.setKp(0.5);
    servo2.setKp(0.5);
}

void loop()
{
    servo1.update();
    servo2.update();

    Serial.print(servo1.getAngle());
    Serial.print(" / ");
    Serial.println(servo2.getAngle());
}
```

## ライセンス

このライブラリは MIT ライセンスに基づいて配布させています。
詳しくは[`LICENSE`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/LICENSE)をご覧ください。
