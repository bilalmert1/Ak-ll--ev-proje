#include <DHT11.h>
#include <LCD-I2C.h>

const int esikDegeri = 200;  
const int buzzerPin = 9;      
const int ledPin = 13;        
const int ledPin2 = 8;        
const int ledPin3 = 7;        
const int pirSensorPin = 10;  
const int dht11Pin = 2;       

int deger;                    

bool motionDetected = false;   
unsigned long motionStartTime = 0; 
const unsigned long motionDuration = 3000;  
const unsigned long ledOnDuration = 3000;  

DHT11 dht11(dht11Pin);
LCD_I2C lcd(0x27, 16, 2); 

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);    
  pinMode(ledPin2, OUTPUT);  
  pinMode(ledPin3, OUTPUT);   
  pinMode(pirSensorPin, INPUT);
  lcd.begin();
  lcd.display();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  
  deger = analogRead(A0); 
  if (deger > esikDegeri) { 
    tone(buzzerPin, 1000);  
    delay(100);
    noTone(buzzerPin);      
    delay(100);

    
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);

    
    digitalWrite(ledPin2, HIGH);
    delay(1000);
    digitalWrite(ledPin2, LOW);

    
    digitalWrite(ledPin3, HIGH);
    delay(1000);
    digitalWrite(ledPin3, LOW);
  } else { 
    noTone(buzzerPin);
  }

 
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
