 #include <GY6050.h>           //library for GYRO #include <Wire.h>
int X = 0;
int Y = 0;
int Z = 0;
GY6050 gyro(0x68);              //to save GYRO data
#include <TinyGPSPlus.h>
TinyGPSPlus gps;
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#define phoneNumber   "+84365801715"
const byte coi = A0;
#include <SoftwareSerial.h>
SoftwareSerial sim800l(10, 11); // RX, TX
float flat = 0.000000, flon = 0.000000;
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (sim800l.available())
      gps.encode(sim800l.read());
    flat =  gps.location.lat(), 6;
    flon =  gps.location.lng(), 6;
   // Serial.println("DOC GPS");
    Serial.print(flat,6);
      Serial.print(" ---- FLON: ");
      Serial.println(flon,6); 
  }
  while (millis() - start < ms);
}

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);
  Wire.begin();
  gyro.initialisation();
  Serial.println("ok");
  pinMode(coi, OUTPUT);
  digitalWrite(coi, HIGH);
}
void loop(){
  X = map(gyro.refresh('A', 'X'), -90, 90, -90, 90);                //mapping the gyro data according to angle limitation of servo motor 
  Y = map(gyro.refresh('A', 'Y'), -90, 90, -90, 90);
  Z= map(gyro.refresh('A', 'Z'), -90, 90, -90, 90);
  Serial.print(X);
  Serial.print(" ----");
  Serial.print(Y);
  Serial.print((" ----"));
  Serial.println(Z);
  if (X > 60 || X < -60 || Z > 60 || Z < -60 || Y <= -60) { // Nếu góc nghiêng lớn hơn 70 độ
    digitalWrite(coi, LOW);
    sendSMS();
      goi();
      
  }
  else digitalWrite(coi, HIGH);
  delay(10); // Đọc cảm biến nghiêng và kiểm tra nút bấm mỗi giây
}


void sendSMS() {
  Serial.println("DANG GUI TIN NHAN");
  sim800l.println("AT+CMGF=1"); // Đặt chế độ gửi tin nhắn
  delay(1000);
  sim800l.println("AT+CMGS=\"" + String(phoneNumber) + "\"");
  delay(1000);
  sim800l.print("NGUOI THAN BI NGA! Vi Tri la: http://maps.google.com/maps?q=loc:" + String(flat,6) +"," + String(flon, 6));
  delay(1000);
  sim800l.write(26);
  Serial.println("DA GUI TIN NHAN");
  delay(5000);
  
}
void goi() {
  Serial.println("DANG GOI DIEN");
  sim800l.println("ATD" + String(phoneNumber) + ";"); // Thực hiện cuộc gọi đến số điện thoại
  delay(20000); // Đợi 30 giây cho cuộc gọi
  sim800l.println("ATH"); // Kết thúc cuộc gọi
  delay(1000);
  Serial.println("KET THUC");
}
