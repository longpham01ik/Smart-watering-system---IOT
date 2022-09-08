#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

#define BLYNK_TEMPLATE_ID "TMPLKSlm4pXQ"
#define BLYNK_DEVICE_NAME "IOT"
#define BLYNK_AUTH_TOKEN "0s9bIcISOGzH-moDNMJPhR30-s1TgpfQ"

char auth[] = "0s9bIcISOGzH-moDNMJPhR30-s1TgpfQ";   //mã auth vừa được gửi về
char ssid[] = "Tran Dang Thuc";       // tên wifi cần truy cập
char pass[] = "66668888";    //pass wifi

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ledr D5 
#define ledg D6 
#define ledy D7 
#define analogPin A0 
#define relay D3 
int value1,value2,analogVal,button,Ref1,Ref2; 
int phantram; 
WidgetLED  appledr(V10); 
WidgetLED  appledg(V11); 
WidgetLED  appledy(V12); 
void setup() 
{ 
  Serial.begin(9600); 
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); 
  pinMode(ledr,OUTPUT); 
  pinMode(ledy,OUTPUT); 
  pinMode(ledg,OUTPUT); 
  pinMode(relay,OUTPUT); 
  Blynk.syncVirtual(V2); 
  Blynk.syncVirtual(V3); 
  Blynk.syncVirtual(V4); 
  Blynk.syncVirtual(V9); 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();

  //Call the function
  timer.setInterval(100L, soilMoistureSensor); 
 } 
BLYNK_WRITE(V2) { // điều chỉnh chế độ 
  value1 = param.asInt(); 
} 
BLYNK_WRITE(V3) { // Lấy giá trị ngưỡng trên 
  Ref1=param.asInt(); 
} 
BLYNK_WRITE(V4) { // Lấy giá trị ngưỡng duoi 
   Ref2=param.asInt(); 
} 
BLYNK_WRITE(V9) { // Nut nhan bat tat relay 
  button=param.asInt(); 
}
void soilMoistureSensor() {
  int value = analogRead(analogPin);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" ");

} 
void loop() 
{ 
  Blynk.run(); 
  timer.run(); 
  analogVal = analogRead(analogPin); // 0 --> 1023 
  phantram=map(analogVal, 0, 1023, 100, 0); // chuyen sang phantram 
  Blynk.virtualWrite(V1,phantram); 
  Blynk.virtualWrite(V8,String(phantram)+"%"); 
  if (value1 == 1){ 
    if (phantram > Ref1){ 
      appledr.off(); 
      appledg.off(); 
      appledy.on(); 
      digitalWrite(ledr,LOW); 
      digitalWrite(ledg,LOW); 
      digitalWrite(ledy,HIGH);
      digitalWrite(relay,LOW); 
      Blynk.notify("Độ ẩm cao"); 
      Blynk.virtualWrite(V6,"Độ ẩm cao"); //hiển thị trên LCD dòng 1 
      lcd.setCursor(0, 1);
      lcd.print("Motor pump OFF");
    } 
    else if (phantram < Ref2){ 
      appledr.on(); 
      appledg.off(); 
      appledy.off(); 
      digitalWrite(ledr,HIGH); 
      digitalWrite(ledg,LOW); 
      digitalWrite(ledy,LOW); 
      digitalWrite(relay,HIGH); 
      Blynk.notify("Độ ẩm thấp"); 
      Blynk.virtualWrite(V6,"Đang tưới nước");
      lcd.setCursor(0, 1);
      lcd.print("Motor pump ON "); 
    } 
    else { 
      appledr.off(); 
      appledg.on(); 
      appledy.off(); 
      digitalWrite(ledr,LOW); 
      digitalWrite(ledg,HIGH); 
      digitalWrite(ledy,LOW); 
      digitalWrite(relay,LOW); 
      Blynk.virtualWrite(V6,"Độ ẩm BT");
      lcd.setCursor(0, 1);
      lcd.print("Motor pump OFF"); 
    } 
  } 
  else { 
    if (button == 1){ 
      digitalWrite(relay,HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Motor pump ON "); 
    } 
    else { 
      digitalWrite(relay,LOW);
      lcd.setCursor(0, 1);
      lcd.print("Motor pump OFF"); 
    }  
  } 
} 
