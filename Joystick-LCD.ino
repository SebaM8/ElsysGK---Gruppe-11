#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>


int avdeling[3][3] = {{0,0,0},
                    {0,0,0},
                    {0,0,0}};
int avdeling9_pers = 0;
int avdeling9_jur = 0;

int x = 0;
int y = 0;
int x_0 = 1;
int y_0 = 1;

int del = 300;

char *navnant[3][3] = {{"Kreftklinikken:", "Ungdomsklinikken:", "Barneklinikken:"},
                    {"Nevrologisk:", "Totalt antall:", "Hudklinikken:"},
                    {"Nevrologi:", "Arbeidsmedisinsk:", "Ore-nese-hals:"}};

char *navn[3][3] = {{"Kreftklinikken", "Ungdomsklinikken", "Barneklinikken"},
                    {"Nevrologisk", "Totalt antall", "Hudklinikken"},
                    {"Nevrologi", "Arbeidsmedisinsk", "Ore-nese-hals"}};

int posisjon[3][3] = {{0,0,0},
                      {0,1,0},
                      {0,0,0}};
int nyposisjon[3][3] = {{0,0,0},
                      {0,0,0},
                      {0,0,0}};

const int X_pin = A2;
const int Y_pin = A1;
const int joystick_knapp = 2;

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

int buttonstate = 0;
int joystick = 0;

unsigned long sistMillis = 0UL;
unsigned long sistMillisInt = 0UL;
unsigned long intervall = 2500UL;
unsigned long intervalljoy = 300UL;
unsigned long intervallint = 200UL;



void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.update(address, number);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address));
}

int konvertorWrite(int avdeling[3][3], int y, int x){
  int a = avdeling[y][x];
  return a;
}

void konvertorRead(int tall, int y, int x){
  avdeling[y][x] = tall;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);
  pinMode(joystick_knapp, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  for (byte i = 0; i < 3; i = i + 1){
      for (byte l = 0; l < 3; l = l + 1){
        int lestTall = readIntFromEEPROM(70+(3*i)+l);
        konvertorRead(lestTall, i, l);
      }
    }

}

void loop() {
  // put your main code here, to run repeatedly:
    
    
    while (interface() == 0){
      visning();
      interface();
    }
    
    while (interface() == 1){
      
      posisjonering();
      interface();
    }
    while (interface() == 2){
      redigering();
      interface();
    }
    for (byte i = 0; i < 3; i = i + 1){
      for (byte l = 0; l < 3; l = l + 1){
        int lagerTall = konvertorWrite(avdeling, i, l);
        writeIntIntoEEPROM(70+(i*3)+l, lagerTall);
      }
    }


}

int interface(){
  
  buttonstate = !digitalRead(joystick_knapp);
  unsigned long currentMillis = millis();
  if(currentMillis - sistMillisInt > intervall){
    if ((buttonstate == 1) && (joystick == 0)){
      joystick = 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Velg: ");
      lcd.setCursor(0, 1);
      lcd.print(String(navn[1+y][1+x]));
      sistMillisInt = currentMillis;
    }
    else if ((buttonstate == 1) && (joystick == 1)){
      joystick = 2;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(String(navnant[1+y][1+x]));
      lcd.setCursor(0, 1);
      lcd.print(String(avdeling[1+y][1+x]));
      sistMillisInt = currentMillis;
    }
    else if ((buttonstate == 1) && (joystick == 2)){
      joystick = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(String(navn[1+y][1+x]));
      lcd.setCursor(0, 1);
      lcd.print(String(avdeling[1+y][1+x]));
      sistMillisInt = currentMillis;
    }
  }

  return joystick;

}

