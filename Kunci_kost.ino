#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define LIMIT_SWITCH_PIN 3  // Limit switch terhubung ke pin digital 4
const int buzzer = 4;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Membuat instance MFRC522
Servo myServo;  // Membuat instance Servo

// Gantilah ID Tag di bawah ini dengan ID RFID Tag yang benar
byte authorizedUID[] = {0xF1, 0x2D, 0xA8, 0x19};
byte authorizedUID2[] = {0xB2, 0xD3, 0x0C, 0x1A};
int infoswitch;
bool akses = false;

void setup() {
  Serial.begin(9600);  // Memulai komunikasi serial
  SPI.begin();         // Memulai SPI bus
  mfrc522.PCD_Init();  // Inisialisasi RFID reader
  myServo.attach(2);   // Hubungkan servo ke pin digital 3
  pinMode(LIMIT_SWITCH_PIN, INPUT);  // Atur limit switch sebagai input
  pinMode(buzzer, OUTPUT);
  myServo.write(90);    // Set servo ke posisi 0 derajat
}

void loop() {
  infoswitch = digitalRead(LIMIT_SWITCH_PIN);
  // Periksa apakah ada kartu yang terdeteksi
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (checkUID(authorizedUID) || checkUID(authorizedUID2)) {
      Buzzer1x("rfidC");
      matiin();
      Serial.println("Access Accept");
    } else {
      Buzzer1x("rfidI");
      Serial.println("Access Denied");
    }
    mfrc522.PICC_HaltA();   // Berhenti membaca kartu
  }
  if(infoswitch == 1){
    matiin();
  }

  if(akses == true){
    myServo.write(90);
  } else{
    myServo.write(0);
  }
  delay(100);
}

void matiin(){
  delay(250);
  if(akses == true){
    akses = false;
    Buzzer1x("servoC");
  } else{
    akses = true;
    Buzzer1x("servoO");
  }
}

void Buzzer1x(String buzz){
  if(buzz == "rfidC"){
    tone(buzzer, 2217); // Send 1KHz sound signal...
    delay(200);   
    tone(buzzer, 4186);     // ...for 1 sec
    delay(100);   
    noTone(buzzer); 
  }
  if(buzz == "rfidI"){
    tone(buzzer, 3136); // Send 1KHz sound signal...
    delay(200);   
    tone(buzzer, 2217);     // ...for 1 sec
    delay(100);   
    noTone(buzzer); 
  }
  if(buzz == "servoC"){
    tone(buzzer, 2217); 
    delay(100); 
    noTone(buzzer); 
  }
  if(buzz == "servoO"){
    tone(buzzer, 4186); 
    delay(100); 
    noTone(buzzer); 
  }
}

// Fungsi untuk memeriksa apakah UID sesuai
boolean checkUID(byte *expectedUID) {
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != expectedUID[i]) {
      return false;
    }
  }
  return true;
}
