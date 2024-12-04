#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2      // Пин, к которому подключен датчик DHT11
#define DHTTYPE DHT11 // Тип датчика
#define RELAY_PIN 3   // Пин для реле/сервопривода

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Адрес LCD экрана

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  float h = dht.readHumidity(); // Чтение влажности
  float t = dht.readTemperature(); // Чтение температуры
  
  // Проверка на ошибки считывания
  if (isnan(h) || isnan(t)) {
    Serial.println("Ошибка считывания с DHT!");
    return;
  }

  // Отображение данных на LCD
  lcd.setCursor(0, 0);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print("% T: ");
  lcd.print(t);
  lcd.print("C");

  // Управление увлажнителем/осушителем
  if (h < 40) { // Увлажнитель включается при влажности ниже 40%
    digitalWrite(RELAY_PIN, HIGH); // Включить увлажнитель
    lcd.setCursor(0, 1);
    lcd.print("Humidifier ON ");
  } else if (h > 60) { // Осушитель включается при влажности выше 60%
    digitalWrite(RELAY_PIN, LOW); // Выключить увлажнитель
    lcd.setCursor(0, 1);
    lcd.print("Dehumidifier ON");
  } else {
    digitalWrite(RELAY_PIN, LOW); // Оба устройства выключены
    lcd.setCursor(0, 1);
    lcd.print("Devices OFF   ");
  }

  delay(2000); // Задержка перед следующим циклом
}
