#include <SoftwareSerial.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Servo.h"

#define newLED 11  // ��� LED�� 2���ɿ� �Ƚ��ϴ�.
#define okLED 12   // �Ķ��� LED�� 4���ɿ� �Ƚ��ϴ�.
#define oldLED 13  // ������ LED�� 6���ɿ� �Ƚ��ϴ�.
 
MPU6050 mpu;
 
int16_t ax, ay, az;
int16_t gx, gy, gz;

Servo myservo;
 
int val;
int prevVal;

int trig = 6;
int echo = 7;
int buzzer = 8;
float duration;
float distance; // Measure_Distance

int lock_pin = 10; // Lock ON, OFF

int analogValue = 0;
float voltage = 0;
int ledDelay = 500; // Measure_Battery

SoftwareSerial mySerial(2,3);

void setup(){
  Wire.begin();
  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(lock_pin,OUTPUT);
  pinMode(newLED, OUTPUT);
  pinMode(okLED, OUTPUT);
  pinMode(oldLED, OUTPUT); // LED�� �ɸ�带 OUTPUT���� �������ݴϴ�
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("Hello World!");
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
  myservo.attach(9);
  myservo.attach(10);
  mySerial.begin(9600);
}

void loop(){
  digitalWrite(trig,HIGH);
  delay(10);
  digitalWrite(trig,LOW);
  duration = pulseIn(echo,HIGH);
  distance = (duration/29)/2;
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  val = map(ay, -17000, 17000, 0, 179);
  f
  analogValue = analogRead(0);
  voltage = (((float)analogValue) / 1024.0 ) * 5.0;
  
  Serial.print("Distance :");
  Serial.print(distance);
  Serial.print("cm");
  Serial.print("\t|\t");
  Serial.print("Angle :");
  Serial.print(val);
  Serial.print("degree");
  Serial.print("\t|\t");
  Serial.print("Voltage :");
  Serial.print(voltage); // A0�ɿ� ������ ���� �����Ͽ� �� ���� ���а����� �ٲ��ݴϴ�.
  Serial.println("V");
   
  if (val != prevVal){
        myservo.write(val);
        prevVal = val;
    }
    
  if(voltage >= 2.5){ // ���а��� 2.5�̻��� ��� ���͸��ܷ� ��л���
    digitalWrite(newLED, HIGH);
    delay(ledDelay);
    digitalWrite(newLED, LOW);
  }
  else if(voltage < 2.5 && voltage > 0.5){ // 2.5�̸� 0.5�ʰ��� ��� ���͸��ܷ� �߰�����
    digitalWrite(okLED, HIGH);
    delay(ledDelay);
    digitalWrite(okLED, LOW);
  }
  else if(voltage <= 0.5){ // 1.0������ ��� ���͸� ���� �� �� ����
    digitalWrite(oldLED, HIGH);
    delay(ledDelay);
    digitalWrite(oldLED, LOW);
  }  
  
  if(mySerial.available()){
    switch(mySerial.read()){
     case 'E' : //function3 ON
      do{ 
       if(distance > 80){  
         tone(buzzer,1000,100);
         delay(100);
         tone(buzzer,1000,100);
         delay(100);       
        }
       else{
        ;
       }
      delay(100);
      }while(mySerial.read() != 'F');
     break; 
     
    case 'F' : // function3 OFF
       noTone(buzzer);
       myservo.detach();
       delay(100);
       break;
    
    
    case 'C' : // function2 ON
    myservo.attach(9);
    if ((val>=75) && (val<=105)){
        myservo.write(30);
       }
       else if((val>=60) && (val<=75)){
        myservo.write(80);
       }
       else if((val>=105) && (val<=120)){
        myservo.write(80);
       }
     break;
     
    case 'D' : // function2 OFF
      myservo.detach();
    break;
    
    
    case 'A' : // function1 ON
      digitalWrite(lock_pin,HIGH);
      myservo.write(90);
      break;
      
    case 'B' : // function1 OFF
      digitalWrite(lock_pin,LOW);
      myservo.write(0);
      break;
  
  }
}
}