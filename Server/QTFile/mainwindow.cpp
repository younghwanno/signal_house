#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QString>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGraphicsScene>
#include <QDial>
#include <QtGui>
#include <QScrollBar>
#include <QDateTime>
#include "client_thread.h"
#include <QSlider>
#include <QCryptographicHash>
#include <QByteArray>

static int count = 0;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    server = new chatServer();
    bool success = server->listen(QHostAddress::Any, 8082);

    if(!success)
    {
        qFatal("Could not listen on port 8082.");
    }

    else
    {
        init_living_room();
        init_kitchen();
        init_room1();
        init_room2();
        init_bath_room();
        init_multipurpose_room();
        init_entrance();
        init_etc();

        init_air_view();
        init_graph_thread();

        init_home_database();
        init_timer();
        init_camera();

        init_open_cv_thread();


        qDebug() <<  "Main thread" << QThread::currentThreadId();
        qDebug() << "Ready";

        //////C,D 클라이언트 -> 서버 정보 제공(센서값 읽기)
        connect(server, SIGNAL(update_value_C_client()), this, SLOT(update_value_C()));
        connect(server, SIGNAL(update_value_D_client()), this, SLOT(update_value_D()));
        connect(server, SIGNAL(update_value_E_client()), this, SLOT(update_value_E()));


        //////A,B 클라이언트 <- 서버 명령 전달(엑추에이터 제어)
        connect(this, SIGNAL(send_value_A()), server, SLOT(send_value_A_client()));
        connect(this, SIGNAL(send_value_B()), server, SLOT(send_value_B_client()));
        connect(this, SIGNAL(send_value_E()), server, SLOT(send_value_E_client()));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::update_value_C()
{

    int temp = server->Living_room.get_Flmae() || server->Kitchen.get_Flmae() ||
                                server->Room1.get_Flmae() || server->Room2.get_Flmae() || (server->Kitchen.get_LPG() > 8000 ? 1 : 0);

    int temp2 = server->Entrance.get_knock();


    if(server->Living_room.get_Co2() > 2000) // 실내 공기가 좋지 않으면
    {
        if(server->client_list[3] != NULL && (server->ETC.get_rain1() == 0 && server->ETC.get_rain2()))
        {
            server->set_B_message(QString("%1,%2,%3,\n").arg("E").arg(1).arg(1)); // 거실 창문 열기
            emit send_value_B();
        }
    }
    else // 실내 공기가좋으면
    {
        server->set_B_message(QString("%1,%2,%3,\n").arg("E").arg(0).arg(0)); // 거실 창문 닫
        emit send_value_B();
    }



    //qDebug() << temp;
    //qDebug() << temp2;
    int temp3 = server->Room1.get_Flmae();
    int temp4 = (server->Kitchen.get_LPG() > 8000 ? 1 : 0);

    //qDebug() << temp4;



    if(temp == 1 || temp2 == 1)
    {
        server->set_A_message(QString("%1,%2,%3,%4,%5,%6,\n").arg("J").arg(temp).arg(temp2).arg(1).arg(temp3).arg(temp4));
        emit send_value_A();

        qDebug() << " ";

        server->set_B_message(QString("%1,\n").arg("R"));
        emit send_value_B();


        this->pre_num = 1;
    }
    else if(server->Room1.get_Flmae() != this->pre_num)
    {
        server->set_A_message(QString("%1,%2,%3,%4,%5,%6,\n").arg("J").arg(temp).arg(temp2).arg(0).arg(temp3).arg(temp4));
        emit send_value_A();

        pre_num = 0;
    }



   //chatServer *server = (chatServer *)sender();
    ///////////////////////////////////////////////////////////////////////////////////////////
    ui->label_33->setText("온도 : " + QString::number(server->Living_room.get_temperature()));
    ui->label_34->setText("습도 : " + QString::number(server->Living_room.get_humidity()));
    ui->label_35->setText("CO2 : " + QString::number(server->Living_room.get_Co2()));
    ui->label_36->setText("Smoke : " + QString::number(server->Living_room.get_Smoke()));


    if(server->Living_room.get_Flmae() == 1)
        ui->label_94->setText("Outbreak of fire!");
    else
        ui->label_94->setText("Steady-Stable");


    ///////////////////////////////////////////////////////////////////////////////////////////
    ui->label_41->setText("온도 : " + QString::number(server->Kitchen.get_temperature()));
    ui->label_42->setText("습도 : " + QString::number(server->Kitchen.get_humidity()));
    //ui->label_43->setText("CO2 : " + QString::number(server->Kitchen.get_Co2()));
   // ui->label_44->setText("Smoke : " + QString::number(server->Kitchen.get_Smoke()));
    ui->label_98->setText("LPG : " + QString::number(server->Kitchen.get_LPG()));


    if(server->Kitchen.get_Flmae() == 1)
        ui->label_95->setText("Outbreak of fire!");
    else
        ui->label_95->setText("Steady-Stable");

    ///////////////////////////////////////////////////////////////////////////////////////////
    ui->label_49->setText("온도 : " + QString::number(server->Room1.get_temperature()));
    ui->label_50->setText("습도 : " + QString::number(server->Room1.get_humidity()));
   // ui->label_51->setText("CO2 : " + QString::number(server->Room1.get_Co2()));
   // ui->label_52->setText("Smoke : " + QString::number(server->Room1.get_Smoke()));



    ///////////////////////////////////////////////////////////////////////////////////////////
    ui->label_57->setText("온도 : " + QString::number(server->Room2.get_temperature()));
    ui->label_58->setText("습도 : " + QString::number(server->Room2.get_humidity()));
   // ui->label_59->setText("CO2 : " + QString::number(server->Room2.get_Co2()));
  //  ui->label_60->setText("Smoke : " + QString::number(server->Room2.get_Smoke()));

    if(server->Room2.get_Flmae() == 1)
        ui->label_97->setText("Outbreak of fire!");
    else
        ui->label_97->setText("Steady-Stable");

    ///////////////////////////////////////////////////////////////////////////////////////////
    ui->label_65->setText("온도 : " + QString::number(server->Bath_room.get_temperature()));
    ui->label_66->setText("습도 : " + QString::number(server->Bath_room.get_humidity()));

    if(server->Room1.get_Flmae() == 1)
        ui->label_96->setText("Outbreak of fire!");
    else
        ui->label_96->setText("Steady-Stable");


    ///////////////////////////////////////////////////////////////////////////////////////////
    ui->label_86->setText("온도 : " + QString::number(server->Multipurpose_room.get_temperature()));
    ui->label_87->setText("습도 : " + QString::number(server->Multipurpose_room.get_humidity()));

}


void MainWindow::update_value_D()
{

    //////패널 상하1
    if(server->ETC.light[0] < server->ETC.light[1] -100)
    {
        if(server->ETC.get_solar_pannel_angle1() < 180)
        {
        server->ETC.set_solar_pannel_angle1(server->ETC.get_solar_pannel_angle1() + 5);
        server->set_B_message(QString("%1,%2,\n")
                                             .arg("Q")
                                             .arg(server->ETC.get_solar_pannel_angle1()));

        qDebug() << "a";
        emit send_value_B();
        }
    }
    else if(server->ETC.light[0] -100 > server->ETC.light[1])
    {
        if(server->ETC.get_solar_pannel_angle1() >= 5)
        {
        server->ETC.set_solar_pannel_angle1(server->ETC.get_solar_pannel_angle1() - 5);
        server->set_B_message(QString("%1,%2,\n")
                                             .arg("Q")
                                             .arg(server->ETC.get_solar_pannel_angle1()));

        qDebug() << "b";
        emit send_value_B();
        }
    }


    //////패널 좌우1
    if(server->ETC.light[2] < server->ETC.light[3] -100)
    {
        if(server->ETC.get_solar_pannel_angle2() < 180)
        {
        server->ETC.set_solar_pannel_angle2(server->ETC.get_solar_pannel_angle2() + 5);
        server->set_B_message(QString("%1,%2,\n")
                                             .arg("Z")
                                             .arg(server->ETC.get_solar_pannel_angle2()));

        qDebug() << "c";
        emit send_value_B();
        }
    }
    else if(server->ETC.light[2] - 100 > server->ETC.light[3])
    {
        if(server->ETC.get_solar_pannel_angle2() >= 5)
        {
        server->ETC.set_solar_pannel_angle2(server->ETC.get_solar_pannel_angle2() - 5);
        server->set_B_message(QString("%1,%2,\n")
                                             .arg("Z")
                                             .arg(server->ETC.get_solar_pannel_angle2()));

        qDebug() << "d";
        emit send_value_B();
        }
    }


    //////패널 상하2
    if(server->ETC.light[4] < server->ETC.light[5] -100)
    {
        if(server->ETC.get_solar_pannel_angle3() < 180)
        {
        server->ETC.set_solar_pannel_angle3(server->ETC.get_solar_pannel_angle3() + 5);
        server->set_B_message(QString("%1,%2,\n")
                                             .arg("V")
                                             .arg(server->ETC.get_solar_pannel_angle3()));

        qDebug() << "e";
        emit send_value_B();
        }
    }
    else if(server->ETC.light[4] -100 > server->ETC.light[5])
    {
        if(server->ETC.get_solar_pannel_angle3() >= 5)
        {
        server->ETC.set_solar_pannel_angle3(server->ETC.get_solar_pannel_angle3() - 5);
        server->set_B_message(QString("%1,%2,\n")
                                             .arg("V")
                                             .arg(server->ETC.get_solar_pannel_angle3()));

        qDebug() << "f";
        emit send_value_B();
        }
    }


    //////패널 좌우
    if(server->ETC.light[6] < server->ETC.light[7] -100)
    {
        if(server->ETC.get_solar_pannel_angle4() < 180)
        {
        server->ETC.set_solar_pannel_angle4(server->ETC.get_solar_pannel_angle4() + 5);
        server->set_B_message(QString("%1,%2,\n")
                                             .arg("S")
                                             .arg(server->ETC.get_solar_pannel_angle4()));

        qDebug() << "g";
        emit send_value_B();
        }
    }
    else if(server->ETC.light[6] -100 > server->ETC.light[7])
    {
        if(server->ETC.get_solar_pannel_angle4() >= 5)
        {
        server->ETC.set_solar_pannel_angle4(server->ETC.get_solar_pannel_angle4() - 5);
        server->set_B_message(QString("%1,%2,\n")
                                             .arg("S")
                                             .arg(server->ETC.get_solar_pannel_angle4()));

        qDebug() << "h";
        emit send_value_B();
        }
    }

    ui->label_78->setText("온도 : " + QString::number(server->ETC.get_temperature()));
    ui->label_80->setText("습도 : " + QString::number(server->ETC.get_humidity()));


     ui->label_120->setText("Power : " + QString::number(server->ETC.get_current()));
    this->graph_worker->get_graph()->set_current(server->ETC.get_current() / 10);
    emit graph_worker->notice_graph_data();



  //  server->ETC.get_current()

    if(server->ETC.get_rain1() <= 600 && server->ETC.get_rain2() <= 600)
    {
        ui->label_99->setText("weather : rainy");

        if(server->ETC.get_home_mode() == 0 && ui->pushButton_113->text() == "Windows(All) Open")
        {
             emit ui->pushButton_113->click();
        }
    }
    else
    {
        ui->label_99->setText("weather : normal");
    }


    if(server->ETC.get_dust() > 0)
    {
        ui->label_101->setText("dust : " + QString::number(server->ETC.get_dust()));

        if(server->ETC.get_dust() > 5000 && ui->pushButton_113->text() == "Windows(All) Open")
        {
             emit ui->pushButton_113->click();
        }
    }

        ui->label_100->setText("UV : " + QString::number(server->ETC.get_UV()));
        //ui->label_111->setText("battery : " + QString::number(server->ETC.get_voltage()));



        //인체감지 전등이 켜져있고 인체감지 되면 불 켜기
        if((server->ETC.get_human1() || server->ETC.get_human2()) && server->Entrance.get_body_sensor())
        {
            if(server->Entrance.get_LED_OnOff_state() == 0)
            {
                ui->pushButton_94->clicked();

                server->set_B_message(QString("%1,%2,%3,\n")
                                                     .arg("X")
                                                     .arg(server->Entrance.get_LED_mode() + server->Entrance.get_LED_OnOff_state())
                                                     .arg(server->Entrance.get_LED_total()));

                body_sensor_time = 0;
                server->Entrance.set_body_sensor_flag(1);

                emit send_value_B();
            }
        }



        if(server->ETC.get_load_cell() >= 3 || server->ETC.get_load_cell() <= -3)
        {
            qDebug() << "무게 인식";
            //////////////////////무게감지 되고, rfid 까지 일치하면 영상처리 시도

            QString qrc =  QString("select count(*) from vehicle_registration_table where vehicle_rfid ='%1'")
            .arg(this->server->ETC.get_rfid());


            database->get_query()->exec(qrc);

            bool ret = false;

            while(database->get_query()->next())
            {
                int count = database->get_query()->value(0).toInt();

                if(count >= 1)
                    ret = true;
            }


            if(opencv_worker->get_open_cv_widget()->get_camera() != NULL && ret)
            {
                qDebug() << "영상처리 시도";

                if(server->ETC.get_park_sensor_flag() == 0)
                    emit ui->pushButton_130->click();
            }
        }




}



void MainWindow::update_value_E()
{
    if(server->get_E_message() == "success\n" || server->get_E_message() == "fail\n")
       emit send_value_E();

///////////////////////////////////////////////////////////////////////////////////거실
    if(server->Living_room.r_window1 != server->Living_room.get_window1()) emit ui->pushButton_41->click();
    if(server->Living_room.r_window2 != server->Living_room.get_window2()) emit ui->pushButton_42->click();
    if(server->Living_room.r_LED_OnOff_state != server->Living_room.get_LED_OnOff_state()) emit ui->pushButton_44->click();

    if(server->Living_room.r_LED_mode != server->Living_room.get_LED_mode())
    {
        if(server->Living_room.r_LED_mode == 0) emit ui->radioButton_11->click();
        else emit ui->radioButton_12->click();
    }

    if(server->Living_room.r_LED_R != server->Living_room.get_LED_R())
    {
        ui->dial_17->setValue(server->Living_room.r_LED_R);
        this->changeValue_living_room_LED();
    }

    if(server->Living_room.r_LED_G != server->Living_room.get_LED_G())
    {
        ui->dial_18->setValue(server->Living_room.r_LED_G);
        this->changeValue_living_room_LED();
    }

    if(server->Living_room.r_LED_B != server->Living_room.get_LED_B())
    {
        ui->dial_19->setValue(server->Living_room.r_LED_B);
        this->changeValue_living_room_LED();
    }

    if(server->Living_room.r_LED_total != server->Living_room.get_LED_total())
    {
        ui->dial_20->setValue(server->Living_room.r_LED_total);
        this->changeValue_living_room_LED();
    }

///////////////////////////////////////////////////////////////////////////////////부엌
    if(server->Kitchen.r_window1 != server->Kitchen.get_window1()) emit ui->pushButton_51->click();
    if(server->Kitchen.r_window2 != server->Kitchen.get_window2()) emit ui->pushButton_52->click();
    if(server->Kitchen.r_LED_OnOff_state != server->Kitchen.get_LED_OnOff_state()) emit ui->pushButton_54->click();
    if(server->Kitchen.r_ventilating_fan != server->Kitchen.get_ventilating_fan()) emit ui->pushButton_55->click();

    if(server->Kitchen.r_LED_mode != server->Kitchen.get_LED_mode())
    {
        if(server->Kitchen.r_LED_mode == 0) emit ui->radioButton_13->click();
        else emit ui->radioButton_14->click();
    }

    if(server->Kitchen.r_LED_R != server->Kitchen.get_LED_R())
    {
        ui->dial_21->setValue(server->Kitchen.r_LED_R);
        this->changeValue_kitchen_LED();
    }

    if(server->Kitchen.r_LED_G != server->Kitchen.get_LED_G())
    {
        ui->dial_22->setValue(server->Kitchen.r_LED_G);
        this->changeValue_kitchen_LED();
    }

    if(server->Kitchen.r_LED_B != server->Kitchen.get_LED_B())
    {
        ui->dial_23->setValue(server->Kitchen.r_LED_B);
        this->changeValue_kitchen_LED();
    }

    if(server->Kitchen.r_LED_total != server->Kitchen.get_LED_total())
    {
        ui->dial_24->setValue(server->Kitchen.r_LED_total);
        this->changeValue_kitchen_LED();
    }

///////////////////////////////////////////////////////////////////////////////////방1
    if(server->Room1.r_window1 != server->Room1.get_window1()) emit ui->pushButton_61->click();
    if(server->Room1.r_door != server->Room1.get_door()) emit ui->pushButton_63->click();
    if(server->Room1.r_LED_OnOff_state != server->Room1.get_LED_OnOff_state()) emit ui->pushButton_64->click();

    if(server->Room1.r_LED_mode != server->Room1.get_LED_mode())
    {
        if(server->Room1.r_LED_mode == 0) emit ui->radioButton_15->click();
        else emit ui->radioButton_16->click();
    }

    if(server->Room1.r_LED_R != server->Room1.get_LED_R())
    {
        ui->dial_25->setValue(server->Room1.r_LED_R);
        this->changeValue_room1_LED();
    }

    if(server->Room1.r_LED_G != server->Room1.get_LED_G())
    {
        ui->dial_26->setValue(server->Room1.r_LED_G);
        this->changeValue_room1_LED();
    }

    if(server->Room1.r_LED_B != server->Room1.get_LED_B())
    {
        ui->dial_27->setValue(server->Room1.r_LED_B);
        this->changeValue_room1_LED();
    }

    if(server->Room1.r_LED_total != server->Room1.get_LED_total())
    {
        ui->dial_28->setValue(server->Room1.r_LED_total);
        this->changeValue_room1_LED();
    }
///////////////////////////////////////////////////////////////////////////////////방2
    if(server->Room2.r_window1 != server->Room2.get_window1()) emit ui->pushButton_71->click();
    if(server->Room2.r_door != server->Room2.get_door()) emit ui->pushButton_73->click();
    if(server->Room2.r_LED_OnOff_state != server->Room2.get_LED_OnOff_state()) emit ui->pushButton_74->click();

    if(server->Room2.r_LED_mode != server->Room2.get_LED_mode())
    {
        if(server->Room2.r_LED_mode == 0) emit ui->radioButton_17->click();
        else emit ui->radioButton_18->click();
    }

    if(server->Room2.r_LED_R != server->Room2.get_LED_R())
    {
        ui->dial_29->setValue(server->Room2.r_LED_R);
        this->changeValue_room2_LED();
    }

    if(server->Room2.r_LED_G != server->Room2.get_LED_G())
    {
        ui->dial_30->setValue(server->Room2.r_LED_G);
        this->changeValue_room2_LED();
    }

    if(server->Room2.r_LED_B != server->Room2.get_LED_B())
    {
        ui->dial_31->setValue(server->Room2.r_LED_B);
        this->changeValue_room2_LED();
    }

    if(server->Room2.r_LED_total != server->Room2.get_LED_total())
    {
        ui->dial_32->setValue(server->Room2.r_LED_total);
        this->changeValue_room2_LED();
    }
///////////////////////////////////////////////////////////////////////////////////화장실

    if(server->Bath_room.r_ventilating_fan != server->Bath_room.get_ventilating_fan()) emit ui->pushButton_85->click();
    if(server->Bath_room.r_door != server->Bath_room.get_door()) emit ui->pushButton_83->click();
    if(server->Bath_room.r_LED_OnOff_state != server->Bath_room.get_LED_OnOff_state()) emit ui->pushButton_84->click();

    if(server->Bath_room.r_LED_mode != server->Bath_room.get_LED_mode())
    {
        if(server->Bath_room.r_LED_mode == 0) emit ui->radioButton_19->click();
        else emit ui->radioButton_20->click();
    }

    if(server->Bath_room.r_LED_R != server->Bath_room.get_LED_R())
    {
        ui->dial_33->setValue(server->Bath_room.r_LED_R);
        this->changeValue_bath_room_LED();
    }

    if(server->Bath_room.r_LED_G != server->Bath_room.get_LED_G())
    {
        ui->dial_34->setValue(server->Bath_room.r_LED_G);
        this->changeValue_bath_room_LED();
    }

    if(server->Bath_room.r_LED_B != server->Bath_room.get_LED_B())
    {
        ui->dial_35->setValue(server->Bath_room.r_LED_B);
        this->changeValue_bath_room_LED();
    }

    if(server->Bath_room.r_LED_total != server->Bath_room.get_LED_total())
    {
        ui->dial_36->setValue(server->Bath_room.r_LED_total);
        this->changeValue_bath_room_LED();
    }
    ///////////////////////////////////////////////////////////////////////////////////다목적실

        if(server->Multipurpose_room.r_window1 != server->Multipurpose_room.get_window1()) emit ui->pushButton_101->click();
        if(server->Multipurpose_room.r_window2 != server->Multipurpose_room.get_window2()) emit ui->pushButton_102->click();
        if(server->Multipurpose_room.r_door != server->Multipurpose_room.get_door()) emit ui->pushButton_103->click();

        if(server->Multipurpose_room.r_LED_OnOff_state != server->Multipurpose_room.get_LED_OnOff_state()) emit ui->pushButton_104->click();

        if(server->Multipurpose_room.r_LED_mode != server->Multipurpose_room.get_LED_mode())
        {
            if(server->Multipurpose_room.r_LED_mode == 0) emit ui->radioButton_23->click();
            else emit ui->radioButton_24->click();
        }

        if(server->Multipurpose_room.r_LED_R != server->Multipurpose_room.get_LED_R())
        {
            ui->dial_41->setValue(server->Multipurpose_room.r_LED_R);
            this->changeValue_multipurpose_room_LED();
        }

        if(server->Multipurpose_room.r_LED_G != server->Multipurpose_room.get_LED_G())
        {
            ui->dial_42->setValue(server->Multipurpose_room.r_LED_G);
            this->changeValue_multipurpose_room_LED();
        }

        if(server->Multipurpose_room.r_LED_B != server->Multipurpose_room.get_LED_B())
        {
            ui->dial_43->setValue(server->Multipurpose_room.r_LED_B);
            this->changeValue_multipurpose_room_LED();
        }

        if(server->Multipurpose_room.r_LED_total != server->Multipurpose_room.get_LED_total())
        {
            ui->dial_44->setValue(server->Multipurpose_room.r_LED_total);
            this->changeValue_multipurpose_room_LED();
        }

        ///////////////////////////////////////////////////////////////////////////////////현관

            if(server->Entrance.r_body_sensor != server->Entrance.get_body_sensor()) emit ui->pushButton_91->click();
            if(server->Entrance.r_door != server->Entrance.get_door()) emit ui->pushButton_93->click();

            if(server->Entrance.r_LED_OnOff_state != server->Entrance.get_LED_OnOff_state()) emit ui->pushButton_94->click();

            if(server->Entrance.r_LED_total != server->Entrance.get_LED_total())
            {
                ui->dial_40->setValue(server->Entrance.r_LED_total);
                this->changeValue_entrance_LED();
            }

         ///////////////////////////////////////////////////////////////////////////////////외부
            //if(server->ETC.r_LED_R != server->ETC.get_Solar_to_Battery()) emit ui->pushButton_133->click(); // s->B
            //if(server->ETC.r_LED_G != server->ETC.get_Solar_to_DC()) emit ui->pushButton_132->click();  // s->D
            //if(server->ETC.r_LED_B != server->ETC.get_Batter_to_DC()) emit ui->pushButton_131->click();  // b->D

            if(server->ETC.r_LED_OnOff_state != server->ETC.get_home_mode())
            {
                if(server->ETC.r_LED_OnOff_state == 0) emit ui->radioButton_25->click();
                else emit ui->radioButton_26->click();
            }

            if(server->ETC.r_LED_mode != server->ETC.get_pump()) emit ui->pushButton_111->click(); // heat pump
            if(server->ETC.r_window1 != server->ETC.get_boiler()) emit ui->pushButton_112->click(); // boiler
            if(server->ETC.r_window2 != server->ETC.get_doors()) emit ui->pushButton_114->click(); // alldoors
            if(server->ETC.r_door != server->ETC.get_windows()) emit ui->pushButton_113->click(); // allwindows


            if(server->ETC.r_solar_pannel_angle != server->ETC.get_solar_pannel_angle1())
            {
                ui->dial_45->setValue(server->ETC.r_solar_pannel_angle);
                this->changeValue_solar_pannel_angle();
            }

            if(server->ETC.r_garage_door != server->ETC.get_garage_door()) emit ui->pushButton_125->click(); // allwindows
}



void MainWindow::init_living_room()
{

    ui->radioButton_11->setChecked(true);

    ui->dial_17->setNotchesVisible(true);
    ui->dial_17->setWrapping(false);
    ui->dial_17->setMinimum(0);
    ui->dial_17->setMaximum(255);

    ui->dial_18->setNotchesVisible(true);
    ui->dial_18->setWrapping(false);
    ui->dial_18->setMinimum(0);
    ui->dial_18->setMaximum(255);

    ui->dial_19->setNotchesVisible(true);
    ui->dial_19->setWrapping(false);
    ui->dial_19->setMinimum(0);
    ui->dial_19->setMaximum(255);

    ui->dial_20->setNotchesVisible(true);
    ui->dial_20->setWrapping(false);
    ui->dial_20->setMinimum(0);
    ui->dial_20->setMaximum(255);

    ui->label_37->setText("Red : 0");
    ui->label_38->setText("Green : 0");
    ui->label_39->setText("Blue : 0");
    ui->label_40->setText("Total : 0");


    ////////////////////////////////////////////////////////////////////////////////////////////////
    connect(ui->dial_17, SIGNAL(sliderReleased()), this, SLOT(changeValue_living_room_LED()));
    connect(ui->dial_18, SIGNAL(sliderReleased()), this, SLOT(changeValue_living_room_LED()));
    connect(ui->dial_19, SIGNAL(sliderReleased()), this, SLOT(changeValue_living_room_LED()));
    connect(ui->dial_20, SIGNAL(sliderReleased()), this, SLOT(changeValue_living_room_LED()));

    connect(ui->radioButton_11, SIGNAL(clicked()), this, SLOT(changeValue_living_room_radio()));
    connect(ui->radioButton_12, SIGNAL(clicked()), this, SLOT(changeValue_living_room_radio()));

    connect(ui->pushButton_41, SIGNAL(clicked()), this, SLOT(changeState_living_room_Window1()));
    connect(ui->pushButton_42, SIGNAL(clicked()), this, SLOT(changeState_living_room_Window2()));
    connect(ui->pushButton_44, SIGNAL(clicked()), this, SLOT(changeState_living_room_LED_onoff()));

    connect(ui->pushButton_115, SIGNAL(clicked()), this, SLOT(show_living_room_table()));

    emit ui->radioButton_11->clicked();
    /////////////////////////////////////////////////////////////////////////////////////////////////
}



void MainWindow::init_kitchen()
{
    ui->radioButton_13->setChecked(true);

    ui->dial_21->setNotchesVisible(true);
    ui->dial_21->setWrapping(false);
    ui->dial_21->setMinimum(0);
    ui->dial_21->setMaximum(255);

    ui->dial_22->setNotchesVisible(true);
    ui->dial_22->setWrapping(false);
    ui->dial_22->setMinimum(0);
    ui->dial_22->setMaximum(255);

    ui->dial_23->setNotchesVisible(true);
    ui->dial_23->setWrapping(false);
    ui->dial_23->setMinimum(0);
    ui->dial_23->setMaximum(255);

    ui->dial_24->setNotchesVisible(true);
    ui->dial_24->setWrapping(false);
    ui->dial_24->setMinimum(0);
    ui->dial_24->setMaximum(255);

    ui->label_45->setText("Red : 0");
    ui->label_46->setText("Green : 0");
    ui->label_47->setText("Blue : 0");
    ui->label_48->setText("Total : 0");


    ////////////////////////////////////////////////////////////////////////////////////////////////
    connect(ui->dial_21, SIGNAL(sliderReleased()), this, SLOT(changeValue_kitchen_LED()));
    connect(ui->dial_22, SIGNAL(sliderReleased()), this, SLOT(changeValue_kitchen_LED()));
    connect(ui->dial_23, SIGNAL(sliderReleased()), this, SLOT(changeValue_kitchen_LED()));
    connect(ui->dial_24, SIGNAL(sliderReleased()), this, SLOT(changeValue_kitchen_LED()));

    connect(ui->radioButton_13, SIGNAL(clicked()), this, SLOT(changeValue_kitchen_radio()));
    connect(ui->radioButton_14, SIGNAL(clicked()), this, SLOT(changeValue_kitchen_radio()));

    connect(ui->pushButton_51, SIGNAL(clicked()), this, SLOT(changeState_kitchen_Window1()));
    connect(ui->pushButton_52, SIGNAL(clicked()), this, SLOT(changeState_kitchen_Window2()));
    connect(ui->pushButton_54, SIGNAL(clicked()), this, SLOT(changeState_kitchen_LED_onoff()));
    connect(ui->pushButton_55, SIGNAL(clicked()), this, SLOT(changeState_kitchen_ventilating_fan()));

    connect(ui->pushButton_116, SIGNAL(clicked()), this, SLOT(show_kitchen_table()));

    emit ui->radioButton_13->clicked();
    /////////////////////////////////////////////////////////////////////////////////////////////////
}




void MainWindow::init_room1()
{
    ui->radioButton_15->setChecked(true);

    ui->dial_25->setNotchesVisible(true);
    ui->dial_25->setWrapping(false);
    ui->dial_25->setMinimum(0);
    ui->dial_25->setMaximum(255);

    ui->dial_26->setNotchesVisible(true);
    ui->dial_26->setWrapping(false);
    ui->dial_26->setMinimum(0);
    ui->dial_26->setMaximum(255);

    ui->dial_27->setNotchesVisible(true);
    ui->dial_27->setWrapping(false);
    ui->dial_27->setMinimum(0);
    ui->dial_27->setMaximum(255);

    ui->dial_28->setNotchesVisible(true);
    ui->dial_28->setWrapping(false);
    ui->dial_28->setMinimum(0);
    ui->dial_28->setMaximum(255);

    ui->label_53->setText("Red : 0");
    ui->label_54->setText("Green : 0");
    ui->label_55->setText("Blue : 0");
    ui->label_56->setText("Total : 0");


    ////////////////////////////////////////////////////////////////////////////////////////////////
    connect(ui->dial_25, SIGNAL(sliderReleased()), this, SLOT(changeValue_room1_LED()));
    connect(ui->dial_26, SIGNAL(sliderReleased()), this, SLOT(changeValue_room1_LED()));
    connect(ui->dial_27, SIGNAL(sliderReleased()), this, SLOT(changeValue_room1_LED()));
    connect(ui->dial_28, SIGNAL(sliderReleased()), this, SLOT(changeValue_room1_LED()));

    connect(ui->radioButton_15, SIGNAL(clicked()), this, SLOT(changeValue_room1_radio()));
    connect(ui->radioButton_16, SIGNAL(clicked()), this, SLOT(changeValue_room1_radio()));

    connect(ui->pushButton_61, SIGNAL(clicked()), this, SLOT(changeState_room1_Window1()));
    connect(ui->pushButton_63, SIGNAL(clicked()), this, SLOT(changeState_room1_Door()));
    connect(ui->pushButton_64, SIGNAL(clicked()), this, SLOT(changeState_room1_LED_onoff()));

    connect(ui->pushButton_117, SIGNAL(clicked()), this, SLOT(show_room1_table()));

    emit ui->radioButton_15->clicked();
    /////////////////////////////////////////////////////////////////////////////////////////////////
}



void MainWindow::init_room2()
{
    ui->radioButton_17->setChecked(true);

    ui->dial_29->setNotchesVisible(true);
    ui->dial_29->setWrapping(false);
    ui->dial_29->setMinimum(0);
    ui->dial_29->setMaximum(255);

    ui->dial_30->setNotchesVisible(true);
    ui->dial_30->setWrapping(false);
    ui->dial_30->setMinimum(0);
    ui->dial_30->setMaximum(255);

    ui->dial_31->setNotchesVisible(true);
    ui->dial_31->setWrapping(false);
    ui->dial_31->setMinimum(0);
    ui->dial_31->setMaximum(255);

    ui->dial_32->setNotchesVisible(true);
    ui->dial_32->setWrapping(false);
    ui->dial_32->setMinimum(0);
    ui->dial_32->setMaximum(255);

    ui->label_61->setText("Red : 0");
    ui->label_62->setText("Green : 0");
    ui->label_63->setText("Blue : 0");
    ui->label_64->setText("Total : 0");


    ////////////////////////////////////////////////////////////////////////////////////////////////
    connect(ui->dial_29, SIGNAL(sliderReleased()), this, SLOT(changeValue_room2_LED()));
    connect(ui->dial_30, SIGNAL(sliderReleased()), this, SLOT(changeValue_room2_LED()));
    connect(ui->dial_31, SIGNAL(sliderReleased()), this, SLOT(changeValue_room2_LED()));
    connect(ui->dial_32, SIGNAL(sliderReleased()), this, SLOT(changeValue_room2_LED()));

    connect(ui->radioButton_17, SIGNAL(clicked()), this, SLOT(changeValue_room2_radio()));
    connect(ui->radioButton_18, SIGNAL(clicked()), this, SLOT(changeValue_room2_radio()));

    connect(ui->pushButton_71, SIGNAL(clicked()), this, SLOT(changeState_room2_Window1()));
    connect(ui->pushButton_73, SIGNAL(clicked()), this, SLOT(changeState_room2_Door()));
    connect(ui->pushButton_74, SIGNAL(clicked()), this, SLOT(changeState_room2_LED_onoff()));

    connect(ui->pushButton_118, SIGNAL(clicked()), this, SLOT(show_room2_table()));

    emit ui->radioButton_17->clicked();
    /////////////////////////////////////////////////////////////////////////////////////////////////
}


void MainWindow::init_bath_room()
{
    ui->radioButton_19->setChecked(true);

    ui->dial_33->setNotchesVisible(true);
    ui->dial_33->setWrapping(false);
    ui->dial_33->setMinimum(0);
    ui->dial_33->setMaximum(255);

    ui->dial_34->setNotchesVisible(true);
    ui->dial_34->setWrapping(false);
    ui->dial_34->setMinimum(0);
    ui->dial_34->setMaximum(255);

    ui->dial_35->setNotchesVisible(true);
    ui->dial_35->setWrapping(false);
    ui->dial_35->setMinimum(0);
    ui->dial_35->setMaximum(255);

    ui->dial_36->setNotchesVisible(true);
    ui->dial_36->setWrapping(false);
    ui->dial_36->setMinimum(0);
    ui->dial_36->setMaximum(255);

    ui->label_69->setText("Red : 0");
    ui->label_70->setText("Green : 0");
    ui->label_71->setText("Blue : 0");
    ui->label_72->setText("Total : 0");


    ////////////////////////////////////////////////////////////////////////////////////////////////
    connect(ui->dial_33, SIGNAL(sliderReleased()), this, SLOT(changeValue_bath_room_LED()));
    connect(ui->dial_34, SIGNAL(sliderReleased()), this, SLOT(changeValue_bath_room_LED()));
    connect(ui->dial_35, SIGNAL(sliderReleased()), this, SLOT(changeValue_bath_room_LED()));
    connect(ui->dial_36, SIGNAL(sliderReleased()), this, SLOT(changeValue_bath_room_LED()));

    connect(ui->radioButton_19, SIGNAL(clicked()), this, SLOT(changeValue_bath_room_radio()));
    connect(ui->radioButton_20, SIGNAL(clicked()), this, SLOT(changeValue_bath_room_radio()));

    connect(ui->pushButton_83, SIGNAL(clicked()), this, SLOT(changeState_bath_room_Door()));
    connect(ui->pushButton_84, SIGNAL(clicked()), this, SLOT(changeState_bath_room_LED_onoff()));
    connect(ui->pushButton_85, SIGNAL(clicked()), this, SLOT(changeState_bath_room_ventilating_fan()));

    connect(ui->pushButton_119, SIGNAL(clicked()), this, SLOT(show_bath_room_table()));

    emit ui->radioButton_19->clicked();
}


void MainWindow::init_multipurpose_room()
{
    ui->radioButton_23->setChecked(true);

    ui->dial_41->setNotchesVisible(true);
    ui->dial_41->setWrapping(false);
    ui->dial_41->setMinimum(0);
    ui->dial_41->setMaximum(255);

    ui->dial_42->setNotchesVisible(true);
    ui->dial_42->setWrapping(false);
    ui->dial_42->setMinimum(0);
    ui->dial_42->setMaximum(255);

    ui->dial_43->setNotchesVisible(true);
    ui->dial_43->setWrapping(false);
    ui->dial_43->setMinimum(0);
    ui->dial_43->setMaximum(255);

    ui->dial_44->setNotchesVisible(true);
    ui->dial_44->setWrapping(false);
    ui->dial_44->setMinimum(0);
    ui->dial_44->setMaximum(255);

    ui->label_90->setText("Red : 0");
    ui->label_91->setText("Green : 0");
    ui->label_92->setText("Blue : 0");
    ui->label_93->setText("Total : 0");


    ////////////////////////////////////////////////////////////////////////////////////////////////
    connect(ui->dial_41, SIGNAL(sliderReleased()), this, SLOT(changeValue_multipurpose_room_LED()));
    connect(ui->dial_42, SIGNAL(sliderReleased()), this, SLOT(changeValue_multipurpose_room_LED()));
    connect(ui->dial_43, SIGNAL(sliderReleased()), this, SLOT(changeValue_multipurpose_room_LED()));
    connect(ui->dial_44, SIGNAL(sliderReleased()), this, SLOT(changeValue_multipurpose_room_LED()));

    connect(ui->radioButton_23, SIGNAL(clicked()), this, SLOT(changeValue_multipurpose_room_radio()));
    connect(ui->radioButton_24, SIGNAL(clicked()), this, SLOT(changeValue_multipurpose_room_radio()));

    connect(ui->pushButton_101, SIGNAL(clicked()), this, SLOT(changeState_multipurpose_room_Window1()));
    connect(ui->pushButton_102, SIGNAL(clicked()), this, SLOT(changeState_multipurpose_room_Window2()));
    connect(ui->pushButton_103, SIGNAL(clicked()), this, SLOT(changeState_multipurpose_room_Door()));
    connect(ui->pushButton_104, SIGNAL(clicked()), this, SLOT(changeState_multipurpose_room_LED_onoff()));

    connect(ui->pushButton_120, SIGNAL(clicked()), this, SLOT(show_multi_room_table()));

    emit ui->radioButton_23->clicked();
}


void MainWindow::init_entrance()
{
    ui->radioButton_21->setChecked(true);
    ui->dial_40->setNotchesVisible(true);
    ui->dial_40->setWrapping(false);
    ui->dial_40->setMinimum(0);
    ui->dial_40->setMaximum(255);
    ui->label_85->setText("Total : 0");


    connect(ui->dial_40, SIGNAL(sliderReleased()), this, SLOT(changeValue_entrance_LED()));
    connect(ui->pushButton_91, SIGNAL(clicked()), this, SLOT(body_sensor_onoff()));
    connect(ui->pushButton_93, SIGNAL(clicked()), this, SLOT(changeState_entrance_Door()));
    connect(ui->pushButton_94, SIGNAL(clicked()), this, SLOT(changeState_entrance_LED_onoff()));
    connect(ui->pushButton_121, SIGNAL(clicked()), this, SLOT(show_entrance_table()));

    emit ui->radioButton_21->clicked();
}



void MainWindow::init_etc()
{
    ui->radioButton_25->setChecked(true);

    ui->label_99->setText("weather : normal");
    ui->label_100->setText("UV : ");
    ui->label_101->setText("dust : ");

    connect(ui->radioButton_25, SIGNAL(clicked()), this, SLOT(changeValue_home_mode()));
    connect(ui->radioButton_26, SIGNAL(clicked()), this, SLOT(changeValue_home_mode()));


    connect(ui->pushButton_111, SIGNAL(clicked()), this, SLOT(changeState_Pump()));  //난방(순환)
    connect(ui->pushButton_112, SIGNAL(clicked()), this, SLOT(changeState_Boiler()));  //보일러(발열 필름)
    connect(ui->pushButton_113, SIGNAL(clicked()), this, SLOT(changeState_Windows()));  // 전체 Window
    connect(ui->pushButton_114, SIGNAL(clicked()), this, SLOT(changeState_Doors()));   //전체 Door

    connect(ui->pushButton_122, SIGNAL(clicked()), this, SLOT(show_etc_table()));

    connect(ui->pushButton_123, SIGNAL(clicked()), this, SLOT(show_vehicle_registration_table()));
    connect(ui->pushButton_124, SIGNAL(clicked()), this, SLOT(show_login_registration_table()));


    //connect(ui->pushButton_133, SIGNAL(clicked()), this, SLOT(Solar_to_Battery()));
    //connect(ui->pushButton_132, SIGNAL(clicked()), this, SLOT(Solar_to_DC()));
    //connect(ui->pushButton_131, SIGNAL(clicked()), this, SLOT(Battery_to_DC()));


    connect(ui->pushButton_125, SIGNAL(clicked()), this, SLOT(changeState_garage_Door()));


    ui->dial_45->setNotchesVisible(true);
    ui->dial_45->setWrapping(false);
    ui->dial_45->setMinimum(0);
    ui->dial_45->setMaximum(180);

    connect(ui->dial_45, SIGNAL(sliderReleased()), this, SLOT(changeValue_solar_pannel_angle()));

    emit ui->radioButton_25->clicked();
}




//////////////////////////////////////////////////////////////////living_room 제어 슬롯
void MainWindow::changeValue_living_room_LED()
{
    ui->label_37->setText(QString("Red : %1").arg(ui->dial_17->value()));
    ui->label_38->setText(QString("Green : %1").arg(ui->dial_18->value()));
    ui->label_39->setText(QString("Blue : %1").arg(ui->dial_19->value()));
    ui->label_40->setText(QString("Total : %1").arg(ui->dial_20->value()));

    server->Living_room.set_LED_R(ui->dial_17->value());
    server->Living_room.set_LED_G(ui->dial_18->value());
    server->Living_room.set_LED_B(ui->dial_19->value());
    server->Living_room.set_LED_total(ui->dial_20->value());


    server->set_B_message(QString("%1,%2,%3,%4,%5,%6,\n")
                                         .arg("A")
                                         .arg(server->Living_room.get_LED_mode() + server->Living_room.get_LED_OnOff_state())
                                         .arg(server->Living_room.get_LED_R())
                                         .arg(server->Living_room.get_LED_G())
                                         .arg(server->Living_room.get_LED_B())
                                         .arg(server->Living_room.get_LED_total()));

    emit send_value_B();
}


void MainWindow::changeValue_living_room_radio()
{

    if(ui->radioButton_11->isChecked())
    {
        ui->dial_17->setEnabled(false);
        ui->dial_18->setEnabled(false);
        ui->dial_19->setEnabled(false);
        ui->dial_20->setEnabled(true);

       server->Living_room.set_LED_mode(0);
    }
    else if(ui->radioButton_12->isChecked())
    {
        ui->dial_17->setEnabled(true);
        ui->dial_18->setEnabled(true);
        ui->dial_19->setEnabled(true);
        ui->dial_20->setEnabled(false);

        server->Living_room.set_LED_mode(1);
    }

    emit ui->dial_17->sliderReleased();
}


void MainWindow::changeState_living_room_LED_onoff()
{
   // graph_worker->get_graph()->set_current(qrand() % 60);
   // emit graph_worker->notice_graph_data();

    if(server->Living_room.get_LED_OnOff_state() == 0)
    {
        server->Living_room.set_LED_OnOff_state(2);
        ui->pushButton_44->setText("LED ON");
    }
    else
    {
        server->Living_room.set_LED_OnOff_state(0);
        ui->pushButton_44->setText("LED OFF");
    }

    emit ui->dial_17->sliderReleased();
}



void MainWindow::changeState_living_room_Window1()
{

    if(server->Living_room.get_window1() == 0)
    {
        server->Living_room.set_window1(1);
        ui->pushButton_41->setText("Window1 Open");
    }
    else
    {
        server->Living_room.set_window1(0);
        ui->pushButton_41->setText("Window1 Close");
    }

    server->set_B_message(QString("%1,%2,%3,\n")
                                         .arg("E")
                                         .arg(server->Living_room.get_window1())
                                         .arg(server->Living_room.get_window2()));

    emit send_value_B();
}


void MainWindow::changeState_living_room_Window2()
{

    if(server->Living_room.get_window2() == 0)
    {
        server->Living_room.set_window2(1);
        ui->pushButton_42->setText("Window2 Open");
    }
    else
    {
        server->Living_room.set_window2(0);
        ui->pushButton_42->setText("Window2 Close");
    }

    server->set_B_message(QString("%1,%2,%3,\n")
                                         .arg("E")
                                         .arg(server->Living_room.get_window1())
                                         .arg(server->Living_room.get_window2()));

    emit send_value_B();
}


//////////////////////////////////////////////////////////////////kitchen 제어 슬롯
void MainWindow::changeValue_kitchen_LED()
{
    ui->label_45->setText(QString("Red : %1").arg(ui->dial_21->value()));
    ui->label_46->setText(QString("Green : %1").arg(ui->dial_22->value()));
    ui->label_47->setText(QString("Blue : %1").arg(ui->dial_23->value()));
    ui->label_48->setText(QString("Total : %1").arg(ui->dial_24->value()));

    server->Kitchen.set_LED_R(ui->dial_21->value());
    server->Kitchen.set_LED_G(ui->dial_22->value());
    server->Kitchen.set_LED_B(ui->dial_23->value());
    server->Kitchen.set_LED_total(ui->dial_24->value());


    server->set_A_message(QString("%1,%2,%3,%4,%5,%6,\n")
                                         .arg("C")
                                         .arg(server->Kitchen.get_LED_mode() + server->Kitchen.get_LED_OnOff_state())
                                         .arg(server->Kitchen.get_LED_R())
                                         .arg(server->Kitchen.get_LED_G())
                                         .arg(server->Kitchen.get_LED_B())
                                         .arg(server->Kitchen.get_LED_total()));

    emit send_value_A();
}


void MainWindow::changeValue_kitchen_radio()
{
    if(ui->radioButton_13->isChecked())
    {
        ui->dial_21->setEnabled(false);
        ui->dial_22->setEnabled(false);
        ui->dial_23->setEnabled(false);
        ui->dial_24->setEnabled(true);

       server->Kitchen.set_LED_mode(0);
    }
    else if(ui->radioButton_14->isChecked())
    {
        ui->dial_21->setEnabled(true);
        ui->dial_22->setEnabled(true);
        ui->dial_23->setEnabled(true);
        ui->dial_24->setEnabled(false);

        server->Kitchen.set_LED_mode(1);
    }

    emit ui->dial_21->sliderReleased();
}


void MainWindow::changeState_kitchen_LED_onoff()
{
    if(server->Kitchen.get_LED_OnOff_state() == 0)
    {
        server->Kitchen.set_LED_OnOff_state(2);
        ui->pushButton_54->setText("LED ON");
    }
    else
    {
        server->Kitchen.set_LED_OnOff_state(0);
        ui->pushButton_54->setText("LED OFF");
    }

    emit ui->dial_21->sliderReleased();
}



void MainWindow::changeState_kitchen_Window1()
{

    if(server->Kitchen.get_window1() == 0)
    {
        server->Kitchen.set_window1(1);
        ui->pushButton_51->setText("Window1 Open");
    }
    else
    {
        server->Kitchen.set_window1(0);
        ui->pushButton_51->setText("Window1 Close");
    }

    server->set_A_message(QString("%1,%2,%3,\n")
                                         .arg("G")
                                         .arg(server->Kitchen.get_window1())
                                         .arg(server->Kitchen.get_window2()));

    emit send_value_A();
}



void MainWindow::changeState_kitchen_Window2()
{

    if(server->Kitchen.get_window2() == 0)
    {
        server->Kitchen.set_window2(1);
        ui->pushButton_52->setText("Window2 Open");
    }
    else
    {
        server->Kitchen.set_window2(0);
        ui->pushButton_52->setText("Window2 Close");
    }

    server->set_A_message(QString("%1,%2,%3,\n")
                                         .arg("G")
                                         .arg(server->Kitchen.get_window1())
                                         .arg(server->Kitchen.get_window2()));

    emit send_value_A();
}


void MainWindow::changeState_kitchen_ventilating_fan()
{
    int temp;

    if(server->Kitchen.get_ventilating_fan() == 0)
    {
        server->Kitchen.set_ventilating_fan(1);
        ui->pushButton_55->setText("Ventilating_fan ON");
        temp = 1;
    }
    else
    {
        server->Kitchen.set_ventilating_fan(0);
        ui->pushButton_55->setText("Ventilating_fan OFF");
        temp = 0;
    }

    server->set_A_message(QString("%1,%2,\n")
                                         .arg("O")
                                         .arg(temp));

    emit send_value_A();

}

//////////////////////////////////////////////////////////////////room1 제어 슬롯
void MainWindow::changeValue_room1_LED()
{
    ui->label_53->setText(QString("Red : %1").arg(ui->dial_25->value()));
    ui->label_54->setText(QString("Green : %1").arg(ui->dial_26->value()));
    ui->label_55->setText(QString("Blue : %1").arg(ui->dial_27->value()));
    ui->label_56->setText(QString("Total : %1").arg(ui->dial_28->value()));

    server->Room1.set_LED_R(ui->dial_25->value());
    server->Room1.set_LED_G(ui->dial_26->value());
    server->Room1.set_LED_B(ui->dial_27->value());
    server->Room1.set_LED_total(ui->dial_28->value());


    server->set_A_message(QString("%1,%2,%3,%4,%5,%6,\n")
                                         .arg("B")
                                         .arg(server->Room1.get_LED_mode() + server->Room1.get_LED_OnOff_state())
                                         .arg(server->Room1.get_LED_R())
                                         .arg(server->Room1.get_LED_G())
                                         .arg(server->Room1.get_LED_B())
                                         .arg(server->Room1.get_LED_total()));

    emit send_value_A();
}


void MainWindow::changeValue_room1_radio()
{
    if(ui->radioButton_15->isChecked())
    {
        ui->dial_25->setEnabled(false);
        ui->dial_26->setEnabled(false);
        ui->dial_27->setEnabled(false);
        ui->dial_28->setEnabled(true);

       server->Room1.set_LED_mode(0);
    }
    else if(ui->radioButton_16->isChecked())
    {
        ui->dial_25->setEnabled(true);
        ui->dial_26->setEnabled(true);
        ui->dial_27->setEnabled(true);
        ui->dial_28->setEnabled(false);

        server->Room1.set_LED_mode(1);
    }

    emit ui->dial_25->sliderReleased();
}


void MainWindow::changeState_room1_LED_onoff()
{
    if(server->Room1.get_LED_OnOff_state() == 0)
    {
        server->Room1.set_LED_OnOff_state(2);
        ui->pushButton_64->setText("LED ON");
    }
    else
    {
        server->Room1.set_LED_OnOff_state(0);
        ui->pushButton_64->setText("LED OFF");
    }

    emit ui->dial_25->sliderReleased();
}



void MainWindow::changeState_room1_Window1()
{

    if(server->Room1.get_window1() == 0)
    {
        server->Room1.set_window1(1);
        ui->pushButton_61->setText("Window Open");
    }
    else
    {
        server->Room1.set_window1(0);
        ui->pushButton_61->setText("Window Close");
    }

    server->set_A_message(QString("%1,%2,%3,\n")
                                         .arg("F")
                                         .arg(server->Room1.get_window1())
                                         .arg(server->Room1.get_door()));

    emit send_value_A();
}



void MainWindow::changeState_room1_Door()
{

    if(server->Room1.get_door() == 0)
    {
        server->Room1.set_door(1);
        ui->pushButton_63->setText("Door Open");
    }
    else
    {
        server->Room1.set_door(0);
        ui->pushButton_63->setText("Door Close");
    }

    server->set_A_message(QString("%1,%2,%3,\n")
                                         .arg("F")
                                         .arg(server->Room1.get_window1())
                                         .arg(server->Room1.get_door()));

    emit send_value_A();
}

//////////////////////////////////////////////////////////////////room2 제어 슬롯
void MainWindow::changeValue_room2_LED()
{
    ui->label_61->setText(QString("Red : %1").arg(ui->dial_29->value()));
    ui->label_62->setText(QString("Green : %1").arg(ui->dial_30->value()));
    ui->label_63->setText(QString("Blue : %1").arg(ui->dial_31->value()));
    ui->label_64->setText(QString("Total : %1").arg(ui->dial_32->value()));

    server->Room2.set_LED_R(ui->dial_29->value());
    server->Room2.set_LED_G(ui->dial_30->value());
    server->Room2.set_LED_B(ui->dial_31->value());
    server->Room2.set_LED_total(ui->dial_32->value());


    server->set_B_message(QString("%1,%2,%3,%4,%5,%6,\n")
                                         .arg("C")
                                         .arg(server->Room2.get_LED_mode() + server->Room2.get_LED_OnOff_state())
                                         .arg(server->Room2.get_LED_R())
                                         .arg(server->Room2.get_LED_G())
                                         .arg(server->Room2.get_LED_B())
                                         .arg(server->Room2.get_LED_total()));
    emit send_value_B();
}


void MainWindow::changeValue_room2_radio()
{
    if(ui->radioButton_17->isChecked())
    {
        ui->dial_29->setEnabled(false);
        ui->dial_30->setEnabled(false);
        ui->dial_31->setEnabled(false);
        ui->dial_32->setEnabled(true);

       server->Room2.set_LED_mode(0);
    }
    else if(ui->radioButton_18->isChecked())
    {
        ui->dial_29->setEnabled(true);
        ui->dial_30->setEnabled(true);
        ui->dial_31->setEnabled(true);
        ui->dial_32->setEnabled(false);

        server->Room2.set_LED_mode(1);
    }

    emit ui->dial_29->sliderReleased();
}


void MainWindow::changeState_room2_LED_onoff()
{
    if(server->Room2.get_LED_OnOff_state() == 0)
    {
        server->Room2.set_LED_OnOff_state(2);
        ui->pushButton_74->setText("LED ON");
    }
    else
    {
        server->Room2.set_LED_OnOff_state(0);
        ui->pushButton_74->setText("LED OFF");
    }

    emit ui->dial_29->sliderReleased();
}




void MainWindow::changeState_room2_Window1()
{
    if(server->Room2.get_window1() == 0)
    {
        server->Room2.set_window1(1);
        ui->pushButton_71->setText("Window Open");
    }
    else
    {
        server->Room2.set_window1(0);
        ui->pushButton_71->setText("Window Close");
    }

    server->set_B_message(QString("%1,%2,%3,\n")
                                         .arg("G")
                                         .arg(server->Room2.get_window1())
                                         .arg(server->Room2.get_door()));

    emit send_value_B();
}



void MainWindow::changeState_room2_Door()
{

    if(server->Room2.get_door() == 0)
    {
        server->Room2.set_door(1);
        ui->pushButton_73->setText("Door Open");
    }
    else
    {
        server->Room2.set_door(0);
        ui->pushButton_73->setText("Door Close");
    }

    server->set_B_message(QString("%1,%2,%3,\n")
                                         .arg("G")
                                         .arg(server->Room2.get_window1())
                                         .arg(server->Room2.get_door()));

    emit send_value_B();
}


//////////////////////////////////////////////////////////////////Bath_room 제어 슬롯
void MainWindow::changeValue_bath_room_LED()
{
    ui->label_69->setText(QString("Red : %1").arg(ui->dial_33->value()));
    ui->label_70->setText(QString("Green : %1").arg(ui->dial_34->value()));
    ui->label_71->setText(QString("Blue : %1").arg(ui->dial_35->value()));
    ui->label_72->setText(QString("Total : %1").arg(ui->dial_36->value()));

    server->Bath_room.set_LED_R(ui->dial_33->value());
    server->Bath_room.set_LED_G(ui->dial_34->value());
    server->Bath_room.set_LED_B(ui->dial_35->value());
    server->Bath_room.set_LED_total(ui->dial_36->value());


    server->set_B_message(QString("%1,%2,%3,%4,%5,%6,\n")
                                         .arg("B")
                                         .arg(server->Bath_room.get_LED_mode() + server->Bath_room.get_LED_OnOff_state())
                                         .arg(server->Bath_room.get_LED_R())
                                         .arg(server->Bath_room.get_LED_G())
                                         .arg(server->Bath_room.get_LED_B())
                                         .arg(server->Bath_room.get_LED_total()));
    emit send_value_B();
}


void MainWindow::changeValue_bath_room_radio()
{
    if(ui->radioButton_19->isChecked())
    {
        ui->dial_33->setEnabled(false);
        ui->dial_34->setEnabled(false);
        ui->dial_35->setEnabled(false);
        ui->dial_36->setEnabled(true);

       server->Bath_room.set_LED_mode(0);
    }
    else if(ui->radioButton_20->isChecked())
    {
        ui->dial_33->setEnabled(true);
        ui->dial_34->setEnabled(true);
        ui->dial_35->setEnabled(true);
        ui->dial_36->setEnabled(false);

        server->Bath_room.set_LED_mode(1);
    }

    emit ui->dial_33->sliderReleased();
}


void MainWindow::changeState_bath_room_LED_onoff()
{
    if(server->Bath_room.get_LED_OnOff_state() == 0)
    {
        server->Bath_room.set_LED_OnOff_state(2);
        ui->pushButton_84->setText("LED ON");
    }
    else
    {
        server->Bath_room.set_LED_OnOff_state(0);
        ui->pushButton_84->setText("LED OFF");
    }

    emit ui->dial_33->sliderReleased();
}



void  MainWindow::changeState_bath_room_Door()
{
    if(server->Bath_room.get_door() == 0)
    {
        server->Bath_room.set_door(1);
        ui->pushButton_83->setText("Door Open");
    }
    else
    {
        server->Bath_room.set_door(0);
        ui->pushButton_83->setText("Door Close");
    }

    server->set_B_message(QString("%1,%2,\n")
                                         .arg("F")
                                         .arg(server->Bath_room.get_door()));

    emit send_value_B();
}





void MainWindow::changeState_bath_room_ventilating_fan()
{
    int temp;

    if(server->Bath_room.get_ventilating_fan() == 0)
    {
        server->Bath_room.set_ventilating_fan(1);
        ui->pushButton_85->setText("Ventilating_fan ON");
        temp = 1;
    }
    else
    {
        server->Bath_room.set_ventilating_fan(0);
        ui->pushButton_85->setText("Ventilating_fan OFF");
        temp = 0;
    }

    server->set_B_message(QString("%1,%2,\n")
                                         .arg("O")
                                         .arg(temp));

    emit send_value_B();

}


//////////////////////////////////////////////////////////////////multipurpose_room 제어 슬롯
void MainWindow::changeValue_multipurpose_room_LED()
{
    ui->label_90->setText(QString("Red : %1").arg(ui->dial_41->value()));
    ui->label_91->setText(QString("Green : %1").arg(ui->dial_42->value()));
    ui->label_92->setText(QString("Blue : %1").arg(ui->dial_43->value()));
    ui->label_93->setText(QString("Total : %1").arg(ui->dial_44->value()));

    server->Multipurpose_room.set_LED_R(ui->dial_41->value());
    server->Multipurpose_room.set_LED_G(ui->dial_42->value());
    server->Multipurpose_room.set_LED_B(ui->dial_43->value());
    server->Multipurpose_room.set_LED_total(ui->dial_44->value());


    server->set_A_message(QString("%1,%2,%3,%4,%5,%6,\n")
                                         .arg("A")
                                         .arg(server->Multipurpose_room.get_LED_mode() + server->Multipurpose_room.get_LED_OnOff_state())
                                         .arg(server->Multipurpose_room.get_LED_R())
                                         .arg(server->Multipurpose_room.get_LED_G())
                                         .arg(server->Multipurpose_room.get_LED_B())
                                         .arg(server->Multipurpose_room.get_LED_total()));

    emit send_value_A();
}


void MainWindow::changeValue_multipurpose_room_radio()
{
    if(ui->radioButton_23->isChecked())
    {
        ui->dial_41->setEnabled(false);
        ui->dial_42->setEnabled(false);
        ui->dial_43->setEnabled(false);
        ui->dial_44->setEnabled(true);

       server->Multipurpose_room.set_LED_mode(0);
    }
    else if(ui->radioButton_24->isChecked())
    {
        ui->dial_41->setEnabled(true);
        ui->dial_42->setEnabled(true);
        ui->dial_43->setEnabled(true);
        ui->dial_44->setEnabled(false);

        server->Multipurpose_room.set_LED_mode(1);
    }

    emit ui->dial_41->sliderReleased();
}


void MainWindow::changeState_multipurpose_room_LED_onoff()
{
    if(server->Multipurpose_room.get_LED_OnOff_state() == 0)
    {
        server->Multipurpose_room.set_LED_OnOff_state(2);
        ui->pushButton_104->setText("LED ON");
    }
    else
    {
        server->Multipurpose_room.set_LED_OnOff_state(0);
        ui->pushButton_104->setText("LED OFF");
    }

    emit ui->dial_41->sliderReleased();
}



void MainWindow::changeState_multipurpose_room_Window1()
{

    if(server->Multipurpose_room.get_window1() == 0)
    {
        server->Multipurpose_room.set_window1(1);
        ui->pushButton_101->setText("Window1 Open");
    }
    else
    {
        server->Multipurpose_room.set_window1(0);
        ui->pushButton_101->setText("Window1 Close");
    }

    server->set_A_message(QString("%1,%2,%3,%4,\n")
                                         .arg("E")
                                         .arg(server->Multipurpose_room.get_window1())
                                         .arg(server->Multipurpose_room.get_window2())
                                         .arg(server->Multipurpose_room.get_door()));

    emit send_value_A();
}


void MainWindow::changeState_multipurpose_room_Window2()
{

    if(server->Multipurpose_room.get_window2() == 0)
    {
        server->Multipurpose_room.set_window2(1);
        ui->pushButton_102->setText("Window2 Open");
    }
    else
    {
        server->Multipurpose_room.set_window2(0);
        ui->pushButton_102->setText("Window2 Close");
    }

    server->set_A_message(QString("%1,%2,%3,%4,\n")
                                         .arg("E")
                                         .arg(server->Multipurpose_room.get_window1())
                                         .arg(server->Multipurpose_room.get_window2())
                                         .arg(server->Multipurpose_room.get_door()));

    emit send_value_A();
}



void MainWindow::changeState_multipurpose_room_Door()
{

    if(server->Multipurpose_room.get_door() == 0)
    {
        server->Multipurpose_room.set_door(1);
        ui->pushButton_103->setText("Door Open");
    }
    else
    {
        server->Multipurpose_room.set_door(0);
        ui->pushButton_103->setText("Door Close");
    }

    server->set_A_message(QString("%1,%2,%3,%4,\n")
                                         .arg("E")
                                         .arg(server->Multipurpose_room.get_window1())
                                         .arg(server->Multipurpose_room.get_window2())
                                         .arg(server->Multipurpose_room.get_door()));

    emit send_value_A();
}



//////////////////////////////////////////////////////////////////entrance 제어 슬롯
void MainWindow::changeValue_entrance_LED()
{
    ui->label_85->setText(QString("Total : %1").arg(ui->dial_40->value()));
    server->Entrance.set_LED_total(ui->dial_40->value());

    server->set_B_message(QString("%1,%2,%3,\n")
                                         .arg("X")
                                         .arg(server->Entrance.get_LED_mode() + server->Entrance.get_LED_OnOff_state())
                                         .arg(server->Entrance.get_LED_total()));

    emit send_value_B();
}


void MainWindow::changeState_entrance_LED_onoff()
{
    if(server->Entrance.get_LED_OnOff_state() == 0)
    {
        server->Entrance.set_LED_OnOff_state(2);
        ui->pushButton_94->setText("LED ON");
    }
    else
    {
        server->Entrance.set_LED_OnOff_state(0);
        ui->pushButton_94->setText("LED OFF");
    }

    emit ui->dial_40->sliderReleased();
}



void MainWindow::changeState_entrance_Door()
{
    if(server->Entrance.get_door() == 0)
    {
        server->Entrance.set_door(1);
        ui->pushButton_93->setText("Door Open");
    }
    else
    {
        server->Entrance.set_door(0);
        ui->pushButton_93->setText("Door Close");
    }

    server->set_B_message(QString("%1,%2,\n")
                                         .arg("H")
                                         .arg(server->Entrance.get_door()));

    emit send_value_B();
}



void MainWindow::body_sensor_onoff()
{
    if(server->Entrance.get_body_sensor() == 0)
    {
        server->Entrance.set_body_sensor(1);
        ui->pushButton_91->setText("Body_Sensor ON");
    }
    else
    {
        server->Entrance.set_body_sensor(0);
        ui->pushButton_91->setText("Body_Sensor OFF");
    }
}


//////////////////////////////////////////////////////////////////etc 제어슬롯
void MainWindow::changeState_Pump()
{
    int temp;

    if(server->ETC.get_pump() == 0)
    {
        server->ETC.set_pump(1);
        ui->pushButton_111->setText("Heating (pump)On");
        temp = 1;
    }
    else
    {
        server->ETC.set_pump(0);
        ui->pushButton_111->setText("Heating (pump)OFF");
        temp = 0;
    }

    server->set_A_message(QString("%1,%2,\n").arg("K").arg(temp));
    emit send_value_A();
}


void MainWindow::changeState_Boiler()
{
    int temp;

    if(server->ETC.get_boiler() == 0)
    {
        server->ETC.set_boiler(1);
        ui->pushButton_112->setText("Boiler On");
        temp = 1;
    }
    else
    {
        server->ETC.set_boiler(0);
        ui->pushButton_112->setText("Boiler OFF");
        temp = 0;
    }

    server->set_A_message(QString("%1,%2,\n").arg("L").arg(temp));
    emit send_value_A();
}



void MainWindow::changeState_Windows()
{
    int temp;

    if(server->ETC.get_windows() == 0)
    {
        server->ETC.set_windows(1);
        ui->pushButton_113->setText("Windows(All) Open");
        temp = 1;
    }
    else
    {
        server->ETC.set_windows(0);
        ui->pushButton_113->setText("Windows(All) Close");
        temp = 0;
    }


    if(server->Living_room.get_window1() != temp) {server->Living_room.set_window1(temp); temp == 0 ? ui->pushButton_41->setText("Window1 Close"): ui->pushButton_41->setText("Window1 Open");}
    if(server->Living_room.get_window2() != temp) {server->Living_room.set_window2(temp); temp == 0 ? ui->pushButton_42->setText("Window2 Close"): ui->pushButton_42->setText("Window2 Open");}

    if(server->Kitchen.get_window1() != temp) {server->Kitchen.set_window1(temp); temp == 0 ? ui->pushButton_51->setText("Window1 Close"): ui->pushButton_51->setText("Window1 Open");}
    if(server->Kitchen.get_window2() != temp) {server->Kitchen.set_window2(temp); temp == 0 ? ui->pushButton_52->setText("Window2 Close"): ui->pushButton_52->setText("Window2 Open");}

    if(server->Room1.get_window1() != temp) {server->Room1.set_window1(temp); temp == 0 ? ui->pushButton_61->setText("Window Close"): ui->pushButton_61->setText("Window Open");}
    if(server->Room2.get_window1() != temp) {server->Room2.set_window1(temp); temp == 0 ? ui->pushButton_71->setText("Window Close"): ui->pushButton_71->setText("Window Open");}

    if(server->Multipurpose_room.get_window1() != temp) {server->Multipurpose_room.set_window1(temp); temp == 0 ? ui->pushButton_101->setText("Window1 Close"): ui->pushButton_101->setText("Window1 Open");}
    if(server->Multipurpose_room.get_window2() != temp) {server->Multipurpose_room.set_window2(temp); temp == 0 ? ui->pushButton_102->setText("Window2 Close"): ui->pushButton_102->setText("Window2 Open");}


    server->set_A_message(QString("%1,%2\n").arg("N").arg(temp));
    server->set_B_message(QString("%1,%2\n").arg("N").arg(temp));
    emit send_value_A();
    emit send_value_B();
}



void MainWindow::changeState_Doors()
{
    int temp;

    if(server->ETC.get_doors() == 0)
    {
        server->ETC.set_doors(1);
        ui->pushButton_114->setText("Doors(All) Open");
        temp = 1;
    }
    else
    {
        server->ETC.set_doors(0);
        ui->pushButton_114->setText("Doors(All) Close");
        temp = 0;
    }


    if(server->Room1.get_door() != temp) {server->Room1.set_door(temp); temp == 0 ? ui->pushButton_63->setText("Door Close"): ui->pushButton_63->setText("Door Open");}
    if(server->Room2.get_door() != temp) {server->Room2.set_door(temp); temp == 0 ? ui->pushButton_73->setText("Door Close"): ui->pushButton_73->setText("Door Open");}

    if(server->Bath_room.get_door() != temp) {server->Bath_room.set_door(temp); temp == 0 ? ui->pushButton_83->setText("Door Close"): ui->pushButton_83->setText("Door Open");}
    if(server->Multipurpose_room.get_door() != temp) {server->Multipurpose_room.set_door(temp); temp == 0 ? ui->pushButton_103->setText("Door Close"): ui->pushButton_103->setText("Door Open");}
    //if(server->Entrance.get_door() != temp) {server->Multipurpose_room.set_door(temp); temp == 0 ? ui->pushButton_103->setText("Door Close"): ui->pushButton_103->setText("Door Open");}

    server->set_A_message(QString("%1,%2\n").arg("M").arg(temp));
    server->set_B_message(QString("%1,%2\n").arg("M").arg(temp));
    emit send_value_A();
    emit send_value_B();
}



void MainWindow::changeValue_home_mode()
{
    if(ui->radioButton_25->isChecked())
    {
       server->ETC.set_home_mode(0);
    }
    else
    {
       server->ETC.set_home_mode(1);
    }
}


void MainWindow::init_air_view()
{
    QString living_room_Image = "/home/csy/test/images/living_room_Image.png";
    QString kitchen_Image = "/home/csy/test/images/kitchen_Image.png";
    QString room1_Image = "/home/csy/test/images/room1_Image.png";
    QString room2_Image = "/home/csy/test/images/room2_Image.png";
    QString bath_room_Image = "/home/csy/test/images/bath_room_Image.png";
    QString multipurpose_room_Image = "/home/csy/test/images/multipurpose_room_Image.png";
    QString entrance_Image = "/home/csy/test/images/entrance_Image.png";

    QString logo = "/home/csy/test/images/logo.PNG";

    ui->label_110->setPixmap(QPixmap(living_room_Image));
    ui->label_110->setScaledContents(true);

    ui->label_113->setPixmap(QPixmap(kitchen_Image));
    ui->label_113->setScaledContents(true);

    ui->label_115->setPixmap(QPixmap(room1_Image));
    ui->label_115->setScaledContents(true);

    ui->label_116->setPixmap(QPixmap(room2_Image));
    ui->label_116->setScaledContents(true);

    ui->label_117->setPixmap(QPixmap(bath_room_Image));
    ui->label_117->setScaledContents(true);

    ui->label_118->setPixmap(QPixmap(multipurpose_room_Image));
    ui->label_118->setScaledContents(true);

    ui->label_119->setPixmap(QPixmap(entrance_Image));
    ui->label_119->setScaledContents(true);

    ui->label_112->setPixmap(QPixmap(logo));
    ui->label_112->setScaledContents(true);

}


void MainWindow::init_graph_thread()
{
    graph_worker = new testing();
    ui->verticalLayout_7->addWidget(graph_worker->get_scrollarea());

    QThread * graph_thread = new QThread;
    graph_worker->moveToThread(graph_thread);

    graph_thread->start();
}


void MainWindow::init_open_cv_thread()
{

   opencv_worker = new open_cv_worker(this->database->get_query());
   ui->verticalLayout_3->addWidget(opencv_worker->get_open_cv_widget());

   if(opencv_worker->get_open_cv_widget()->get_camera() != NULL)
   {
       QThread * opencv_thread = new QThread;
       opencv_worker->moveToThread(opencv_thread);

       connect(ui->pushButton_128, SIGNAL(clicked()), this, SLOT(camera_capture2()));
       connect(ui->pushButton_129, SIGNAL(clicked()), this, SLOT(camera_on_off2()));
       connect(ui->pushButton_130, SIGNAL(clicked()), opencv_worker, SLOT(process_image_data()));
       connect(opencv_worker, SIGNAL(notice_open_door()), this, SLOT(notice_open_door()));

       opencv_thread->start();
   }
}


void MainWindow::init_home_database()
{
    database = new home_database();
    database->db_start();

    if(database->get_query() != NULL)
    {
        server->set_query(database->get_query());
    }
}


void MainWindow::init_timer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_count()));
    timer->start(1000);
}


