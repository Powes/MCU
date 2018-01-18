## 快速入门
Arduino 语言是以setup()开头，loop()作为主体的一个程序构架。
- setup()的：用来初始化变量，管脚模式，调用库函数等等，此函数只运行一次。
- loop()函数是一个循环函数，
---
##### Digital Output数字输出实验
功能：使 PIN13脚上的 LED 闪烁。

```
int ledPin = 13; // 设定控制LED 的数字IO 脚
void setup()
{
    pinMode(ledPin, OUTPUT); // 设定数字IO 口的模式，OUTPUT 为输出
}
void loop()
{
    digitalWrite(ledPin, HIGH); // 设定PIN13 脚为HIGH = 4V
    delay(1000); // 设定延时时间，1000 = 1 秒
    digitalWrite(ledPin, LOW); // 设定PIN13 脚为LOW = 0V
    delay(1000); //设定延时时间
}
```
---
##### PWM使 PIN11脚上的 LED逐渐变亮逐渐变暗。

```
int ledPin = 11; // 设定控制LED 的数字IO 脚
int val; //定义一个变量
void setup()
{
    pinMode(ledPin, OUTPUT); // 设定数字IO 口的模式，OUTPUT 为输出
}
void loop()
{
    for(val=0;val<255;val++) //变量循环+1
    {
        analogWrite(ledPin, val); //PWM 输出
        delay(50); //设定延时时间
    }
    for(val=255;val>0;val--) //变量循环-1
    {
        analogWrite(ledPin, val);
        delay(50);
    }
}
```
---
##### 闭合开关 SW，PIN13 脚上的 LED 亮，断开开关 SW，PIN13 脚上的 LED 灭。（数字输入）

```
int ledPin = 13; // 设定控制LED 的数字IO 脚
int switchPin = 7; // 设定开关的数字IO 脚
int val = 0; //定义一个变量
void setup()
{
    pinMode(ledPin, OUTPUT); // 设定数字IO 口的模式，OUTPUT 为输出
    pinMode(switchPin, INPUT); // 设定数字IO 口的模式，INPUT 为输入
}
void loop()
{
    val = digitalRead(switchPin); //读数字IO 口上的状态
    if (HIGH == val) digitalWrite(ledPin, LOW); // 如果开关断开，LED 灭
    else digitalWrite(ledPin, HIGH); // 如果开关闭合，LED 亮
}
```
---
##### 调节电位器 RPot，使输入模拟量的变化来改变 LED 闪烁的频率。(模拟输入)

```
int ledPin = 13; // 设定控制LED 的数字IO 脚
int RPotPin = 5; // 设定模拟输入IO 脚
int val = 0; //定义一个变量
void setup()
{
pinMode(ledPin, OUTPUT); // 设定数字IO 口的模式，OUTPUT 为输出
}
void loop()
{
val = analogRead(RPotPin); //读模拟IO 口上的数据
digitalWrite(ledPin, HIGH); // 设定PIN13 脚为HIGH = 4V
delay(value); // 延时时间为模拟输入的值
digitalWrite(ledPin, LOW); // 设定PIN13 脚为LOW
delay(value); // 延时时间为模拟输入的值
}
```
---
##### 串口通讯
Arduino 语言中还提供了Serial.available() 判断串口缓冲器状态、Serial.read()读串口、Serial.print()串
口发送及Serial.println()带换行符串口发送四个函数。
下面我们用一段代码来演示这些函数的用途。实验无须外围电路，只需要将下载的USB 线连接即可。

```
char word;
void setup()
{
    Serial.begin(9600); // 打开串口，设置波特率为9600 bps
}
void loop()
{
    if (Serial.available() > 0) //判断串口缓冲器是否有数据装入
    {
        word = Serial.read(); //读取串口
        if(word=='a') //判断输入的字符是否为a
        {
            Serial.print("Robot "); //从串口发送字符串
            Serial.println("is NO.1"); //从串口发送字符串并换行
        }
    }
}
```
