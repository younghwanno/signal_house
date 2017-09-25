

#include "ESP8266.h" 
#include <DHT11.h>
#include <SoftwareSerial.h> 

 
#define SSID        "test"
#define HOST_NAME   "192.168.43.64"
#define HOST_PORT   8082
#define PASSWORD    "123456789" 

#define TH_1 22
#define TH_2 23
#define TH_3 24
#define TH_4 25
#define TH_5 26
#define TH_6 27
#define BUTTON_IN 40

DHT11 dht1(TH_1); 
DHT11 dht2(TH_2);
DHT11 dht3(TH_3); 
DHT11 dht4(TH_4); 
DHT11 dht5(TH_5); 
DHT11 dht6(TH_6); 



#define MQ_PIN_1 A0 //define which analog input channel you are going to use
#define MQ_PIN_5 A4 // LPG 전용

int RL_VALUE  = 5; //define the load resistance on the board, in kilo ohms
#define RO_CLEAN_AIR_FACTOR 9.83 //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,

#define CALIBARAION_SAMPLE_TIMES 50 //define how many samples you are going to take in the calibration phase
#define CALIBRATION_SAMPLE_INTERVAL 500 //define the time interal(in milisecond) between each samples in the

#define READ_SAMPLE_INTERVAL 50 //define how many samples you are going to take in normal operation
#define READ_SAMPLE_TIMES 1 //define the time interal(in milisecond) between each samples in

#define GAS_LPG 0
#define GAS_CO 1
#define GAS_SMOKE 2

float LPGCurve[3] = {2.3,0.21,-0.47}; //two points are taken from the curve.
float COCurve[3] = {2.3,0.72,-0.34}; //two points are taken from the curve.
float SmokeCurve[3] ={2.3,0.53,-0.44}; //two points are taken from the curve.
float Ro = 10; //Ro is initialized to 10 kilo ohms


int flame_checks[5];
int flame_sensorstate[5] = {0};

uint8_t mux_id; 


int button_state;

 
SoftwareSerial mySerial(11, 10); // RX:D11, TX:D10 
ESP8266 wifi(mySerial); 


float temp[10]={0}, humi[10]={0};
int LPG[2] = {0};
int CO[2] = {0};
int SMOKE[2] = {0};
int relay = 40;
char free_text[100] = {0};


void setup(void) 
{   
    Serial.begin(9600); 
    
    
    Serial.print("Calibration is done...\n");
    Serial.print("Ro=");
    Serial.print(Ro);
    Serial.print("kohm");
    Serial.print("\n");
                 
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


     for(int i= 0 ; i < 5 ;i++)
     {
     flame_checks[i] = i + 30;
     pinMode(flame_checks[i], INPUT);
     }
     
     wifi.send("C,3,20\n",8); 

    pinMode(BUTTON_IN, INPUT_PULLUP); 

} 
 

 
  
void loop(void) 
{

   char send_message[150]; 

   int err;


  if( (err=dht1.read(humi[0], temp[0])) == 0 && (err=dht2.read(humi[1], temp[1])) == 0 && (err=dht3.read(humi[2], temp[2])) == 0 && (err=dht4.read(humi[3], temp[3])) == 0 && (err=dht5.read(humi[4], temp[4])) == 0 && (err=dht6.read(humi[5], temp[5])) == 0)
  {
    LPG[1] = MQGetGasPercentage(MQRead(MQ_PIN_5)/Ro,GAS_LPG);
    CO[0] = MQGetGasPercentage(MQRead(MQ_PIN_1)/Ro,GAS_CO);
    SMOKE[0] = MQGetGasPercentage(MQRead(MQ_PIN_1)/Ro,GAS_SMOKE);

    for(int i = 0 ; i < 4 ;i++)
    {
      flame_sensorstate[i] = digitalRead(flame_checks[i]);
      
      if(flame_sensorstate[i] == HIGH) flame_sensorstate[i] = 0;
      else flame_sensorstate[i] = 1;
    }


     if(digitalRead(BUTTON_IN) == LOW) button_state = 1;
     else button_state = 0;
      
    sprintf(send_message,"C,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,  %d,%d,  %d,%d,%d,%d, %d, %d\n",
                                  (int)temp[0],(int)humi[0],(int)temp[1],(int)humi[1],(int)temp[2],(int)humi[2],
                                  (int)temp[3],(int)humi[3],(int)temp[4],(int)humi[4],(int)temp[5],(int)humi[5],
                                  CO[0],SMOKE[0],
                                  flame_sensorstate[0],
                                  flame_sensorstate[1],
                                  flame_sensorstate[2],
                                  flame_sensorstate[3],
                                  LPG[1],
                                  button_state
                                  );
                                  
    if(strcmp(send_message, free_text) != 0)
    {
      wifi.send(send_message,strlen(send_message));
      strcpy(free_text, send_message);
      Serial.print(send_message);
    }
    
  }
  
  delay(500);
} 



float MQCalibration(int mq_pin)
{
  int i;
  float val=0;
  
  for (i=0;i< CALIBARAION_SAMPLE_TIMES ;i++)
  {
    val += MQResistanceCalculation(analogRead(mq_pin));
  }
  val = val/CALIBARAION_SAMPLE_TIMES; //calculate the average value
  
  val = val/RO_CLEAN_AIR_FACTOR; //divided by RO_CLEAN_AIR_FACTOR yields the Ro
  //according to the chart in the datasheet
  
  return val;
}




float MQResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}



float MQRead(int mq_pin)
{
  int i;
  float rs=0;
  
  for (i=0;i< READ_SAMPLE_TIMES ;i++)
  {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    //delay(READ_SAMPLE_INTERVAL);
  }
  
  rs = rs/READ_SAMPLE_TIMES;
  
  return rs;
}


int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{    
  if ( gas_id == GAS_LPG )
  {
    return MQGetPercentage(rs_ro_ratio,LPGCurve);
  }
  else if ( gas_id == GAS_CO )
  {
    return MQGetPercentage(rs_ro_ratio,COCurve);
  } 
  else if ( gas_id == GAS_SMOKE )
  {
    return MQGetPercentage(rs_ro_ratio,SmokeCurve);
}

  return 0;
}


int MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  int temp = (pow(10,( ((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0])));
  return temp; 
}


