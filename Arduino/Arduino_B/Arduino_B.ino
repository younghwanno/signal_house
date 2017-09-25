/*
   Arduino B
*/
#include "ESP8266.h"
#include <SoftwareSerial.h>
#include <Servo.h>

#define SSID        "test"
#define HOST_NAME   "192.168.43.64"
#define HOST_PORT   8082
#define PASSWORD    "123456789"

#define redpin_1 2
#define greenpin_1 3
#define bluepin_1 4

//////////////화장실
#define redpin_2 5
#define greenpin_2 6
#define bluepin_2 7

//////////////방2
#define redpin_3 8
#define greenpin_3 12
#define bluepin_3 13

#define redpin_4 9
#define greenpin_4 50
#define bluepin_4 51

#define relay5 40 // 2번째 릴레이 in 2, 화장실 환풍기
#define relay6 41 // 태양광(solar) - 축전지(battery)
#define relay7 42 // 태양광(solar) - 6V DC
#define relay8 43 // 축전지(battery) - 6V DC

Servo myservo[10];
int servos_state[10] = {0,};

int pos = 0;
int len, i;
uint8_t mux_id;
char buffer[50] = {0};
int mode, r = 0 , g = 0, b = 0, t = 0;

int comma_position[10];
String tokens[10];
int strlength;
int str_start, str_end;
int bath_room_ventilating_fan = 0;


int battery1_relay = 1;
int battery2_relay = 1;


SoftwareSerial mySerial(11, 10); /* RX:D11, TX:D10 */
ESP8266 wifi(mySerial);


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

  pinMode(redpin_4, OUTPUT);
  pinMode(bluepin_4, OUTPUT);
  pinMode(greenpin_4, OUTPUT);

  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);


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

  digitalWrite(relay5, LOW);
  digitalWrite(relay6, LOW);
  digitalWrite(relay7, LOW);
  //digitalWrite(relay8, HIGH);

  wifi.send("B,3,20\n", 8);
}



