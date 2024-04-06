#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 3        // DHT11 感测器连接至 Arduino 的脚位 3
#define DHTTYPE DHT11   // 使用 DHT11 感测器

#define RELAY_PIN 5     // 继电器连接至 Arduino 的脚位 5
#define FAN_PIN RELAY_PIN // 风扇连接至继电器的脚位，与继电器相同
#define MIST_PIN 7      // 水雾化器连接至 Arduino 的脚位 7

#define LCD_ADDR 0x27   // I2C 接口 LCD 的地址
#define LCD_COLS 16     // LCD 的列数
#define LCD_ROWS 2      // LCD 的行数

DHT dht(DHTPIN, DHTTYPE);
float humidity, temperature;

// 创建 I2C 接口的 LCD 对象
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

void DisplayStatus(const String& status) {
  lcd.setCursor(0, 1);
  lcd.print(status);
  // 填充剩余空间以清除之前的显示内容
  for (int i = status.length(); i < LCD_COLS; i++) {
    lcd.print(" ");
  }
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(MIST_PIN, OUTPUT);

  //初始化温湿度感测器
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // 更新 LCD 显示湿度和温度
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(humidity, 1);
  lcd.print("%");

  // 检查是否需要加湿，根据相对湿度
  if (humidity < 60) { // 如果相对湿度低于60%，则开始加湿
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(MIST_PIN, HIGH); 
    digitalWrite(FAN_PIN, HIGH);
    DisplayStatus("Humidifying...");
  }
  else {
    digitalWrite(RELAY_PIN, LOW); 
    digitalWrite(MIST_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
    DisplayStatus("Humidity OK");
  }
  
  delay(5000); // 5秒后重新检测
}