void MainWindow::init_camera()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    camera1 = NULL;

    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        if (cameraInfo.deviceName() == "/dev/video0")
         {
            camera1 = new QCamera(cameraInfo, this);
            break;
        }
    }

        if(camera1 == NULL)
        {
            QString camera_invalid = "/home/csy/test/images/camera_invalid1.png";
            QLabel *temp = new QLabel();
            temp->setPixmap(QPixmap(camera_invalid).scaled(210,110));
            ui->verticalLayout_2->addWidget(temp);
        }
        else
        {
        image_capture1 = new QCameraImageCapture(this->camera1);
        video_record1 = new QMediaRecorder(this->camera1);

        QAudioEncoderSettings audioSettings;
        audioSettings.setQuality(QMultimedia::HighQuality);
        video_record1->setAudioSettings(audioSettings);

        QVideoEncoderSettings videoSettings = video_record1->videoSettings();
        //videoSettings.setCodec("video/mp4");
        videoSettings.setQuality(QMultimedia::HighQuality);
        videoSettings.setResolution(640,480);
        //videoSettings.setFrameRate(30.0);
        video_record1->setVideoSettings(videoSettings);
        camera_state_flag1 = 0;

        QVideoWidget *videoWidget = new QVideoWidget();
        videoWidget->setFixedSize(210,130);
        ui->verticalLayout_2->addWidget(videoWidget);
        camera1->setViewfinder(videoWidget);

        connect(ui->pushButton_127, SIGNAL(clicked()), this, SLOT(camera_capture1()));
        connect(ui->pushButton_126, SIGNAL(clicked()), this, SLOT(camera_on_off1()));
        }
}


