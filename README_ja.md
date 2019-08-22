Parallax FeedBack 360 Servo Control Library 4 Arduino
=====

このライブラリは[Parallax FeedBack 360° High Speed Servo](https://www.parallax.com/product/900-00360)の制御を容易にします。このサーボモーターはPWM信号によるフィードバックが行われます。このライブラリを使用して、サーボの回転角度を制御できる他、サーボの回転角度を読み取ることができます。

English README is [`README.md`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/README.md).

## サポート
[`SUPPORTED.md`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/SUPPORTED.md)をご覧ください。

## インストール方法
[Release](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/releases)よりzipファイルをダウンロードし、Arduino IDEのメニューにある`スケッチ`→`ライブラリをインクルード`→`.zip形式のライブラリをインストール`を選択してダウンロードしたzipファイルをインストールします。

## ピン番号
フィードバック信号ピンは外部割り込みができるピンを指定し、そこにサーボモーターのフィードバック信号線を割り当ててください。
制御ピンにはサーボモーターの制御線を割り当てたピンを指定してください。

## サンプル
このライブラリにはいくつかのサンプルが含まれており、`ファイル`→`スケッチ例`より確認することができます。
### Rotation
```arduino
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
    // サーボモータを1秒ごとに270度、-180度回転(+-4度の誤差を含む)    servo.rotate(270, 4);
    delay(1000);
    servo.rotate(-180, 4);
    delay(1000);
}

```
### Read
```arduino
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
    Serial.println(servo.Angle());
}
```

## ライセンス
このライブラリはMITライセンスに基づいて配布させています。
詳しくは[`LICENSE`](https://github.com/HyodaKazuaki/Parallax-FeedBack-360-Servo-Control-Library-4-Arduino/blob/master/LICENSE)をご覧ください。