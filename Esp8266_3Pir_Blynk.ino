
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "   " your templated
#define BLYNK_DEVICE_NAME "   " Your Device Id
#define BLYNK_AUTH_TOKEN "  "  your token
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


const int pir1 = D1;  // define the PIR motion sensor 1 as pin D1
const int pir2 = D2;  // define the PIR motion sensor 2 as pin D2
const int pir3 = D3;  // define the PIR motion sensor 3 as pin D3

const int led1 = D4;  // define the LED 1 as pin D4
const int led2 = D5;  // define the LED 2 as pin D5
const int led3 = D6;  // define the LED 3 as pin D6
const int LDRSensor = D8;
const int ANALOG_IN_PIN = D7;

float adc_voltage = 0.0;
float in_voltage = 0.0;
// Floats for resistor values in divider (in ohms) / resistor pada modul
float R1 = 30000.0;
float R2 = 7500.0; 
// seting tegangan yang digunakan semakin besar. tegangan semakin diturunkan ref_voltagenya
float ref_voltage = 2.48;
int adc_value = 0;
int selectMode;

char auth[] = "   ";    // your token
char ssid[] = "   ";    // your wifi name
char pass[] = "   ";   // your wifi pass
WidgetLED PJU1(V3);
WidgetLED PJU2(V4);
WidgetLED PJU3(V5);
BlynkTimer timer;

void setup() {
  
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(pir3, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode (LDRSensor, INPUT);
  timer.setInterval(1000L, Mode);
  
}

BLYNK_WRITE(V6)
{
  selectMode = param.asInt();
  Mode;
}

void Mode()
{
  if (selectMode == 1){
    Serial.println("   Manual ");
    digitalWrite(led1, HIGH); PJU1.on();
    digitalWrite(led2, LOW);  PJU2.on();
    digitalWrite(led3, LOW);  PJU3.on();
  }
  else
  {
    Serial.println("   Auto ");
    automode();
  }
}





void sensor_arus() {
unsigned int x=0;
float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;

  for (int x = 0; x < 150; x++){ //Get 150 samples
  AcsValue = analogRead(A0);     //Read current sensor values   
  Samples = Samples + AcsValue;  //Add samples together
  delay (3); // let ADC settle before next sample 3ms
  }
  AvgAcs=Samples/150.0;//Taking Average of Samples
  AcsValueF = (4.135 - (AvgAcs * (5.0 / 1024.0)) )/0.185;//4.035 adalah offset untuk acs712(sensor arus) 20A di alat ini
  Serial.print(" Current : ");Serial.print(AcsValueF);Serial.print(" A ");//Print the read current on Serial monitor
  Blynk.virtualWrite(V1, AcsValueF);
  sensor_tegangan();
}




void automode(){
    // read the state of the PIR motion sensors
  int pir1State = digitalRead(pir1);
  int pir2State = digitalRead(pir2);
  int pir3State = digitalRead(pir3);
  int Sensordata = digitalRead (LDRSensor);
  Serial.print("Sensor value : ");
  Serial.print(Sensordata);
  // if motion is detected, turn on the corresponding LED
  if (pir1State == HIGH && Sensordata == 1) {
    digitalWrite(led1, HIGH);
    PJU1.on();
  } else {
    digitalWrite(led1, LOW);
    PJU1.off();
  }
  if (pir2State == HIGH && Sensordata == 1) {
    digitalWrite(led2, LOW);
    PJU2.on();
  } else {
    digitalWrite(led2, HIGH);
    PJU2.off();
  }
  if (pir3State == HIGH && Sensordata == 1) {
    digitalWrite(led3, LOW);
    PJU3.on();
  } else {
    digitalWrite(led3, HIGH);
    PJU3.off();
  }
}

void sensor_tegangan(){
   // Read the Analog Input
   adc_value = analogRead(ANALOG_IN_PIN);
   // Determine voltage at ADC input
   adc_voltage  = (adc_value * ref_voltage) / 1024.0; 
   // Calculate voltage at divider input
   in_voltage = adc_voltage / (R2/(R1+R2)) ; 
   // Print results to Serial Monitor to 2 decimal places
  Serial.print(" Voltage : ");
  Serial.println(in_voltage, 2);
  Blynk.virtualWrite(V2, in_voltage);
  // Short delay
  delay(100);
}


void loop() {
  Blynk.run();
  timer.run();
  sensor_arus();
}