void MainWindow::camera_capture1()
{
    this->camera1->setCaptureMode(QCamera::CaptureStillImage);
    this->camera1->start();
    this->camera1->searchAndLock();
    image_capture1->capture("/home/csy/test/images/still_shot1.jpg");
    this->camera1->unlock();

    qDebug() << "still shot";
}


void MainWindow::camera_on_off1()
{
    if(this->camera_state_flag1 == 0)
    {
        this->camera1->start();
        ui->pushButton_126->setText("ON");
        this->camera_state_flag1 = 1;
    }
    else
    {
        this->camera1->stop();
        ui->pushButton_126->setText("OFF");
        this->camera_state_flag1 = 0;
    }
}


void MainWindow::camera_capture2()
{
    QCamera* temp = opencv_worker->get_open_cv_widget()->get_camera();
    image_capture2 = new QCameraImageCapture(temp);

    temp->setCaptureMode(QCamera::CaptureStillImage);
    temp->start();
    temp->searchAndLock();
    image_capture2->capture("/home/csy/test/images/still_shot2.jpg");
    temp->unlock();

    qDebug() << "still shot";
}


void MainWindow::camera_on_off2()
{
    QCamera* temp = opencv_worker->get_open_cv_widget()->get_camera();

    if(this->camera_state_flag2 == 0)
    {
        temp->start();
        ui->pushButton_129->setText("ON");
        this->camera_state_flag2 = 2;
    }
    else
    {
        temp->stop();
        ui->pushButton_129->setText("OFF");
        this->camera_state_flag2 = 0;
    }
}


