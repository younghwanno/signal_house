
/*
*  Arduino D
*    
*/
#include "ESP8266.h" 
#include "HX711.h"
#include <DHT11.h>
#include <SoftwareSerial.h> 


#include <AddicoreRFID.h>
#include <SPI.h>

 

#define uchar   unsigned char
#define uint    unsigned int
uchar serNumA[5];
uchar fifobytes;
uchar fifoValue;
AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module

 
const int chipSelectPin = 53;
const int NRSTPD = 8;

#define MAX_LEN 16

 
#define SSID        "test"
#define HOST_NAME   "192.168.43.64"
#define HOST_PORT   8082
#define PASSWORD    "123456789" 

#define TH_7 22
DHT11 dht7(TH_7);


#define rain_check1 A0
#define rain_check2 A1
int rain_sensorstate[2] ={0};


#define human_check1 12
#define human_check2 13
int human_sensorstate[2] ={0};


#define dust_check  9
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;


#define vibration_check   3
int state, vibration_state = 0;


#define current_check1 A2
#define current_check2 A5
int current_Value1, current_Value2;


#define DOUT  4
#define CLK  2

HX711 scale(DOUT, CLK);

float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup
////////////////////////////////////////////////////Voltage1

#define voltage_check1 A3

    float voltage1 = 0.0;
    float voltage_sensorValue1 = 0.0f;
    String voltageString1 = "0.0";
    int stringLength1 = 0;

    float vout1 = 0.0;
    float vin1 = 0.0;
    float R1_1 = 30000.0;
    float R2_1 = 7500.0;

//////////////////////////////////////////////////////Voltage2
#define voltage_check2 A6

    float voltage2 = 0.0;
    float voltage_sensorValue2 = 0.0f;
    String voltageString2 = "0.0";
    int stringLength2 = 0;

    float vout2 = 0.0;
    float vin2 = 0.0;
    float R1_2 = 30000.0;
    float R2_2 = 7500.0;



#define ReadUVintensityPin  A4 //Output from the sensor
int light_sensor[8] = {0};
uint8_t mux_id; 


 
SoftwareSerial mySerial(11, 10); // RX:D11, TX:D10 
ESP8266 wifi(mySerial); 


int relay = 40;


char free_text[100] = {0};



void setup(void) 
{ 
    pinMode(dust_check, INPUT);
    starttime = millis();//get the current time;
    pinMode(vibration_check, INPUT);
    attachInterrupt(1, blink, FALLING);// Trigger the blink function when the falling edge is detected
    pinMode(ReadUVintensityPin, INPUT);
    
     Serial.begin(9600);               
    Serial.print("setup begin\r\n"); 
 
    Serial.print("FW Version:"); 
    Serial.println(wifi.getVersion().c_str()); 
       
    if (wifi.setOprToStationSoftAP()) { 
        Serial.print("to station + softap ok\r\n"); 
    } else { 
        Serial.print("to station + softap err\r\n"); 
    } 
  
    if (wifi.joinAP(SSID, PASSWORD)) { 
        Serial.print("Join AP success\r\n"); 
        Serial.print("IP:"); 
        Serial.println( wifi.getLocalIP().c_str());        
    } else { 
        Serial.print("Join AP failure\r\n"); 
    } 
     
    if (wifi.disableMUX()) { 
        Serial.print("single ok\r\n"); 
    } else { 
        Serial.print("single err\r\n"); 
    } 
      
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) { 
       Serial.print("create tcp ok\r\n"); 
    } else { 
       Serial.print("create tcp err\r\n"); 
    } 
 
      Serial.print("setup end\r\n"); 

    scale.set_scale();
    scale.tare();  //Reset the scale to 0
  
    long zero_factor = scale.read_average(); //Get a baseline reading


    SPI.begin();
    pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
    digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
    pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
    digitalWrite(NRSTPD, HIGH);


    myRFID.AddicoreRFID_Init();  
    wifi.send("D,3,20\n",8); 

} 
 


 
  
void loop(void) 
{
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];

    status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str); 
    status = myRFID.AddicoreRFID_Anticoll(str);

    if (status != MI_OK) str[0] = NULL;
 
     myRFID.AddicoreRFID_Halt();        //Command tag into hibernation              

