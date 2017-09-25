#include "ESP8266.h" 
#include <SoftwareSerial.h> 
#include <Servo.h> 
 
#define SSID        "test"
#define HOST_NAME   "192.168.43.64"
#define HOST_PORT   8082
#define PASSWORD    "123456789" 

////////////////////다목적 led
#define redpin_1 8
#define greenpin_1 12
#define bluepin_1 13

////////////////////방1 led
#define redpin_2 5 
#define greenpin_2 6 
#define bluepin_2 7

/////////////////////부엌 led
#define redpin_3 2 
#define greenpin_3 3
#define bluepin_3 4

#define Buzzer1 30
#define Buzzer2 31

#define relay1 40 // Water_Pump
#define relay2 41 // Kitchen_Ventile Fan

#define relay3 42 // boiler
#define relay4 43 // fire_fight


Servo myservo[10];
int servos_state[10] = {0,};

int pos = 0; 
int len,i; 
uint8_t mux_id; 
char buffer[50] = {0};
int mode, r =0 ,g = 0, b =0, t =0;

int comma_position[10];
String tokens[10];
int strlength;
int str_start,str_end;

int boiler_state = 0;
int heating_state = 0;
int kitchen_ventilating_fan = 0;
int buzzer_state = 0;
int melody_index = 0;
int fire_fight1 = 0;
int fire_fight2 = 0;
int gas_fight = 0;


unsigned long starttime;
unsigned long sampletime_ms = 5000;//sampe 5s ;
 
SoftwareSerial mySerial(11, 10); /* RX:D11, TX:D10 */   
ESP8266 wifi(mySerial); 

int length = 25; // \uc74c\uc758 \uac1c\uc218
char notes1[] = "ddddddddddddddddddddddddd";
char notes2[] = "bbbbbbbbbbbbbbbbbbbbbbbbb";


int beats1[] = {4,4,4,4,4,4,4,
               4,4,4,4,4,4,
               4,4,4,4,4,4,4,
               4,4,4,4,4};
               
int beats2[] = {3,1,2,2,2,2,4,
               2,2,4,2,2,4,
               3,1,2,2,2,2,4,
               2,2,3,1,8};
               
int tempo = 100;



void setup(void) 
{ 
     Serial.begin(9600); 

     pinMode(redpin_1, OUTPUT); 
     pinMode(bluepin_1, OUTPUT); 
     pinMode(greenpin_1, OUTPUT); 
 
     pinMode(redpin_2, OUTPUT); 
     pinMode(bluepin_2, OUTPUT); 
     pinMode(greenpin_2, OUTPUT); 

     pinMode(redpin_3, OUTPUT); 
     pinMode(bluepin_3, OUTPUT); 
     pinMode(greenpin_3, OUTPUT); 
     
     pinMode(Buzzer1, OUTPUT);
    pinMode(Buzzer2 ,OUTPUT);

    pinMode(relay1, OUTPUT); 
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT); 
    pinMode(relay4, OUTPUT);

    myservo[7].write(180);
                 
    Serial.print("setup begin\r\n"); 
 
    Serial.print("FW Version:"); 
    Serial.println(wifi.getVersion().c_str()); 
       
    if (wifi.setOprToStationSoftAP())
    { 
        Serial.print("to station + softap ok\r\n"); 
    } 
    else 
    { 
        Serial.print("to station + softap err\r\n"); 
    } 
  
    if (wifi.joinAP(SSID, PASSWORD))
    { 
        Serial.print("Join AP success\r\n"); 
        Serial.print("IP:"); 
        Serial.println( wifi.getLocalIP().c_str());        
    } 
    else
    { 
        Serial.print("Join AP failure\r\n"); 
    } 
     
    if (wifi.disableMUX())
    { 
        Serial.print("single ok\r\n"); 
    } 
    else 
    { 
        Serial.print("single err\r\n"); 
    } 
      
    if (wifi.createTCP(HOST_NAME, HOST_PORT))
    { 
       Serial.print("create tcp ok\r\n"); 
    }
    else
    { 
       Serial.print("create tcp err\r\n"); 
    } 
 
      Serial.print("setup end\r\n");

       digitalWrite(relay1,HIGH);
       digitalWrite(relay2,HIGH);
       digitalWrite(relay3,HIGH);
       digitalWrite(relay4,HIGH); // 화재 진압 펌프
       
       wifi.send("A,3,20\n",8); 
} 
 

 
  