void MainWindow::camera2_image_processing()
{
    if(camera_state_flag2 == 0)
        opencv_worker->get_open_cv_widget()->get_camera()->start();

    camera_capture2();

    qDebug() << "이미지 가공..";
}



void MainWindow::show_living_room_table()
{
    if(database->get_query() != NULL)
    {
        database->get_living_room_table_object()->show_table()->show();
        emit database->get_living_room_table_object()->get_pushbutton()->click();
    }

    database->get_living_room_table_object()->show_table()->raise();
}


void MainWindow::show_kitchen_table()
{
    if(database->get_query() != NULL)
    {
        database->get_kitchen_table_object()->show_table()->show();
        emit database->get_kitchen_table_object()->get_pushbutton()->click();
    }

    database->get_kitchen_table_object()->show_table()->raise();
}

void MainWindow::show_room1_table()
{
    if(database->get_query() != NULL)
    {
        database->get_room1_table_object()->show_table()->show();
        emit database->get_room1_table_object()->get_pushbutton()->click();
    }

    database->get_room1_table_object()->show_table()->raise();
}

void MainWindow::show_room2_table()
{
    if(database->get_query() != NULL)
    {
        database->get_room2_table_object()->show_table()->show();
        emit database->get_room2_table_object()->get_pushbutton()->click();
    }

    database->get_room2_table_object()->show_table()->raise();
}

