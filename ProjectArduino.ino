#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 3        
#define DHTTYPE DHT11   

#define RELAY_PIN 5     
#define FAN_PIN RELAY_PIN 
#define MIST_RELAY_PIN 6  

#define LCD_ADDR 0x27   
#define LCD_COLS 16     
#define LCD_ROWS 2      

DHT dht(DHTPIN, DHTTYPE);
float humidity, temperature;

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

void DisplayStatus(const String& status) {
  lcd.setCursor(0, 1);
  lcd.print(status);
  for (int i = status.length(); i < LCD_COLS; i++) {
    lcd.print(" ");
  }
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MIST_RELAY_PIN, OUTPUT); 

  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(humidity, 1);
  lcd.print("%");

  if (humidity < 60) { 
    digitalWrite(RELAY_PIN, HIGH); 
    digitalWrite(MIST_RELAY_PIN, HIGH); 
    DisplayStatus("Humidifying...");
  } else {
    digitalWrite(RELAY_PIN, LOW);  
    digitalWrite(MIST_RELAY_PIN, LOW); 
    DisplayStatus("Humidity OK");
  }

  delay(5000); 
}