void loop(void) 
{


if( buzzer_state == 1)
{

    if ((millis() - starttime) > sampletime_ms ) //if the sampel time == 30s
    {
      starttime = millis();
      
      digitalWrite(relay2,HIGH);       
      buzzer_state = 0;
      melody_index = 0;
    }
    else
    {
           
      if(notes1[melody_index] == ' ')
      {
        delay(beats1[melody_index] * tempo);
      }
      else
      {
        playNote(notes1[melody_index], beats1[melody_index] * tempo);
      }

      melody_index++;
     }
}


  i = 0;
  str_start = 0;
  str_end = 0;

  memset(buffer,0,sizeof(buffer));
  len = wifi.recv(buffer, sizeof(buffer),300);
  
  if(len > 0)
  {
    String all(buffer);
    Serial.print(all);

      while((comma_position[i] = all.indexOf(',',str_start)) != -1)
      {
        tokens[i] = all.substring(str_start,comma_position[i]);
        str_start = comma_position[i] + 1;
        
        i++;
      }

      char type = tokens[0].charAt(0);
      
      switch(type)
      {
        case 'A' :
        {
          control_LED(redpin_1,greenpin_1,bluepin_1,tokens[1].toInt(),tokens[2].toInt(),tokens[3].toInt(),tokens[4].toInt(),tokens[5].toInt());
          break;
        }
        case 'B' :
        {
          control_LED(redpin_2,greenpin_2,bluepin_2,tokens[1].toInt(),tokens[2].toInt(),tokens[3].toInt(),tokens[4].toInt(),tokens[5].toInt());
          break; 
        }
        case 'C' :
        {
          control_LED(redpin_3,greenpin_3,bluepin_3,tokens[1].toInt(),tokens[2].toInt(),tokens[3].toInt(),tokens[4].toInt(),tokens[5].toInt());
          break;
        }
        case 'E' :
        {
          if(tokens[1].toInt() == 1)
          {
            if(servos_state[0] == 0)
            {
              myservo[0].attach(22);
              delay(300);
              myservo[0].write(90);
              myservo[0].detach();
              servos_state[0] = 1;
            }
          }
          else if(tokens[1].toInt() == 0)
          {
            if(servos_state[0] == 1)
            {
              myservo[0].attach(22);
              delay(300);
              myservo[0].write(180);
              myservo[0].detach();
              servos_state[0] = 0;
            }
          }
  
          ///////////////////////////////Window 2
          if(tokens[2].toInt() == 1)
          {
            if(servos_state[1] == 0)
            {
              myservo[1].attach(23);
              delay(300);
              myservo[1].write(90);
              myservo[1].detach();
              servos_state[1] = 1;
            }
          }
          else if(tokens[2].toInt() == 0)
          {
            if(servos_state[1] == 1)
            {
              myservo[1].attach(23);
              delay(300);
              myservo[1].write(180);
              myservo[1].detach();
              servos_state[1] = 0;
            }
          }
  
     
        ///////////////////////////////Door
          if(tokens[3].toInt() == 1)
          {
            if(servos_state[5] == 0)
            {
              myservo[5].attach(27);
              delay(300);
              myservo[5].write(0);
              myservo[5].detach();
              servos_state[5] = 1;
            }
          }
          else if(tokens[3].toInt() == 0)
          {        
            if(servos_state[5] == 1)
            {
              myservo[5].attach(27);
              delay(300);
              myservo[5].write(180);
              myservo[5].detach();
              servos_state[5] = 0;
            }
          } 
           
          break;
        }
        case 'F' :
        {
          ///////////////////////////////Window1
          if(tokens[1].toInt() == 1)
          {
            if(servos_state[2] == 0)
            {
              myservo[2].attach(24);
              delay(300);
              myservo[2].write(90);
              myservo[2].detach();
              servos_state[2] = 1;
            }
          }
          else if(tokens[1].toInt() == 0)
          {
            if(servos_state[2] == 1)
            {
              myservo[2].attach(24);
              delay(300);
              myservo[2].write(180);
              myservo[2].detach();
              servos_state[2] = 0;
            }
          }
       
        ///////////////////////////////Door
          if(tokens[2].toInt() == 1)
          {
            if(servos_state[6] == 0)
            {
              myservo[6].attach(28);
              delay(300);
              myservo[6].write(90);
              myservo[6].detach();
              servos_state[6] = 1;
            }
          }
          else if(tokens[2].toInt() == 0)
          {         
            if(servos_state[6] == 1)
            {
              myservo[6].attach(28);
              delay(300);
              myservo[6].write(180);
              myservo[6].detach();
              servos_state[6] = 0;
            }
          } 
                 
          break;
        }
        case 'G' :
        {
          if(tokens[1].toInt() == 1)
          {
            if(servos_state[3] == 0)
            {
              myservo[3].attach(25);
              delay(300);
              myservo[3].write(90);
              myservo[3].detach();
              servos_state[3] = 1;
            }
          }
          else if(tokens[1].toInt() == 0)
          {
            if(servos_state[3] == 1)
            {
              myservo[3].attach(25);
              delay(300);
              myservo[3].write(180);
              myservo[3].detach();
              servos_state[3] = 0;
            }
          }
  
          ///////////////////////////////Window 2
          if(tokens[2].toInt() == 1)
          {
            if(servos_state[4] == 0)
            {
              myservo[4].attach(26);
              delay(300);
              myservo[4].write(90);
              myservo[4].detach();
              servos_state[4] = 1;
            }
          }
          else if(tokens[2].toInt() == 0)
          {
            if(servos_state[4] == 1)
            {
              myservo[4].attach(26);
              delay(300);
              myservo[4].write(180);
              myservo[4].detach();
              servos_state[4] = 0;
            }
          }
        
          break;
        }
        case 'H' :
        {
        //Serial.println("control..Entrance Door!!");       
        break;
        }
        case 'J' :
        {     
          
           if(tokens[5].toInt() != 0)
           {
              if(gas_fight == 0)
              {
                digitalWrite(relay2,LOW); 
                gas_fight = 1;
              }
           }
           else
           {         
              if(gas_fight != 0)
              {
                digitalWrite(relay2,HIGH);
                gas_fight = 0;
              }
           }    

            //////////////////////불꽃 감지 시 진공펌프 작동        
           if(tokens[4].toInt() != 0)
           {
              if(fire_fight2 == 0)
              {
                digitalWrite(relay4,LOW); 
               fire_fight2 = 1;
              }
           }
           else
           {         
              if(fire_fight2 != 0)
              {
                digitalWrite(relay4,HIGH);
                fire_fight2 = 0;
              }
           }     
                   
           
           if(tokens[3].toInt() != 0)
           {
              if(fire_fight1 == 0)
              {
                 fire_fight1 = 1;          
                 control_LED(redpin_1,greenpin_1,bluepin_1,3,255,0,0,0);
                 control_LED(redpin_2,greenpin_2,bluepin_2,3,255,0,0,0);
                 control_LED(redpin_3,greenpin_3,bluepin_3,3,255,0,0,0);             
              }
           }
           else
           {
              if(fire_fight1 != 0)
              {
                fire_fight1 = 0;
              }
           }                 

          if(tokens[1] || tokens[2])
          {       
             starttime = millis();              
             buzzer_state = 1;
          }

    
          break;
        }     
        case 'K' :
        {
           if(tokens[1].toInt() == 1)
           {
              if(!heating_state)
              {
                digitalWrite(relay1,LOW);
                heating_state = 1;
              }
           }
           else
           {
              if(heating_state)
              {
                digitalWrite(relay1,HIGH);
                heating_state = 0;
              }
           }

          break;
        }
        case 'L' :
        {
           if(tokens[1].toInt() == 1)
           {
              if(!boiler_state)
              {
                digitalWrite(relay3,LOW);
                boiler_state = 1;
              }
           }
           else
           {
              if(boiler_state)
              {
                digitalWrite(relay3,HIGH);
                boiler_state = 0;
              }
           }  
          break;
        }
        case 'N' :
        {
          if(tokens[1].toInt() == 1)
          {
            for(int i=0; i<5;i++)
            {
              if(servos_state[i] == 0)
              {
                myservo[i].attach(i+22);
              }
            }
            delay(300);
            for(int i = 0 ; i<5 ;i++)
            {
                if(servos_state[i] == 0)
                {
                  myservo[i].write(180);
                  servos_state[i] = 1;
                }
            }
            for(int i=0; i<5;i++){
              if(servos_state[i] == 0)
              {
                myservo[i].detach();
              }
            }
          }
          else if(tokens[1].toInt() == 0)
          {
            for(int i=0; i<5;i++)
            {
              if(servos_state[i] == 1)
              {
                myservo[i].attach(i+22);
              }
            }
            delay(300);
            for(int i = 0; i < 5 ;i++)
            {
                if(servos_state[i] == 1)
                {
                  myservo[i].write(0);
                  servos_state[i] = 0;
                }
            }
            for(int i=0; i<5;i++)
            {
              if(servos_state[i] == 1)
              {
                 myservo[i].detach();
              }
            }
          }
          
          break;
        }
        case 'M' :
        {
     
            if(tokens[1].toInt() == 1)
            {
              for(int i= 5; i < 7; i++)
              {
                if(servos_state[i] == 0)
                {
                  myservo[i].attach(i+22);
                }
              }
              
              delay(300);
              
              for(int i = 5 ; i < 7 ;i++)
              {
                  if(servos_state[i] == 0)
                  {
                    myservo[i].write(180);
                    servos_state[i] = 1;
                  }
              }
              
              for(int i=5; i<7; i++)
              {
                if(servos_state[i] == 0)
                {
                  myservo[i].detach();
                }
              }
            }
            else if(tokens[1].toInt() == 0)
            {
              for(int i= 5; i < 7; i++)
              {
                if(servos_state[i] == 1)
                {
                  myservo[i].attach(i+22);
                }
              }
              
              delay(300);
              
              for(int i = 5; i < 7 ;i++)
              {
                  if(servos_state[i] == 1)
                  {
                    myservo[i].write(0);
                    servos_state[i] = 0;
                  }
              }
               for(int i=5; i<7; i++)
               {
                if(servos_state[i] == 1)
                {
                  myservo[i].detach();
                }
              }

            }                    
            break;
        }
        case 'O' :
        {
        
            if(tokens[1].toInt() == 1)
            {
                if(!kitchen_ventilating_fan)
                {
                  digitalWrite(relay2,LOW);
                  kitchen_ventilating_fan = 1;              
                } 
            }
            else if(tokens[1].toInt() == 0)
            {
                if(kitchen_ventilating_fan)
                {
                  digitalWrite(relay2,HIGH);
                  kitchen_ventilating_fan = 0;
                }         
            }         
                       
        break;
        }    
        case 'P' :
        {      
          if(tokens[1].toInt() == 1)
          {
            if(servos_state[7] == 0)
            { 
              myservo[7].attach(29);
              myservo[7].write(90);
              delay(300);
              myservo[7].detach();
              servos_state[7] = 1;
            }
          }
          else if(tokens[1].toInt() == 0)
          {
            if(servos_state[7] == 1)
            {
              myservo[7].attach(29);
              myservo[7].write(180);
              delay(300);
              myservo[7].detach();
              servos_state[7] = 0;
            }
          }
          break;
        }
      }  
  }
 
} 