void MainWindow::show_bath_room_table()
{
    if(database->get_query() != NULL)
    {
        database->get_bath_room_table_object()->show_table()->show();
        emit database->get_bath_room_table_object()->get_pushbutton()->click();
    }

    database->get_bath_room_table_object()->show_table()->raise();
}

void MainWindow::show_multi_room_table()
{
    if(database->get_query() != NULL)
    {
        database->get_multipurpose_room_table_object()->show_table()->show();
        emit database->get_multipurpose_room_table_object()->get_pushbutton()->click();
    }

    database->get_multipurpose_room_table_object()->show_table()->raise();
}

void MainWindow::show_entrance_table()
{
    if(database->get_query() != NULL)
    {
        database->get_entrance_table_object()->show_table()->show();
        emit database->get_entrance_table_object()->get_pushbutton()->click();
    }

    database->get_entrance_table_object()->show_table()->raise();
}

void MainWindow::show_etc_table()
{
    if(database->get_query() != NULL)
    {
        database->get_etc_table_object()->show_table()->show();
        emit database->get_etc_table_object()->get_pushbutton()->click();
    }

    database->get_etc_table_object()->show_table()->raise();
}

void MainWindow::show_vehicle_registration_table()
{
    if(database->get_query() != NULL)
    {
        database->get_vehicle_registration_table_object()->show_table()->show();
        emit database->get_vehicle_registration_table_object()->get_pushbutton()->click();
    }

    database->get_vehicle_registration_table_object()->show_table()->raise();
}