//////////////////////////////////////////////////////////////////rfid


    vibration_state = 0;
    duration = pulseIn(dust_check, LOW);
    lowpulseoccupancy = lowpulseoccupancy + duration;

   char send_message[100]={0}; 
 
    concentration = 0;

    scale.set_scale(calibration_factor); //Adjust to this calibration factor
    
    if ((millis() - starttime) > sampletime_ms) //if the sampel time == 30s
    {
      ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=>100
      concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
    /*  Serial.print("concentration = ");
      Serial.print(concentration);
      Serial.println(" pcs/0.01cf");
      Serial.println("\n");*/
      lowpulseoccupancy = 0;
      starttime = millis();
    }

     rain_sensorstate[0] = analogRead(rain_check1);
     rain_sensorstate[1] = analogRead(rain_check2);

     human_sensorstate[0] = digitalRead(human_check1);
     human_sensorstate[1] = digitalRead(human_check2);
     
     voltage1 = readVoltage_1();
     voltage2 = readVoltage_2();

     int uvLevel = averageAnalogRead(ReadUVintensityPin);
     float outputVoltage = 5.0 * uvLevel / 1024;
     //float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);
     int UV;
     int analogValue = analogRead(ReadUVintensityPin);
     float temp = 0, humi = 0;

     if(dht7.read(humi,temp) != 0)
     humi = temp = 0;

     if (analogValue<20)
    {
      UV = 0;
    }
   else
    {
      UV = 0.05*analogValue-1;
     }

    if(state!=0)
    {
       state = 0;
       vibration_state = 1;
    }

  current_Value1 = analogRead(current_check1);
  current_Value2 = analogRead(current_check2);
  
  current_Value1  = (current_Value1 * 27.03 / 1023) * 100;
  current_Value2  = (current_Value2 * 27.03 / 1023) * 100;
  
  for(int i = 0 ; i< 8 ; i++)
  {
    light_sensor[i] = analogRead((7+i));
   }

    
  sprintf(send_message,"D,%d,%d,  %d,%d,  %d,%d,  %d,%d, %d, %d, %d,%d,  %d,%d,  %d,  %d,%d,%d,%d,%d,%d,%d,%d \n",
                                rain_sensorstate[0], rain_sensorstate[1],
                                human_sensorstate[0], human_sensorstate[1],
                                vibration_state, (int)concentration,
                                (int)(voltage1*100) , UV,
                                (int)(scale.get_units()*45),
                                (int)str[0], (int)temp, (int)humi,current_Value1, current_Value2,
                                (int)(voltage2*100),
                                light_sensor[0],light_sensor[1],light_sensor[2],light_sensor[3],
                                light_sensor[4],light_sensor[5],light_sensor[6],light_sensor[7]
                                );
                               
      if(Serial.available())
      {
        char temp = Serial.read();
        
        if(temp == '+' || temp == 'a')
          calibration_factor += 10;
          
        else if(temp == '-' || temp == 'z')
          calibration_factor -= 10;
      }


    if(strcmp(send_message, free_text) != 0)
    {      
      wifi.send(send_message,strlen(send_message));
      strcpy(free_text, send_message);
      Serial.print(send_message);
    }

  delay(500);
} 

float readVoltage_1()
{
  voltage_sensorValue1 = analogRead(voltage_check1);

  vout1 = (voltage_sensorValue1 * 4.78) / 1024.0;

  if(vout1 <= 0.7)
    vout1 = 0;
  else
     vout1 -= 0.07;
    
  vin1 = vout1 / (R2_1 / (R1_1 + R2_1));
  return vin1;
}

float readVoltage_2()
{
  voltage_sensorValue2 = analogRead(voltage_check2);

  vout2 = (voltage_sensorValue2 * 4.78) / 1024.0;

  if(vout2 <= 0.7)
    vout2 = 0;
  else
     vout2 -= 0.07;
    
  vin2 = vout2 / (R2_2 / (R1_2 + R2_2));
  return vin2;
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
    
  runningValue /= numberOfReadings;

  return (runningValue);

}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min ;
}


void blink()//Interrupts function
{
  state++;
}