void loop(void)
{
  i = 0;
  str_start = 0;
  str_end = 0;

  memset(buffer, 0, sizeof(buffer));
  len = wifi.recv(buffer, sizeof(buffer), 300);

  if (len > 0)
  {
    String all(buffer);
    Serial.print(all);

    while ((comma_position[i] = all.indexOf(',', str_start)) != -1)
    {
      tokens[i] = all.substring(str_start, comma_position[i]);
      str_start = comma_position[i] + 1;

      i++;
    }

    char type = tokens[0].charAt(0);
    
    switch (type)
    {
      case 'X' :
        {
          control_LED(redpin_4, greenpin_4, bluepin_4, tokens[1].toInt(), tokens[2].toInt(), tokens[2].toInt(), tokens[2].toInt(), tokens[2].toInt());
          break;
        }

      case 'A' :
        {
          control_LED(redpin_1, greenpin_1, bluepin_1, tokens[1].toInt(), tokens[2].toInt(), tokens[3].toInt(), tokens[4].toInt(), tokens[5].toInt());
          break;
        }
      case 'B' :
        {
          control_LED(redpin_2, greenpin_2, bluepin_2, tokens[1].toInt(), tokens[2].toInt(), tokens[3].toInt(), tokens[4].toInt(), tokens[5].toInt());
          break;
        }
      case 'C' :
        {
          control_LED(redpin_3, greenpin_3, bluepin_3, tokens[1].toInt(), tokens[2].toInt(), tokens[3].toInt(), tokens[4].toInt(), tokens[5].toInt());
          break;
        }
      case 'E' :
        {
          Serial.println("control..livingroom windows!!");

          

          if(tokens[1].toInt() == 0 && tokens[2].toInt() == 0)
          {
            if (servos_state[0] == 1 && servos_state[1] == 1)
            {
              myservo[0].attach(22);
              myservo[1].attach(23);
              delay(300);
              myservo[0].write(0);
              myservo[1].write(0);
              
              myservo[0].detach();
              myservo[1].detach();
              
              servos_state[0] = 0;
              servos_state[1] = 0;
            }
          }
          else if (tokens[1].toInt() == 1 && tokens[2].toInt() == 1)
          {
            if (servos_state[0] == 0 && servos_state[1] == 0)
            {
              myservo[0].attach(22);
              myservo[1].attach(23);
              delay(300);
              myservo[0].write(90);
              myservo[1].write(90);
              
              myservo[0].detach();
              myservo[1].detach();
              
              servos_state[0] = 1;
              servos_state[1] = 1;
            }
          }

          //////////////////////////Window 1
          if (tokens[1].toInt() == 1)
          {
            if (servos_state[0] == 0)
            {
              myservo[0].attach(22);
              delay(300);
              myservo[0].write(90);
              myservo[0].detach();
              servos_state[0] = 1;
            }
          }
          else if (tokens[1].toInt() == 0)
          {
            if (servos_state[0] == 1)
            {
              myservo[0].attach(22);
              delay(300);
              myservo[0].write(0);
              myservo[0].detach();
              servos_state[0] = 0;
            }
          }

          ///////////////////////////////Window 2
          if (tokens[2].toInt() == 1)
          {
            if (servos_state[1] == 0)
            {
              myservo[1].attach(23);
              delay(300);
              myservo[1].write(90);
              myservo[1].detach();
              servos_state[1] = 1;
            }
          }
          else if (tokens[2].toInt() == 0)
          {
            if (servos_state[1] == 1)
            {
              myservo[1].attach(23);
              delay(300);
              myservo[1].write(0);
              myservo[1].detach();
              servos_state[1] = 0;
            }
          }
         
          break;
        }
      case 'F' :
        {
          //////////////////////////Door
          if (tokens[1].toInt() == 1)
          {
            if (servos_state[3] == 0)
            {
              myservo[3].attach(25);
              delay(300);
              myservo[3].write(90);
              myservo[3].detach();
              servos_state[3] = 1;
            }
          }
          else if (tokens[1].toInt() == 0)
          {
            if (servos_state[3] == 1)
            {
              myservo[3].attach(25);
              delay(300);
              myservo[3].write(180);
              myservo[3].detach();
              servos_state[3] = 0;
            }
          }

          break;
        }
      case 'G' :
        {

          if (tokens[2].toInt() == 1)
          {


            if (servos_state[4] == 0)
            {
              myservo[4].attach(26);
              delay(300);
              myservo[4].write(90);
              myservo[4].detach();
              servos_state[4] = 1;
            }
          }
          else if (tokens[2].toInt() == 0)
          {
            if (servos_state[4] == 1)
            {
              myservo[4].attach(26);
              delay(300);
              myservo[4].write(180);
              myservo[4].detach();
              servos_state[4] = 0;
            }

            //////////////////////////Window 1
            if (tokens[1].toInt() == 1)
            {
              if (servos_state[2] == 0)
              {
                myservo[2].attach(24);
                delay(300);
                myservo[2].write(90);
                myservo[2].detach();
                servos_state[2] = 1;
              }
            }
            else if (tokens[1].toInt() == 0)
            {
              if (servos_state[2] == 1)
              {
                myservo[2].attach(24);
                delay(300);
                myservo[2].write(180);
                myservo[2].detach();
                servos_state[2] = 0;
              }
            }
          }

          break;
        }
      case 'H' :
        {
          //Serial.println("control..Entrance Door!!");

          if (tokens[1].toInt() == 1)
          {
            if (servos_state[5] == 0)
            {
              myservo[5].attach(27);
              delay(300);
              myservo[5].write(90);
              myservo[5].detach();
              servos_state[5] = 1;
            }
          }
          else if (tokens[1].toInt() == 0)
          {
            if (servos_state[5] == 1)
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

      case 'N' :
        {
          ///Window 전체제어

          if (tokens[1].toInt() == 1)
          {
            for (int i = 0; i < 2; i++)
            {
              if (servos_state[i] == 0)
              {
                myservo[i].attach(i + 22);
              }
            }
            
            delay(300);
            
            for (int i = 0 ; i < 2 ; i++)
            {
              if (servos_state[i] == 0)
              {
                myservo[i].write(180);
                servos_state[i] = 1;
              }
            }
            
            for (int i = 0; i < 2; i++)
            {
              if (servos_state[i] == 0)
              {
                myservo[i].detach();
              }
            }
          }
          else if (tokens[1].toInt() == 0)
          {
            for (int i = 0; i < 2; i++)
            {
              if (servos_state[i] == 1)
              {
                myservo[i].attach(i + 22);
              }
            }
            
            delay(300);
            
            for (int i = 0; i < 2 ; i++)
            {
              if (servos_state[i] == 1)
              {
                myservo[i].write(90);
                servos_state[i] = 0;
              }
            }
            for (int i = 0; i < 2; i++)
            {
              if (servos_state[i] == 1)
              {
                myservo[i].detach();
              }
            }
          }

          break;
        }
      case 'M' :
        {
          //Door전제 체어

          if (tokens[1].toInt() == 1)
          {
            for (int i = 2; i < 6; i++)
            {
              if (servos_state[i] == 0)
              {
                myservo[i].attach(i + 22);
              }
            }
            
            delay(300);
            
            for (int i = 2 ; i < 6 ; i++)
            {
              if (servos_state[i] == 0)
              {
                myservo[i].write(180);
                servos_state[i] = 1;
              }
            }
            for (int i = 2; i < 6; i++)
            {
              if (servos_state[i] == 0)
              {
                myservo[i].detach();
              }
            }
          }
          else if (tokens[1].toInt() == 0)
          {
            for (int i = 2; i < 6; i++)
            {
              if (servos_state[i] == 1)
              {
                myservo[i].attach(i + 22);
              }
            }
            
            delay(300);
            
            for (int i = 2; i < 6 ; i++)
            {
              if (servos_state[i] == 1)
              {
                myservo[i].write(90);
                servos_state[i] = 0;
              }
            }
            for (int i = 2; i < 6; i++)
            {
              if (servos_state[i] == 1)
              {
                myservo[i].detach();
              }
            }
          }

          break;
        }
      case 'O' :
        {
          //화장실 환풍기 제어

          if (tokens[1].toInt() == 1)
          {
            if (!bath_room_ventilating_fan)
            {
              digitalWrite(relay5, LOW);
              bath_room_ventilating_fan = 1;
            }
          }
          else if (tokens[1].toInt() == 0)
          {
            if (bath_room_ventilating_fan)
            {
              digitalWrite(relay5, HIGH);
              bath_room_ventilating_fan = 0;
            }
          }

          break;
        }
      case 'P' :
        {
          ////배터리 1 릴레이
          if (tokens[1].toInt() == 1)
          {
            if (battery1_relay == 0)
            {
              digitalWrite(relay6, LOW);
              battery1_relay = 1;
            }
          }
          else
          {
            if (battery1_relay == 1)
            {
              digitalWrite(relay6, HIGH);
              battery1_relay = 0;
            }
          }
/*
          ////배터리 2 릴레이
          if(tokens[2].toInt() == 1)
          {
            if (battery2_relay == 0)
            {
              digitalWrite(relay7, LOW);
              battery2_relay = 1;
            }
          }
          else
          {
            if(battery2_relay == 1)
            {
              digitalWrite(relay7, HIGH);
              battery2_relay = 0;
            }
          }
*/

          break;
        }
      case 'Q' : //패널 1 상하
        {
          myservo[6].attach(28);
          delay(300);
          myservo[6].write(tokens[1].toInt());
          myservo[6].detach();

          break;
        }
        
      case 'Z' : // 패널 1 좌우
        {
          myservo[7].attach(29);
          delay(300);
          myservo[7].write(tokens[1].toInt());
          myservo[7].detach();

          break;
        }
        
      case 'V' : // 패널 2 상하
        {
          myservo[8].attach(30);
          delay(300);
          myservo[8].write(tokens[1].toInt());
          myservo[8].detach();

          break;
        }     
        
      case 'S' : // 패널 2 좌우
        {
          myservo[9].attach(31);
          delay(300);
          myservo[9].write(tokens[1].toInt());
          myservo[9].detach();

          break;
        }     
                   
      case 'R' :
        {
          control_LED(redpin_1, greenpin_1, bluepin_1, 3, 255, 0, 0, 0);
          control_LED(redpin_2, greenpin_2, bluepin_2, 3, 255, 0, 0, 0);
          control_LED(redpin_3, greenpin_3, bluepin_3, 3, 255, 0, 0, 0);
          break;
        }
    }
  }
}

void control_LED(int r_pin, int g_pin, int b_pin, int mode, int R, int G, int B, int T)
{

  switch (mode)
  {
    case 0:
    case 1:
      {
        analogWrite(r_pin, 0);
        analogWrite(g_pin, 0);
        analogWrite(b_pin, 0);

        break;
      }

    case 2:
      {
        analogWrite(r_pin, T);
        analogWrite(g_pin, T);
        analogWrite(b_pin, T);

        break;
      }
    case 3:
      {
        analogWrite(r_pin, R);
        analogWrite(g_pin, G);
        analogWrite(b_pin, B);

        break;
      }

  }
}
