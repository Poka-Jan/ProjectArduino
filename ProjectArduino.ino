#include "DHT.h"
#include <LiquidCrystal.h>

#define DHTPIN 2          // DHT11連接到Arduino的數位引腳2
#define DHTTYPE DHT11     // 使用的DHT型號
#define FAN_PIN 3         // 風扇連接到引腳3
#define MIST_PIN 4        // 水霧化器連接到引腳4
#define LCD_RS 7          // LCD的RS接腳
#define LCD_EN 8          // LCD的EN接腳
#define LCD_D4 9          // LCD的D4接腳
#define LCD_D5 10         // LCD的D5接腳
#define LCD_D6 11         // LCD的D6接腳
#define LCD_D7 12         // LCD的D7接腳

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(FAN_PIN, OUTPUT);
  pinMode(MIST_PIN, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  // 讀取溫度和濕度
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // 檢查讀取是否成功
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.print("Read Error!");
    return;
  }

  // 顯示溫度和濕度到LCD
  lcd.setCursor(0, 0);
  lcd.print("Hum:");
  lcd.print(h);
  lcd.print("% Temp:");
  lcd.print(t);
  lcd.print((char)223); // 顯示攝氏度符號
  lcd.print("C");

  // 判斷是否過熱
  if(t > 30 && h < 50) {
    Serial.println("Overheat detected, activating mister and fan.");
    lcd.setCursor(0, 1);
    lcd.print("Overheat! Cooling..");
    digitalWrite(MIST_PIN, HIGH);
    delay(1000);
    digitalWrite(MIST_PIN, LOW);
    digitalWrite(FAN_PIN, HIGH);
    delay(5000);
    digitalWrite(FAN_PIN, LOW);
  } else {
    // 不過熱時關閉水霧化器和風扇
    digitalWrite(MIST_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
  }

  delay(2000); // 每2秒讀取一次
}
