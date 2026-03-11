#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 6                     // Definiendo el led Verde
#define LED_R 4                     // Definiendo el led Rojo
#define BUZZER 2                    // Definiendo el Buzzer
MFRC522 mfrc522(SS_PIN, RST_PIN);   
Servo myServo;                      // Definiendo el nombre del servo
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C en direccion 0x27, 16 columnas x 2 filas
 
void setup() 
{
  Serial.begin(9600);      // Iniciando la serial de comunicacion
  SPI.begin();             // Iniciando SPI bus
  mfrc522.PCD_Init();      // Iniciando el MFRC522
  myServo.attach(3);       // Definiendo el pin del Servo
  myServo.write(0);        // Posicion Incial del servo
  pinMode(LED_G, OUTPUT);  // Seteando el led verde a su pin correspondiente
  pinMode(LED_R, OUTPUT);  // Seteando el led rojo a su pin correspondiente
  pinMode(BUZZER, OUTPUT); // Seteando el buzzer a su pin correspondiente
  noTone(BUZZER);
  
  // Iniciando el LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  Serial.println("Buscando Tarjeta");
  Serial.println();
  
  lcd.setCursor(0, 0);
  lcd.print("Buscando Tarjeta");

}
void loop() 
{
  // Buscando por Tarjetas
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Elige una de las Tarjetas
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //Mostrando el UID de la tarjeta
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
 ;
  

  Serial.print("Mensaje : ");                 // Seteando la base del mensaje de la solicitud
  content.toUpperCase();
  if (content.substring(1) == "C3 73 04 36")  // UID de la tarjeta que tiene saldo
  {
    Serial.println("Pasaste Pagando Normal"); // Manda el mensaje a la Serial Monitor
    Serial.println(); 
    
    // Mostrar mensaje en LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mensaje:");
    lcd.setCursor(0, 1);
    lcd.print("Pagando Normal");
    
    delay(500);                // Esperando 500 milisegundos
    digitalWrite(LED_G, HIGH); // Encendiendo el led
    tone(BUZZER, 500);         // Enciendo el buzzer
    delay(300);                // Esperando 300 milisegundos
    noTone(BUZZER);            // Apagando el buzzer
    myServo.write(90);         // Moviendo el buzzer 90 grados
    delay(5000);               // Esperando 5 segundos
    myServo.write(0);          // Devolviendo el servo a 0 grados
    digitalWrite(LED_G, LOW);  // Apagando el led
    
    // Volver a pantalla de espera
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Buscando Tarjeta");
  }
    // Manda el mensaje a la Serial Monitor
 else   {
    Serial.println("Pasaste Debiendo"); 
    Serial.println();
    
    // Mostrar mensaje en LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mensaje:");
    lcd.setCursor(0, 1);
    lcd.print("Pasaste Debiendo");
    
    delay(500);                // Esperando 500 milisegundos
    digitalWrite(LED_R, HIGH); // Encendiendo el led
    tone(BUZZER, 900);         // Enciendo el buzzer
    delay(300);                // Esperando 300 milisegundos
    noTone(BUZZER);            // Apagando el buzzer
    myServo.write(90);         // Moviendo el buzzer 90 grados
    delay(5000);               // Esperando 5 segundos
    myServo.write(0);          // Devolviendo el servo a 0 grados
    digitalWrite(LED_R, LOW);  // Apagando el led 
    
    // Volver a pantalla de espera
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Buscando la Tarjeta");
  }
}

//Codigo Hecho y Explicado via Etiquetas Por Fernando Alexis Sanchez Javier