void visning(){
  int oldL = 4;
  int oldI = 4;
    for (byte i = 0; i < 3; i = i + 1){
        for (byte l = 0; l < 3; l = l + 1){
          unsigned long currentMillis = millis();
          if(currentMillis - sistMillis > intervall){
            if ((oldL != l)||(oldI != i)){
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(String(navn[i][l]));
              lcd.setCursor(0, 1);
              lcd.print(String(avdeling[i][l]));
              oldL = l;
              oldI = i;
              sistMillis = currentMillis;
            }
          }
        }
    }
}

void redigering(){
  unsigned long currentMillis = millis();
  String disp1;
  String disp2;
  int gammel = avdeling[1+y][1+x];
  int sjekk = 0;
  if(currentMillis - sistMillis > intervalljoy){
    if (analogRead(Y_pin) > 600) {
      Serial.println("Opp");
      avdeling[1+y][1+x] += 1;
      Serial.println("Antall prosjekter:");
      Serial.println(avdeling[1+y][1+x]);
      sistMillis = currentMillis;
    }
    else if (analogRead(Y_pin) < 400) {
      Serial.println("Ned");
      if (avdeling[1+y][1+x] > 0) {
        avdeling[1+y][1+x] -= 1;
      }
      Serial.println("Antall prosjekter:");
      Serial.println(avdeling[1+y][1+x]);
      sistMillis = currentMillis;
    }
  }
  if (avdeling[1+y][1+x] != gammel){
    Serial.println("Antall prosjekter:");
    Serial.println("-------");
    for (byte i = 0; i < 3; i = i + 1){
      for (byte l = 0; l < 3; l = l + 1){
        Serial.print(avdeling[i][l]);
      }
      Serial.println('\r');
    }
    Serial.println("-------");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(String(navnant[1+y][1+x]));
    lcd.setCursor(0, 1);
    lcd.print(String(avdeling[1+y][1+x]));
  }
}

void posisjonering(){
  unsigned long currentMillis = millis();
  if(currentMillis - sistMillis > intervalljoy){
    if (analogRead(Y_pin) > 600) {
      Serial.println("Opp");
        if (y > -1){
          y -= 1;
          Serial.println("X/Y:");
          Serial.print(x);
          Serial.println(y);
          Serial.println("x_0/y_0:");
          Serial.print(x_0);
          Serial.println(y_0);
        }
      sistMillis = currentMillis;
    }
    if (analogRead(Y_pin) < 400) {
      Serial.println("Ned");
        if (y < 1){
          y += 1;
          Serial.println("X/Y:");
          Serial.print(x);
          Serial.println(y);
          Serial.println("x_0/y_0:");
          Serial.print(x_0);
          Serial.println(y_0);
        }
      sistMillis = currentMillis;
    }
    if (analogRead(X_pin) > 600) {
      Serial.println("Venstre");
        if (x > -1){
          x -= 1;
          Serial.println("X/Y:");
          Serial.print(x);
          Serial.println(y);
          Serial.println("x_0/y_0:");
          Serial.print(x_0);
          Serial.println(y_0);
        }
      sistMillis = currentMillis;
    }
    if (analogRead(X_pin) < 400) {
      Serial.println("Høyre");
        if (x < 1){
          x += 1;
          Serial.println("X/Y:");
          Serial.print(x);
          Serial.println(y);
          Serial.println("x_0/y_0:");
          Serial.print(x_0);
          Serial.println(y_0);
        }
      sistMillis = currentMillis;
    }
  }

  if ((y != y_0) || (x != x_0)){
    y_0 = y;
    x_0 = x;
    int posisjon[3][3] = {{0,0,0},
                          {0,0,0},
                          {0,0,0}}; 
    posisjon[1+y][1+x] = 1;
    Serial.println("Du er her:");
    Serial.println("-------");
    for (byte i = 0; i < 3; i = i + 1){
      for (byte l = 0; l < 3; l = l + 1){
        Serial.print(posisjon[i][l]);
      }
      Serial.println('\r');
    }
    Serial.println("-------");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Velg: ");
    lcd.setCursor(0, 1);
    lcd.print(String(navn[1+y][1+x]));
  }
}
