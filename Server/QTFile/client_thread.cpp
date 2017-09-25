#include <QApplication>
#include <QDebug>
#include <QThread>
#include "client_thread.h"
#include "server.h"

CountWorker::CountWorker(QTcpServer* server, QTcpSocket* client)
{
    this->client = client;
    this->server = server;

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}


void CountWorker::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    chatServer *real_server =(chatServer *)server;
    QRegExp rx("(\\,)");


    while(client->canReadLine())
    {
        receive_message = client->readLine();
        QStringList query = receive_message.split(rx);
        QString ab = query[0];

        switch(ab[0].toLatin1())
        {
            case 'A': // 엑추에이터 1
            {
                real_server->get_mutex()->lock();

                if(real_server->client_list[0] == NULL)
                    real_server->client_list[0] = this;

                real_server->get_mutex()->unlock();
                break;
            }

            case 'B': // 엑추에이터 2
            {
                real_server->get_mutex()->lock();

                if(real_server->client_list[1] == NULL)
                    real_server->client_list[1] = this;

                real_server->get_mutex()->unlock();
                break;
            }

            case 'C': // 센서 1
            {
                real_server->get_mutex()->lock();

                if(real_server->client_list[2] == NULL)
                {
                    connect(this, SIGNAL(update_value_C()), real_server, SLOT(update_value_C()));
                    real_server->client_list[2] = this;
                }
                    //온,습도 값 gui에 반영하기..

                    else if(query.length() == 21)
                    {

                    qDebug() <<receive_message;

                    real_server->Living_room.set_temperature(query[1].toInt());
                    real_server->Living_room.set_humidity(query[2].toInt());
                    real_server->Living_room.set_Co2(query[13].toInt());
                    real_server->Living_room.set_Smoke(query[14].toInt());
                    real_server->Living_room.set_Flame(query[15].toInt());


                    real_server->Kitchen.set_temperature(query[3].toInt());
                    real_server->Kitchen.set_humidity(query[4].toInt());
                    //real_server->Kitchen.set_Co2(query[15].toInt());
                   // real_server->Kitchen.set_Smoke(query[16].toInt());
                    real_server->Kitchen.set_Flame(query[16].toInt());
                    real_server->Kitchen.set_LPG(query[19].toInt());



                    real_server->Room1.set_temperature(query[5].toInt());
                    real_server->Room1.set_humidity(query[6].toInt());
                    //real_server->Room1.set_Co2(query[17].toInt());
                   // real_server->Room1.set_Smoke(query[18].toInt());
                    real_server->Room1.set_Flame(query[17].toInt());


                    real_server->Room2.set_temperature(query[7].toInt());
                    real_server->Room2.set_humidity(query[8].toInt());
                   // real_server->Room2.set_Co2(query[19].toInt());
                    //real_server->Room2.set_Smoke(query[20].toInt());
                    real_server->Room2.set_Flame(query[18].toInt());



                    real_server->Bath_room.set_temperature(query[9].toInt());
                    real_server->Bath_room.set_humidity(query[10].toInt());

                    real_server->Multipurpose_room.set_temperature(query[11].toInt());
                    real_server->Multipurpose_room.set_humidity(query[12].toInt());


                    real_server->Entrance.set_knock(query[20].toInt());

                    emit update_value_C();
                    }

                real_server->get_mutex()->unlock();

                break;
            }

            case 'D': // 센서 2
            {
                real_server->get_mutex()->lock();

                if(real_server->client_list[3] == NULL)
                {
                    connect(this, SIGNAL(update_value_D()), real_server, SLOT(update_value_D()));
                    real_server->client_list[3] = this;

                    //qDebug() << receive_message;
                }


                    else if(query.length() == 24)
                    {

                    qDebug() <<receive_message;

                    real_server->ETC.set_rain1(query[1].toInt());
                    real_server->ETC.set_rain2(query[2].toInt());

                    real_server->ETC.set_human1(query[3].toInt());
                    real_server->ETC.set_human2(query[4].toInt());



                    real_server->ETC.set_vibration(query[5].toInt());
                    //real_server->ETC.set_current(query[6].toInt());
                    real_server->ETC.set_dust(query[6].toInt());

                    real_server->ETC.set_voltage(query[7].toInt() / 100.0);
                    real_server->ETC.set_UV(query[8].toInt() / 100.0);
                    real_server->ETC.set_load_cell(query[9].toInt() / 100.0);


                    if(query[10].toInt())
                        real_server->ETC.set_rfid(query[10].toInt());
                    else
                        real_server->ETC.set_rfid(-1);


                    real_server->ETC.set_temperature(query[11].toInt());
                    real_server->ETC.set_humidity(query[12].toInt());


                    real_server->ETC.set_current(query[13].toInt());
                    real_server->ETC.set_current2(query[14].toInt());
                    real_server->ETC.set_voltage2(query[15].toInt());



                    for(int i = 0; i < 8 ; i++)
                    {
                        real_server->ETC.light[i] = query[16 + i].toInt();
                    }




                    emit update_value_D();
                    }
                real_server->get_mutex()->unlock();

                break;
            }

            case 'E': // 안드로이드 클라이언트
            {  //qDebug() << receive_message;
                //real_server->get_mutex()->lock();

               if(real_server->client_list[4] == NULL)
               {
                   connect(this, SIGNAL(update_value_E()), real_server, SLOT(update_value_E()));
                   real_server->client_list[4] = this;


                   if(real_server->get_query() != NULL)
                   {


                       QString tr = query[2];
                       tr.resize(tr.length()-1);

                       qDebug() << query[1];
                       qDebug() << tr;

                       QString qrc =  QString("select count(*) from login_registration_table where login_ID ='%1' and login_PW ='%2'")
                               .arg(query[1]).arg(tr);

                      real_server->get_query()->exec(qrc);
                      QString value;

                      while( real_server->get_query()->next())
                      {
                          int count =  real_server->get_query()->value(0).toInt();

                          if(count == 1)
                          {
                               value = "success\n";
                               qDebug() << value;

                               real_server->set_E_message(value.toStdString().data());
                               emit update_value_E();
                          }
                          else
                          {
                               value = "fail\n";
                               qDebug() << value;

                               real_server->set_E_message(value.toStdString().data());
                               emit update_value_E();

                               this->client->disconnect();
                               this->disconnected();
                               this->destroyed();
                          }
                      }


                   }
               }

               else
               {
                   qDebug() << receive_message;
                   ////////////////////////////////////////////////////거실
                   real_server->Living_room.r_window1 = query[1].toInt();
                   real_server->Living_room.r_window2 = query[2].toInt();

                   if(query[3].toInt() < 2)
                   {
                       real_server->Living_room.r_LED_mode = query[3].toInt();
                       real_server->Living_room.r_LED_OnOff_state = 0;
                   }
                   else
                   {
                       real_server->Living_room.r_LED_OnOff_state = 2;
                       real_server->Living_room.r_LED_mode = query[3].toInt() - 2;
                   }

                   real_server->Living_room.r_LED_R = query[4].toInt();
                   real_server->Living_room.r_LED_G = query[5].toInt();
                   real_server->Living_room.r_LED_B = query[6].toInt();
                   real_server->Living_room.r_LED_total = query[7].toInt();
                   ////////////////////////////////////////////////////부엌
                   real_server->Kitchen.r_window1 = query[8].toInt();
                   real_server->Kitchen.r_window2 = query[9].toInt();
                   real_server->Kitchen.r_ventilating_fan = query[10].toInt();

                   if(query[11].toInt() < 2)
                   {
                       real_server->Kitchen.r_LED_OnOff_state = 0;
                       real_server->Kitchen.r_LED_mode = query[11].toInt();
                   }
                   else
                   {
                       real_server->Kitchen.r_LED_OnOff_state = 2;;
                       real_server->Kitchen.r_LED_mode = query[11].toInt() - 2;
                   }

                   real_server->Kitchen.r_LED_R = query[12].toInt();
                   real_server->Kitchen.r_LED_G = query[13].toInt();
                   real_server->Kitchen.r_LED_B = query[14].toInt();
                   real_server->Kitchen.r_LED_total = query[15].toInt();
                   ////////////////////////////////////////////////////방1
                   real_server->Room1.r_window1 = query[16].toInt();
                   real_server->Room1.r_door = query[17].toInt();

                   if(query[18].toInt() < 2)
                   {
                       real_server->Room1.r_LED_OnOff_state = 0;
                       real_server->Room1.r_LED_mode = query[18].toInt();
                   }
                   else
                   {
                       real_server->Room1.r_LED_OnOff_state = 2;
                       real_server->Room1.r_LED_mode = query[18].toInt() - 2;
                   }

                   real_server->Room1.r_LED_R = query[19].toInt();
                   real_server->Room1.r_LED_G = query[20].toInt();
                   real_server->Room1.r_LED_B = query[21].toInt();
                   real_server->Room1.r_LED_total = query[22].toInt();
                   ////////////////////////////////////////////////////방2
                   real_server->Room2.r_window1 = query[23].toInt();
                   real_server->Room2.r_door = query[24].toInt();

                   if(query[25].toInt() < 2)
                   {
                       real_server->Room2.r_LED_OnOff_state = 0;
                       real_server->Room2.r_LED_mode = query[25].toInt();
                   }
                   else
                   {
                       real_server->Room2.r_LED_OnOff_state = 2;
                       real_server->Room2.r_LED_mode = query[25].toInt() - 2;
                   }

                   real_server->Room2.r_LED_R = query[26].toInt();
                   real_server->Room2.r_LED_G = query[27].toInt();
                   real_server->Room2.r_LED_B = query[28].toInt();
                   real_server->Room2.r_LED_total = query[29].toInt();
                   ////////////////////////////////////////////////////화장실
                   real_server->Bath_room.r_ventilating_fan = query[30].toInt();
                   real_server->Bath_room.r_door = query[31].toInt();

                   if(query[32].toInt() < 2)
                   {
                       real_server->Bath_room.r_LED_OnOff_state = 0;
                       real_server->Bath_room.r_LED_mode = query[32].toInt();
                   }
                   else
                   {
                       real_server->Bath_room.r_LED_OnOff_state = 2;
                       real_server->Bath_room.r_LED_mode = query[32].toInt() - 2;
                   }

                   real_server->Bath_room.r_LED_R = query[33].toInt();
                   real_server->Bath_room.r_LED_G = query[34].toInt();
                   real_server->Bath_room.r_LED_B = query[35].toInt();
                   real_server->Bath_room.r_LED_total = query[36].toInt();
                   ////////////////////////////////////////////////////다목적실
                   real_server->Multipurpose_room.r_window1 = query[37].toInt();
                   real_server->Multipurpose_room.r_window2 = query[38].toInt();
                   real_server->Multipurpose_room.r_door = query[39].toInt();

                   if(query[40].toInt() < 2)
                   {
                       real_server->Multipurpose_room.r_LED_OnOff_state = 0;
                       real_server->Multipurpose_room.r_LED_mode = query[40].toInt();
                   }
                   else
                   {
                       real_server->Multipurpose_room.r_LED_OnOff_state = 2;
                       real_server->Multipurpose_room.r_LED_mode = query[40].toInt() - 2;
                   }

                   real_server->Multipurpose_room.r_LED_R = query[41].toInt();
                   real_server->Multipurpose_room.r_LED_G = query[42].toInt();
                   real_server->Multipurpose_room.r_LED_B = query[43].toInt();
                   real_server->Multipurpose_room.r_LED_total= query[44].toInt();
                   ////////////////////////////////////////////////////현관
                   real_server->Entrance.r_body_sensor = query[45].toInt();
                   real_server->Entrance.r_door = query[46].toInt();

                   if(query[47].toInt() < 2)
                   {
                       real_server->Entrance.r_LED_OnOff_state = 0;
                       real_server->Entrance.r_LED_mode = query[47].toInt();
                   }
                   else
                   {
                       real_server->Entrance.r_LED_OnOff_state = 2;
                       real_server->Entrance.r_LED_mode = query[47].toInt() - 2;
                   }

                   real_server->Entrance.r_LED_total= query[48].toInt();
                   ////////////////////////////////////////////////////외부
                   real_server->ETC.r_LED_R = query[49].toInt(); // S->B
                   real_server->ETC.r_LED_G = query[50].toInt(); // S->D
                   real_server->ETC.r_LED_B = query[51].toInt(); // B->D

                   real_server->ETC.r_LED_OnOff_state = query[52].toInt(); // home mode
                   real_server->ETC.r_LED_mode = query[53].toInt(); // heat pump
                   real_server->ETC.r_window1 = query[54].toInt(); // boiler
                   real_server->ETC.r_window2 = query[55].toInt(); // alldoors
                   real_server->ETC.r_door = query[56].toInt();  // allwindows

                   real_server->ETC.r_solar_pannel_angle = query[57].toInt();
                   real_server->ETC.r_garage_door = query[58].toInt();

                   emit update_value_E();
               }
               //real_server->get_mutex()->unlock();
                break;
            }
            default:qDebug() << "default!";
                break;
        }
    }
}



void CountWorker::disconnected()
{
    chatServer *real_server =(chatServer *)server;

    for(int i= 0 ;i<5;i++)
    {
        if(real_server->client_list[i] == this)
        {
            real_server->client_list[i] = NULL;
            qDebug() <<  "my Thread" << QThread::currentThreadId() << ": im dead..";
        }
    }
}


QTcpSocket* CountWorker::get_client_sock()
{
    return this->client;
}


