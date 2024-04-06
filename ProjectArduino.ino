#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 3        // DHT11 感測器連接至 Arduino 的腳位 3
#define DHTTYPE DHT11   // 使用 DHT11 感測器

#define RELAY_PIN 5     // 繼電器連接至 Arduino 的腳位 5
#define FAN_PIN RELAY_PIN // 風扇連接至繼電器的腳位，與繼電器相同
#define MIST_PIN 7      // 水霧化器連接至 Arduino 的腳位 4

#define LCD_ADDR 0x27   // I2C 介面 LCD 的地址
#define LCD_COLS 16     // LCD 的列數
#define LCD_ROWS 2      // LCD 的行數

DHT dht(DHTPIN, DHTTYPE);
float humidity, temperature;

// 創建 I2C 介面的 LCD 物件
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
void DisplayEnvironmentVariable(float humidity, float temperature)
{
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("                         ");
  lcd.setCursor(0, 1);
  lcd.print("Temperature:             ");
  lcd.print(temperature);
  lcd.print("                         ");
}



void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(MIST_PIN, OUTPUT);

  //初始化溫濕度感測器
  dht.begin();
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  lcd.init();
  lcd.backlight();
  DisplayEnvironmentVariable(humidity, temperature);
}

void loop() {
  // if (isnan(humidity) || isnan(temperature)) {
  //   lcd.clear();
  //   lcd.print("Error reading data.");
  //   delay(2000); 
  // }

  // 檢查是否需要加濕 標準我亂訂的
  if (humidity < 60 || temperature > 26) { 
    digitalWrite(RELAY_PIN, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humidifying...");
    delay(5000); 
    digitalWrite(MIST_PIN, HIGH); 
    digitalWrite(FAN_PIN, HIGH);

  }
  else {
    digitalWrite(RELAY_PIN, LOW); 
    digitalWrite(MIST_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
    lcd.setCursor(0, 0);
    lcd.print("0_0");
    delay(5000);
  }
}