void MainWindow::show_login_registration_table()
{
   if(database->get_query() != NULL)
   {
       database->get_login_registration_table_object()->show_table()->show();
       emit database->get_login_registration_table_object()->get_pushbutton()->click();
   }

   database->get_login_registration_table_object()->show_table()->raise();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    this->database->get_living_room_table_object()->show_table()->close();
    this->database->get_kitchen_table_object()->show_table()->close();
    this->database->get_room1_table_object()->show_table()->close();
    this->database->get_room2_table_object()->show_table()->close();
    this->database->get_bath_room_table_object()->show_table()->close();
    this->database->get_multipurpose_room_table_object()->show_table()->close();
    this->database->get_entrance_table_object()->show_table()->close();
    this->database->get_etc_table_object()->show_table()->close();
    this->database->get_vehicle_registration_table_object()->show_table()->close();
    this->database->get_login_registration_table_object()->show_table()->close();
}


void MainWindow::timer_count()
{
     count++;

     ////컨버터에서 넘어오는 전압 1이 안정적일때 릴레이 닫기
     bool vol_value1 = ((server->ETC.get_voltage() >= 680 && server->ETC.get_voltage() <= 710) ? true : false);
     ////배터리의 전압 2이가 완충이 덜 되었으면 릴레이 닫기
     bool vol_value2 = true;


     if(count % 60 == 0) // 배터리 전압 voltage2fmf 측정을 위해서 릴레이를 잠시 끔
     {
         if(server->client_list[1] != NULL)
         {
             server->set_B_message(QString("%1,%2,\n").arg("P").arg(0));
             emit send_value_B();
         }
     }
     else if(count % 63 == 0)
     {
         if(server->client_list[3] != NULL)  // 배터리 전압 측정
         {
             vol_value2 = ((server->ETC.get_voltage2() <= 680) ? true : false);

             if(server->ETC.get_voltage() < server->ETC.get_voltage2()) vol_value2 = false;

             //이 전압으로 배터리 용량 측정, 디스플레이에 배치
             //완충 전압 6.85
             //방전 종지전압 5.45
             server->ETC.set_battery2_capacity((1 - ((685 - server->ETC.get_voltage2()) / 1.4)) * 100.0);

             ui->label_114->setText(QString("battery : %1").arg(server->ETC.get_battery1_capacity()));
             //ui->label_111->setText(QString("battery2 : %1").arg(server->ETC.get_battery2_capacity()));
         }






         if(server->client_list[1] != NULL) // 전압 측정 종료, 릴레이 2개를 다시 켬
         {
             server->set_B_message(QString("%1,%2,\n").arg("P").arg(1));
             emit send_value_B();
         }
     }


     ///릴레이 상태를 바꿔야 할 경우 명령
     if(voltage_relay != (vol_value1 && vol_value2))
     {
         voltage_relay = (vol_value1 && vol_value2);

         server->set_B_message(QString("%1,%2,\n").arg("P").arg(voltage_relay));
         emit send_value_B();
     }




     if(server->client_list[3] != NULL)
     {
         //파워 그래프 수치 표시, 그리기
         int power = (server->ETC.get_voltage() * server->ETC.get_current());

         ui->label_120->setText(QString("Power : %1").arg(power));

         graph_worker->get_graph()->set_current(power);
         emit graph_worker->notice_graph_data();
     }



     /////////////인체감지
     if(server->Entrance.get_body_sensor_flag() == 1)
     {
         body_sensor_time++;

         if(body_sensor_time == 5)
         {
            if(server->Entrance.get_LED_OnOff_state() != 0 && server->Entrance.get_body_sensor_flag() == 1)
            {
                ui->pushButton_94->clicked();
                server->Entrance.set_body_sensor_flag(0);
                body_sensor_time = 0;
                qDebug() << "5초 지낫따1111111111111111111111";
            }
         }
     }

     /////////////차고지 문 닫기
     if(server->ETC.get_garage_door() == 1 && server->ETC.get_park_sensor_flag() == 1)
     {
         if((server->ETC.get_load_cell() >= -3 && server->ETC.get_load_cell() < 3))
            park_sensor_time++;
         else
             park_sensor_time = 0;


         if(park_sensor_time == 3)
         {
            if((server->ETC.get_load_cell() >= -3 && server->ETC.get_load_cell() < 3))
            {
                ui->pushButton_125->clicked();
                server->ETC.set_park_sensor_flag(0);
                park_sensor_time = 0;
                qDebug() << "3초 지낫따1111111111111111111111";
            }
         }
     }


     if(database->get_query() != NULL)// && count % 60 == 0 )
     {
        //qDebug() << "it'time";

        QDateTime now = QDateTime::currentDateTime();


        /*
        //if(server->client_list[2] != NULL)
        {
            this->server->Living_room.set_datetime(now);
            this->server->Kitchen.set_datetime(now);
            this->server->Room1.set_datetime(now);
            this->server->Room2.set_datetime(now);
            this->server->Bath_room.set_datetime(now);
            this->server->Multipurpose_room.set_datetime(now);
            this->server->Entrance.set_datetime(now);

            database->get_living_room_table_object()->insert_view(&this->server->Living_room);
            database->get_kitchen_table_object()->insert_view(&this->server->Kitchen);
            database->get_room1_table_object()->insert_view(&this->server->Room1);
            database->get_room2_table_object()->insert_view(&this->server->Room2);
            database->get_bath_room_table_object()->insert_view(&this->server->Bath_room);
            database->get_multipurpose_room_table_object()->insert_view(&this->server->Multipurpose_room);
            database->get_entrance_table_object()->insert_view(&this->server->Entrance);
        }

        */


  /*
        if(server->client_list[3] != NULL)
        {

            this->server->ETC.set_datetime(now);
            database->get_etc_table_object()->insert_view(&this->server->ETC);

        }
 */



        if(server->client_list[4] != NULL)
        {
            QString tp = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,"
                                 "%13,%14,%15,%16,%17,%18,%19,%20,%21,%22,%23,%24,"
                                 "%25,%26,%27,%28,%29,%30,%31,%32,%33,%34,"
                                 "%35,%36,%37,%38,%39,%40,%41,%42,%43,%44,"
                                 "%45,%46,%47,%48,%49,%50,%51,%52,%53,"
                                 "%54,%55,%56,%57,%58,%59,%60,%61,%62,%63,"
                                 "%64,%65,%66,%67,%68,%69,"
                                 "%70,%71,%72,%73,%74,%75,%76,%77,%78,%79,%80,%81,%82,%83,%84,%85"
                                 "\n")

                    .arg(server->Living_room.get_temperature()).arg(server->Living_room.get_humidity()).arg(server->Living_room.get_Co2()).arg(server->Living_room.get_Smoke())
                    .arg(server->Living_room.get_Flmae()).arg(server->Living_room.get_LED_mode() + server->Living_room.get_LED_OnOff_state()).arg(server->Living_room.get_LED_R())
                    .arg(server->Living_room.get_LED_G()).arg(server->Living_room.get_LED_B()).arg(server->Living_room.get_LED_total())
                    .arg(server->Living_room.get_window1()).arg(server->Living_room.get_window2())

                    .arg(server->Kitchen.get_temperature()).arg(server->Kitchen.get_humidity()).arg(server->Kitchen.get_LPG()).arg(server->Kitchen.get_Flmae())
                    .arg(server->Kitchen.get_LED_mode() + server->Kitchen.get_LED_OnOff_state()).arg(server->Kitchen.get_LED_R()).arg(server->Kitchen.get_LED_G())
                    .arg(server->Kitchen.get_LED_B()).arg(server->Kitchen.get_LED_total()).arg(server->Kitchen.get_window1()).arg(server->Kitchen.get_window2())
                    .arg(server->Kitchen.get_ventilating_fan())

                    .arg(server->Room1.get_temperature()).arg(server->Room1.get_humidity()).arg(server->Room1.get_Flmae())
                    .arg(server->Room1.get_LED_mode() + server->Room1.get_LED_OnOff_state()).arg(server->Room1.get_LED_R()).arg(server->Room1.get_LED_G())
                    .arg(server->Room1.get_LED_B()).arg(server->Room1.get_LED_total()).arg(server->Room1.get_window1()).arg(server->Room1.get_door())

                    .arg(server->Room2.get_temperature()).arg(server->Room2.get_humidity()).arg(server->Room2.get_Flmae())
                    .arg(server->Room2.get_LED_mode() + server->Room2.get_LED_OnOff_state()).arg(server->Room2.get_LED_R()).arg(server->Room2.get_LED_G())
                    .arg(server->Room2.get_LED_B()).arg(server->Room2.get_LED_total()).arg(server->Room2.get_window1()).arg(server->Room2.get_door())

                    .arg(server->Bath_room.get_temperature()).arg(server->Bath_room.get_humidity()).arg(server->Bath_room.get_LED_mode() + server->Bath_room.get_LED_OnOff_state())
                    .arg(server->Bath_room.get_LED_R()).arg(server->Bath_room.get_LED_G()).arg(server->Bath_room.get_LED_B()).arg(server->Bath_room.get_LED_total())
                    .arg(server->Bath_room.get_ventilating_fan()).arg(server->Bath_room.get_door())

                    .arg(server->Multipurpose_room.get_temperature()).arg(server->Multipurpose_room.get_humidity())
                    .arg(server->Multipurpose_room.get_LED_mode() + server->Multipurpose_room.get_LED_OnOff_state()).arg(server->Multipurpose_room.get_LED_R())
                    .arg(server->Multipurpose_room.get_LED_G()).arg(server->Multipurpose_room.get_LED_B()).arg(server->Multipurpose_room.get_LED_total())
                    .arg(server->Multipurpose_room.get_window1()).arg(server->Multipurpose_room.get_window2()).arg(server->Multipurpose_room.get_door())

                    .arg(server->Entrance.get_temperature()).arg(server->Entrance.get_humidity()).arg(server->Entrance.get_LED_mode() + server->Entrance.get_LED_OnOff_state())
                    .arg(server->Entrance.get_LED_total()).arg(server->Entrance.get_body_sensor()).arg(server->Entrance.get_door())

                    .arg(server->ETC.get_temperature()).arg(server->ETC.get_humidity()).arg(server->ETC.get_voltage()).arg(server->ETC.get_UV()).arg(server->ETC.get_dust())
                    .arg(server->ETC.get_Solar_to_Battery()).arg(server->ETC.get_Solar_to_DC()).arg(server->ETC.get_Batter_to_DC())
                    .arg((server->ETC.get_rain1() > 700 && server->ETC.get_rain2() == true) >= 1 ? 1 : 0).arg(server->ETC.get_home_mode())
                    .arg(server->ETC.get_pump()).arg(server->ETC.get_boiler()).arg(server->ETC.get_doors()).arg(server->ETC.get_windows())
                    .arg(server->ETC.get_solar_pannel_angle1()).arg(server->ETC.get_garage_door());


            server->set_E_message(tp);
            emit send_value_E();

        }


     }

}

