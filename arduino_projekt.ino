#include <OneWire.h>
#include <Wire.h>   // standardowa biblioteka Arduino
#include <LiquidCrystal_I2C.h> // dolaczenie pobranej biblioteki I2C dla LCD
#include <math.h>

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;    
const int pinTempSensor = A0;


const int pinFotoSensor = A2;


#define ONE_WIRE_BUS 2

#define red 3
#define green 5
#define blue 6

#define stateOne 12 //temp
#define stateTwo 13 //LedControl

//temp variales
int state = 0;
// state == 1 led zapala sie gdy temp lub swiatlo przekrocza wartosci progowe
// state == 2 zmiana led pilotem
//state == 3 ustawienia wart progowych
 int refreshTemp = 0;
float maxTemp = 25.0;
 float minTemp = 15.0;
 int maxFoto = 700;
 int minFoto = 50;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x27

OneWire oneWire(ONE_WIRE_BUS); 
 
void setup()  
{
  //led
  pinMode(red, OUTPUT); // Piny, podłączone do diody jako wyjścia
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  //Pin info
  pinMode(stateOne, OUTPUT);
  pinMode(stateTwo, OUTPUT);
  digitalWrite(stateOne, LOW);
  digitalWrite(stateTwo, LOW);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);// Świecimy tylko na zielono
  //lcd
  lcd.begin(16,2);   // Inicjalizacja LCD 2x16
  lcd.backlight(); // zalaczenie podwietlenia 
  lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
  lcd.print("Welcome!");
  delay(500);
  lcd.setCursor(0,1); //Ustawienie kursora w pozycji 0,0 (drugi wiersz, pierwsza kolumna)
  lcd.print("Feel free to use");
}
void ClearDisplay(bool delayy){
  lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
  lcd.print("                ");
   if(delayy)delay(200);
  lcd.setCursor(0,1); //Ustawienie kursora w pozycji 0,0 (drugi wiersz, pierwsza kolumna)
  lcd.print("                ");
  if(delayy) delay(200);
}
void LEDon(bool off, bool r, bool g, bool b){
   if(off){
   digitalWrite(red, LOW);
   digitalWrite(green, LOW);
   digitalWrite(blue, LOW);
   
   }if(r)
     digitalWrite(red, HIGH);
   if(g)
    digitalWrite(green, HIGH);
   if(b)
   digitalWrite(blue, HIGH);
}
void SetDisplay(bool emptyDisplay,bool delayy, String lineOne, String lineTwo){
  if (emptyDisplay) ClearDisplay(delayy);
  lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
  lcd.print(lineOne);
  if(delayy) delay(200);
  lcd.setCursor(0,1); //Ustawienie kursora w pozycji 0,0 (drugi wiersz, pierwsza kolumna)
  lcd.print(lineTwo);
  if(delayy) delay(200);
}
void loop() 
{
  if((digitalRead(9) == HIGH) && (digitalRead(10) == HIGH)) //A i C
  {  
    digitalWrite(stateOne, LOW);
	digitalWrite(stateTwo, HIGH);
    state =2;      
    SetDisplay(true, true, "Switched to:", "LED control :D");
  }
  if((digitalRead(11) == HIGH) && (digitalRead(8) == HIGH)) //B i D
    {
      state = 1;
	    digitalWrite(stateOne, HIGH);
		digitalWrite(stateTwo, LOW);
      SetDisplay(true,true,  "Switched to:", "TEMP & FOTO");
    }
  if (state == 2)
  {
	  
         if (digitalRead(9) == HIGH) {  // Button A pressed
        LEDon(false, true, false, false);
        lcd.setCursor(0,1); 
        lcd.print("A: RED enabled");
       delay(5);
      }
        if (digitalRead(11) == HIGH) {  // Button B pressed
        LEDon(false, false, true, false);
        lcd.setCursor(0,1); 
        lcd.print("B: GREEN enabled");
       delay(5);
      }
        if (digitalRead(10) == HIGH) {  // Button C pressed
        LEDon(false, false, false, true);
        lcd.setCursor(0,1); 
        lcd.print("C: BLUE enabled");
       delay(5);
      }
      if (digitalRead(8) == HIGH) {  // Button D pressed
        LEDon(true, false, false, false);

        lcd.setCursor(0,1); 
        lcd.print("D: COLORS disabled");
       delay(5);
      }
  }
  
  else if(state ==1){
    //red temp
    if(refreshTemp > 99){
            int analog = analogRead(pinTempSensor);

          float R = 1023.0/analog-1.0;
          R = R0*R;

          float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
      
          String tempStr =  "Temp: ";
          tempStr += temperature;
          tempStr += " C";
          
          int foto = analogRead(pinFotoSensor);      
          String fotoStr ="Light: ";
            fotoStr += foto;
          SetDisplay(false, false, tempStr, fotoStr);
          refreshTemp =0;
          //LEDon(false, false, false, false);
          if(temperature > maxTemp)
          {
            String lineTwo = "";
            lineTwo += temperature;
            lineTwo += " > ";
            lineTwo += maxTemp;
            SetDisplay(true, false,"Max temp", lineTwo);
            LEDon(true, true, false, false);
              delay(500);
          }
          else if(temperature < minTemp){
            String lineTwo = "";
            lineTwo += temperature;
            lineTwo += " < ";
            lineTwo += minTemp;
            SetDisplay(true, false, "Min temp", lineTwo);
            LEDon(true, false, false, true);
              delay(500);
          }
          else if(foto > maxFoto)
          {
            String lineTwo = "";
            lineTwo += foto;
            lineTwo += " > ";
            lineTwo += maxFoto;
            SetDisplay(true, false, "Max foto", lineTwo);
            LEDon(true, true, true, false);
              delay(500);
          }
          else if(foto < minFoto){
            String lineTwo = "";
            lineTwo += foto;
            lineTwo += " < ";
            lineTwo += minFoto;
            SetDisplay(true, false, "Min foto reached", lineTwo );
            LEDon(true, true, false, true);
              delay(500);
          }
          else{
            LEDon(true, false, false, false);
          }
    }
  refreshTemp += 1;
  }
  //end of read
 
}
