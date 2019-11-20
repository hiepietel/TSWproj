#include <OneWire.h>
#include <Wire.h>   // standardowa biblioteka Arduino
#include <LiquidCrystal_I2C.h> // dolaczenie pobranej biblioteki I2C dla LCD
#include <math.h>

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;    
const int pinTempSensor = A0;

#define ONE_WIRE_BUS 2

#define red 3
#define green 5
#define blue 6
 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x27

OneWire oneWire(ONE_WIRE_BUS); 
 
void setup()  
{
  //led
    pinMode(red, OUTPUT); // Piny, podłączone do diody jako wyjścia
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);// Świecimy tylko na zielono
  //lcd
  lcd.begin(16,2);   // Inicjalizacja LCD 2x16
  
  lcd.backlight(); // zalaczenie podwietlenia 
  lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
  lcd.print("A-R|B-G|C-B");
  delay(500);
  lcd.setCursor(0,1); //Ustawienie kursora w pozycji 0,0 (drugi wiersz, pierwsza kolumna)
  lcd.print("");
  
  Serial.begin(9600);
}
 int refreshTemp = 0;
void loop() 
{
  //red temp

  
    if(refreshTemp > 24){
            int a = analogRead(pinTempSensor);

          float R = 1023.0/a-1.0;
          R = R0*R;

      float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
      
          String tempStr =  "Temp: ";
          tempStr += temperature;
          tempStr += " C";
          lcd.setCursor(0,0); 
          lcd.print(tempStr);
          refreshTemp =0;
    }
  refreshTemp += 1;
  //end of read
  if (digitalRead(9) == HIGH) {  // Button A pressed
    digitalWrite(red, HIGH);
    lcd.setCursor(0,1); 
    lcd.print("A: RED enabled");
    Serial.print("RED enabled");
   delay(5);
  }
    if (digitalRead(11) == HIGH) {  // Button B pressed
    digitalWrite(green, HIGH);
    lcd.setCursor(0,1); 
    lcd.print("B: GREEN enabled");
    Serial.print("GREEN enabled");
   delay(5);
  }
    if (digitalRead(10) == HIGH) {  // Button C pressed
    digitalWrite(blue, HIGH);
    lcd.setCursor(0,1); 
    lcd.print("C: BLUE enabled");
    Serial.print("BLUE enabled");
   delay(5);
  }
  if (digitalRead(8) == HIGH) {  // Button D pressed
    digitalWrite(blue, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    lcd.setCursor(0,1); 
    lcd.print("D: COLORS disabled");
    Serial.print("COLORS disabled");
   delay(5);
  }
  lcd.backlight(); // zalaczenie podswietlenia
   
  // lcd.setCursor(0,1); 
  //  lcd.print("");
//lcd.noBacklight(); // wylaczenie podswietlenia
   //delay(5000);
}
