#include <SPI.h>                          //SPI kütüphanemizi tanımlıyoruz.
#include <MFRC522.h>                      //MFRC522 kütüphanemizi tanımlıyoruz.
#include <Servo.h>                        //Servo kütüphanemizi tanımlıyoruz.
const byte kirmizi_led = 5;
const byte yesil_led = 6;
int RST_PIN = 9;                          //RC522 modülü reset pinini tanımlıyoruz.
int SS_PIN = 10;                          //RC522 modülü chip select pinini tanımlıyoruz.
int servoPin = 8;                         //Servo motor pinini tanımlıyoruz.
int buzzer = 7;
Servo motor;                              //Servo motor için değişken oluşturuyoruz.
MFRC522 rfid(SS_PIN, RST_PIN);            //RC522 modülü ayarlarını yapıyoruz.
byte ID[4] = {233, 82, 170, 89};          //Yetkili kart ID'sini tanımlıyoruz. 

void setup() { 
  pinMode(buzzer,OUTPUT);
  motor.attach(servoPin);                 //Servo motor pinini motor değişkeni ile ilişkilendiriyoruz.
  Serial.begin(9600);                     //Seri haberleşmeyi başlatıyoruz.
  SPI.begin();                            //SPI iletişimini başlatıyoruz.
  rfid.PCD_Init();  //RC522 modülünü başlatıyoruz.

  pinMode(kirmizi_led, OUTPUT);
  pinMode(yesil_led, OUTPUT);
}
 
void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())    //Yeni kartın okunmasını bekliyoruz.
    return;

  if ( ! rfid.PICC_ReadCardSerial())      //Kart okunmadığı zaman bekliyoruz.
    return;

  if (rfid.uid.uidByte[0] == ID[0] &&     //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
    rfid.uid.uidByte[1] == ID[1] && 
    rfid.uid.uidByte[2] == ID[2] && 
    rfid.uid.uidByte[3] == ID[3] ) {
        Serial.println("Kapi acildi");
        ekranaYazdir();
         digitalWrite(yesil_led, HIGH);
        delay(1000);
        digitalWrite(yesil_led, LOW);
        motor.write(180);                 //Servo motoru 180 dereceye getiriyoruz.
        delay(3000);
        motor.write(0);                   //Servo motoru 0 dereceye getiriyoruz.
        delay(1000);
             
          
    }
    else{                                 //Yetkisiz girişte içerideki komutlar çalıştırılır.
      digitalWrite(buzzer,HIGH); //Buzzer'a güç göndererek ses çıkarmasını sağladık.
 delay(1000);
 digitalWrite(buzzer,LOW); //Buzzerın gücünü keserek susmasını sağladık sağladık.
 delay(1000);                 // 1 saniye boyunca susmasını söyledik.
  digitalWrite(kirmizi_led, HIGH);
     delay(2000);
     digitalWrite(kirmizi_led, LOW);

      Serial.println("Yetkisiz Kart");
      ekranaYazdir();
        
              
    }
  rfid.PICC_HaltA();
}
void ekranaYazdir(){
  Serial.print("ID Numarasi: ");
  for(int sayac = 0; sayac < 4; sayac++){
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}