void control_LED(int r_pin,int g_pin, int b_pin, int mode, int R, int G, int B, int T)
{
    
    switch(mode)
    {
      case 0: 
      case 1:
      {   
        analogWrite(r_pin,0); 
        analogWrite(g_pin,0); 
        analogWrite(b_pin,0);
        
        break;
      } 
       
      case 2:
      { 
        analogWrite(r_pin,T); 
        analogWrite(g_pin,T); 
        analogWrite(b_pin,T);
  
        break;
      }
      case 3:
      { 
        analogWrite(r_pin,R); 
        analogWrite(g_pin,G); 
        analogWrite(b_pin,B);
  
        break;
      }    
    }
    
}



void playTone(int tone, int duration) {
  
  for (long i = 0; i < duration * 1000L; i += tone * 2)
  {
      digitalWrite(Buzzer1, HIGH);
      digitalWrite(Buzzer2, HIGH);
      delayMicroseconds(tone);
      digitalWrite(Buzzer1, LOW);
      digitalWrite(Buzzer2, LOW);
      delayMicroseconds(tone);
  }
}


void playNote(char note, int duration) {
  
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' }; 
  int tones[] = { 1915 };

//    if(type == 0)
    {
      for (int i = 0; i < 8; i++)
      {   
        if (names[i] == note)
        { playTone(tones[i], duration); }  
      }
    }
}

