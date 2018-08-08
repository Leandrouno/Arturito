//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

#include <OneWire.h>
#include <DallasTemperature.h>

 
// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ =2;
const int Rele =3;
 
// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int backLight   = 10;    // LCD Panel Backlight LED connected to digital pin 10
int lightLevel  = 255;   // Initialise light full on
int lcd_key     = 0;
int adc_key_in  = 0;
float t =sensorDS18B20.getTempCByIndex(0), tant=0;
int cons = t;
int autom = 0;
int inicio = 0, estado = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
adc_key_in = analogRead(0);      // read the value from the sensor 
// my [Mark Bramwell's] buttons when read are centered at these valies: 0, 144, 329, 504, 741
// we add approx 50 to those values and check to see if we are close
if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
if (adc_key_in < 50)   return btnRIGHT;  
if (adc_key_in < 195)  return btnUP; 
if (adc_key_in < 380)  return btnDOWN; 
if (adc_key_in < 555)  return btnLEFT; 
if (adc_key_in < 790)  return btnSELECT;   
return btnNONE;  // when all others fail, return this...
}

void setup()
{
Serial.begin(9600);
sensorDS18B20.begin(); 
pinMode(Rele,OUTPUT);
digitalWrite(Rele, HIGH);
lcd.begin(16, 2);              // start the LCD library
lcd.setCursor(0,0);            // move cursor to beginning of line "0"
lcd.print("Temperatura"); // print a simple message
sensorDS18B20.requestTemperatures();
lcd.clear();
}

void loop()
{

analogWrite(backLight, lightLevel);
lcd.setCursor(0,0);            // move cursor to beginning of line "0"
lcd.print("Tempe.");
lcd.setCursor(9,0);            // move to position 13 on the second line
lcd.print(t);
lcd.setCursor(13,1);            
lcd.print("OFF");

lcd.setCursor(0,1);            // move cursor to beginning of line "0"
lcd.print("Consigna");
lcd.setCursor(9,1);            // move to position 13 on the second line
lcd.print(cons);

lcd.setCursor(0,1);            // move to the begining of the second line
lcd_key = read_LCD_buttons();  // read the buttons

switch (lcd_key){
   case btnRIGHT:{Serial.println("PANTALLA ENCENDIDA");lightLevel = 255;lcd.display();delay(200);break;}
   case btnLEFT:  {Serial.println("PANTALLA APAGADA"); lightLevel = 1;lcd.noDisplay();delay(200);break;}
    case btnUP:{
                      Serial.println("BOTON UP");
                      Serial.print("Consigna:");Serial.println(cons);
                      lcd.clear();
                      lcd.display();
                      lightLevel = 255;                            
                      cons++;                     
                      delay(200);
                      lcd.clear();
                      break;}
     case btnDOWN:{    
                      Serial.println("BOTON DOWN");
                      Serial.print("Constante:");Serial.println(cons);
                      lcd.clear();
                      lcd.display();
                      lightLevel = 255;                          
                      cons--;                     
                      delay(200);
                      lcd.clear();
                      break;}
   case btnSELECT:  { 
                          Serial.println("BOTON SELECT");
                          lcd.clear();
                          lcd.display();
                          lcd.setCursor(0,0);  
                          lcd.print("Automatico ...");
                          if(autom==0){Serial.println("MODO AUTOMATICO"); autom=1;} 
                            else {Serial.println("DESACTIVADO"); autom=0; inicio = 0; if( estado == 1 ){power();estado = 0;}}
                          delay(200);
                          lcd.clear();
                          break;
                          }
               
                }//FIN SWITCH LCD

   
if (autom==1){
lcd.clear();
//Serial.println("LOOPEA......"); 
lcd.setCursor(0,0);            // move cursor to beginning of line "0"
lcd.print("Tempe.");
lcd.setCursor(9,0);            // move to position 13 on the second line
lcd.print(t);
lcd.setCursor(13,1);            
lcd.print("ON");

lcd.setCursor(0,1);            // move cursor to beginning of line "0"
lcd.print("Consigna");
lcd.setCursor(9,1);            // move to position 13 on the second line
lcd.print(cons);
  
//  sensorDS18B20.requestTemperatures();
Serial.print("Temperatura: ");
Serial.println(sensorDS18B20.getTempCByIndex(0));
t =sensorDS18B20.getTempCByIndex(0);
  delay(100);

if(cons > t && estado == 0 && inicio == 0)
  {power(); estado = 1; inicio = 1;}
    else
    if(t>cons+2 && estado == 1 && inicio == 1)
      {power(); estado = 0;delay(20000);}
        else
          if(t<cons-2 && estado == 0 && inicio == 1)
           {power(); estado = 1;}         

if(t > cons+3 && estado == 0 && inicio == 1)
    estado=1;
    
  // tant=t;
   
  }// FIN AUTOMATICO
} // FIN LOOP

void power (){
   digitalWrite(Rele, LOW);
    delay(300);
    digitalWrite(Rele, HIGH);
if(estado == 0){
   Serial.print("Manda Codigo PRENDIDO/////A32AB931/////Temperatura:");}
    else{
       Serial.print("Manda Codigo APAGADO //////////Temperatura:");}
Serial.print(t);Serial.print("Consigna:");Serial.println(cons);
Serial.println(" ");

}
