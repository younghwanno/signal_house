#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>


#include "graph_thread.h"
#include "open_cv_thread.h"

#include "server.h"
#include "database.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QMediaPlayer>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QCameraInfo>
#include <QList>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        int pre_num = 0;
        int body_sensor_time = 0;
        int park_sensor_time = 0;
        bool voltage_relay = false;

        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void init_room1();
        void init_room2();
        void init_multipurpose_room();
        void init_kitchen();
        void init_living_room();
        void init_bath_room();
        void init_entrance();
        void init_etc();
        void init_air_view();

        void init_graph_thread();
        void init_open_cv_thread();
        void init_home_database();
        void init_timer();

        void init_camera();       
        void change_energy_image(int, int, int);


    protected :
        void closeEvent(QCloseEvent *event);

    private:

        Ui::MainWindow *ui;
        chatServer *server;

        testing *graph_worker;
        open_cv_worker *opencv_worker;
        home_database *database;
        QTimer *timer;

        QCamera * camera1;
        int camera_state_flag1;
        QCameraImageCapture *image_capture1;
        QMediaRecorder *video_record1;

        int camera_state_flag2;
        QCameraImageCapture *image_capture2;

    public slots:
         void timer_count();
    private slots:

        void changeState_garage_Door();
        void notice_open_door();
        void show_living_room_table();
        void show_kitchen_table();
        void show_room1_table();
        void show_room2_table();
        void show_bath_room_table();
        void show_multi_room_table();
        void show_entrance_table();
        void show_etc_table();

        void show_vehicle_registration_table();
        void show_login_registration_table();

        void camera_capture1();
        void camera_on_off1();

        void camera_capture2();
        void camera_on_off2();
        void camera2_image_processing();

        void update_value_C();
        void update_value_D();
        void update_value_E();

        //거실
        void changeValue_living_room_LED();
        void changeValue_living_room_radio();
        void changeState_living_room_LED_onoff();
        void changeState_living_room_Window1();
        void changeState_living_room_Window2();


        //부엌
        void changeValue_kitchen_LED();
        void changeValue_kitchen_radio();
        void changeState_kitchen_LED_onoff();
        void changeState_kitchen_Window1();
        void changeState_kitchen_Window2();
        void changeState_kitchen_ventilating_fan();


        //방1
        void changeValue_room1_LED();
        void changeValue_room1_radio();
        void changeState_room1_LED_onoff();
        void changeState_room1_Window1();
        void changeState_room1_Door();


        //방2
        void changeValue_room2_LED();
        void changeValue_room2_radio();
        void changeState_room2_LED_onoff();
        void changeState_room2_Window1();
        void changeState_room2_Door();


        //화장실
        void changeValue_bath_room_LED();
        void changeValue_bath_room_radio();
        void changeState_bath_room_LED_onoff();
        void changeState_bath_room_Door();
        void changeState_bath_room_ventilating_fan();


        //다목적
        void changeValue_multipurpose_room_LED();
        void changeValue_multipurpose_room_radio();
        void changeState_multipurpose_room_LED_onoff();
        void changeState_multipurpose_room_Window1();
        void changeState_multipurpose_room_Window2();
        void changeState_multipurpose_room_Door();


        //현관
        void body_sensor_onoff();
        void changeValue_entrance_LED();
        void changeState_entrance_LED_onoff();
        void changeState_entrance_Door();


        //etc
        void changeState_Pump();
        void changeState_Boiler();
        void changeState_Windows();
        void changeState_Doors();
        void changeValue_home_mode();
        void changeValue_solar_pannel_angle();

        //////태양열 컨트롤러


    signals:
        void send_value_A();
        void send_value_B();
        void send_value_E();

};

#endif // MAINWINDOW_H
