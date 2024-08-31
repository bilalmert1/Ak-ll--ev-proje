#include <DHT11.h>
#include <LCD-I2C.h>

const int esikDegeri = 200;   // Gaz eşik değerini belirliyoruz.
const int buzzerPin = 9;      // Buzzer'ın takılacağı pin
const int ledPin = 13;        // LED'in bağlı olduğu pin numarasını belirtin
const int ledPin2 = 8;        // İkinci LED'in bağlı olduğu pin numarasını belirtin
const int ledPin3 = 7;        // Üçüncü LED'in bağlı olduğu pin numarasını belirtin
const int pirSensorPin = 10;  // Hareket sensörünün sinyal çıkış pinini belirtin
const int dht11Pin = 2;       // DHT11 sensörünün bağlı olduğu pin

int deger;                     // Gaz sensöründen okunan değer

bool motionDetected = false;   // Hareket algılandı mı?
unsigned long motionStartTime = 0;  // Hareketin başlangıç zamanını kaydet
const unsigned long motionDuration = 3000;  // Hareketin algılanma süresi (ms)
const unsigned long ledOnDuration = 3000;   // LED'in yanma süresi (ms)

DHT11 dht11(dht11Pin);
LCD_I2C lcd(0x27, 16, 2);  // Default address of most PCF8574 modules, change according

void setup() {
  pinMode(buzzerPin, OUTPUT); // Buzzer pinimizi çıkış olarak ayarlıyoruz.
  pinMode(ledPin, OUTPUT);    // Birinci LED pinini çıkış olarak ayarla
  pinMode(ledPin2, OUTPUT);   // İkinci LED pinini çıkış olarak ayarla
  pinMode(ledPin3, OUTPUT);   // Üçüncü LED pinini çıkış olarak ayarla
  pinMode(pirSensorPin, INPUT); // Hareket sensörü pinini giriş olarak ayarla
  lcd.begin();
  lcd.display();
  lcd.backlight();
  Serial.begin(9600); // Seri Port başlat
}

void loop() {
  // Gaz sensörü kodu
  deger = analogRead(A0); // Sensörden analog değer okuyoruz.
  if (deger > esikDegeri) { // Sensörden okunan değer eşik değerinden büyükse çalışır.
    tone(buzzerPin, 1000);  // Buzzer frekansını 1000 Hz olarak ayarla
    delay(100);
    noTone(buzzerPin);      // Buzzer'ı kapat
    delay(100);

    // İlk LED'i yak
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);

    // İkinci LED'i yak
    digitalWrite(ledPin2, HIGH);
    delay(1000);
    digitalWrite(ledPin2, LOW);

    // Üçüncü LED'i yak
    digitalWrite(ledPin3, HIGH);
    delay(1000);
    digitalWrite(ledPin3, LOW);
  } else { // Sensörden okunan değer eşik değerinin altındaysa çalışır.
    noTone(buzzerPin); // Buzzer'ı kapat
  }

  // Hareket sensörü kodu
  int motionSensorValue = digitalRead(pirSensorPin);
  if (motionSensorValue == HIGH && !motionDetected) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Hareket Algılandı! LED Açık");
    motionStartTime = millis();
    motionDetected = true;
  }

  if (motionDetected && (millis() - motionStartTime < motionDuration)) {
    motionSensorValue = digitalRead(pirSensorPin);
    if (motionSensorValue == HIGH) {
      motionStartTime = millis();
    }
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED Kapalı");
    motionDetected = false;
  }

  delay(100);

  // DHT11 sensörü kodu
  int temperature = dht11.readTemperature();
  int humidity = dht11.readHumidity();

  if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT &&
      humidity != DHT11::ERROR_CHECKSUM && humidity != DHT11::ERROR_TIMEOUT) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    lcd.print("Temperature: ");
    lcd.setCursor(12, 0);
    lcd.print(temperature);
    Serial.print("Humidity: ");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.setCursor(9, 1);
    lcd.print(humidity);
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    if (temperature == DHT11::ERROR_TIMEOUT || temperature == DHT11::ERROR_CHECKSUM) {
      Serial.print("Temperature Reading Error: ");
      Serial.println(DHT11::getErrorString(temperature));
    }
    if (humidity == DHT11::ERROR_TIMEOUT || humidity == DHT11::ERROR_CHECKSUM) {
      Serial.print("Humidity Reading Error: ");
      Serial.println(DHT11::getErrorString(humidity));
    }
  }

  delay(1000);
}