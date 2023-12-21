#include<Servo.h>

#define NOTE_D4 294
#define NOTE_F4S 739
#define NOTE_A4 440
#define NOTE_C4 523
#define NOTE_D4S 622

Servo servo;

int const trigPin = 6; 
int const echoPin = 5; 
int const ledOk = 8;
int const ledDescarte = 9;
int const pinoBuzzer = 10;

void setup() 
{ 
pinMode(trigPin, OUTPUT);  
pinMode(echoPin, INPUT);

pinMode(ledOk, OUTPUT);
pinMode(ledDescarte, OUTPUT);   

pinMode(pinoBuzzer, OUTPUT);

servo.attach(3); 

Serial.begin(9600);
} 

void loop() 
{    

int duration, distance;
int distancia;

servo.write(108);

digitalWrite(trigPin, HIGH);  
delay(100); 
digitalWrite(trigPin, LOW);// Measure the pulse input in echo pin 
duration = pulseIn(echoPin, HIGH);// Distance is half the duration devided by 29.1 (from datasheet) 
distance = (duration/2) / 29.1;// if distance less than 0.5 meter and more than 0 (0 or less means over range)

Serial.print("Distancia: ");
Serial.println(distance);

if (distance >= 15 && distance <= 22)  
{
  distancia = distance;

  somErro();
  servo.write(180);
  digitalWrite(ledDescarte, HIGH);
  delay(200);
  digitalWrite(ledDescarte, LOW);
  servo.write(108);

  eventoTrue(distancia);
      
}  
else if(distance >= 5 && distance < 15)
{ 
  distancia = distance;
  somAcerto();
  servo.write(36);
  digitalWrite(ledOk, HIGH);
  delay(200);
  digitalWrite(ledOk, LOW);
  servo.write(108);

  eventoTrue(distancia);
}
else
{

}
}

void somAcerto()
{
  tone(pinoBuzzer, NOTE_D4, 1000);
  tone(pinoBuzzer, NOTE_F4S, 1000);
  tone(pinoBuzzer, NOTE_A4, 1000);
}

void somErro()
{
  tone(pinoBuzzer, NOTE_C4, 1000);
  tone(pinoBuzzer, NOTE_D4S, 1000);
  tone(pinoBuzzer, NOTE_F4S, 1000);
}

void eventoTrue(int distancia)
{
  int distance, duration;

  digitalWrite(trigPin, HIGH);  
  delay(100); 
  digitalWrite(trigPin, LOW);// Measure the pulse input in echo pin 
  duration = pulseIn(echoPin, HIGH);// Distance is half the duration devided by 29.1 (from datasheet) 
  distance = (duration/2) / 29.1;// if distance less than 0.5 meter and more than 0 (0 or less means over range)

  while(distancia >= distance-8 && distancia<= distance+8)
  {
    digitalWrite(trigPin, HIGH);  
    delay(100); 
    digitalWrite(trigPin, LOW);// Measure the pulse input in echo pin 
    duration = pulseIn(echoPin, HIGH);// Distance is half the duration devided by 29.1 (from datasheet) 
    distance = (duration/2) / 29.1;// if distance less than 0.5 meter and more than 0 (0 or less means over range)

    Serial.print(distancia);
    Serial.print(distance);
    Serial.println("Aguardando novo panetone");
  }

  if(distancia != distance)
  {
    return;
  }

}