void MainWindow::change_energy_image(int SB, int SD, int BD)
{
    int num = (SB << 2) + (SD << 1) + BD;

    QString energy_Image = QString("/home/csy/test/images/Plates/%1.JPG").arg(num);
    ui->label_112->setPixmap(QPixmap(energy_Image));
    ui->label_112->setScaledContents(true);
}



void MainWindow::notice_open_door()
{
    ///영상처리, db조회 후에 열라는 슬롯!.
    if(server->ETC.get_garage_door() == 0)
    {
        park_sensor_time = 0;
        server->ETC.set_park_sensor_flag(1);
        ui->pushButton_125->click();
    }
}



void MainWindow::changeState_garage_Door()
{
    //A에게 문여는 명령 전달코드.

    if(server->ETC.get_garage_door() == 0)
    {
        server->ETC.set_garage_door(1);
        ui->pushButton_125->setText("garage door Open");
    }
    else
    {
        server->ETC.set_garage_door(0);
        ui->pushButton_125->setText("garage door Close");
    }

    server->set_A_message(QString("%1,%2,\n")
                                         .arg("P")
                                         .arg(server->ETC.get_garage_door()));

    emit send_value_A();
}



void MainWindow::changeValue_solar_pannel_angle()
{
    ui->label_88->setText(QString("Solar panel angle : %1").arg(ui->dial_45->value()));
    server->ETC.set_solar_pannel_angle1(ui->dial_45->value());

    server->set_B_message(QString("%1,%2,\n")
                                         .arg("Q")
                                         .arg(server->ETC.get_solar_pannel_angle1()));

    emit send_value_B();
}




