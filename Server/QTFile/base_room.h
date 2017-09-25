#ifndef BASE_ROOM_H
#define BASE_ROOM_H

#include <QString>
#include <QDateTime>

class base_room
{
    private :
        QDateTime date;
        int temperature = 0;
        int humidity = 0;

        int window1 = 0;
        int window2 = 0;
        int door = 0;

        int LED_OnOff_state = 0;
        int LED_mode = 0;

        int LED_R = 0;
        int LED_G = 0;
        int LED_B = 0;
        int LED_total = 0;

        int Co2 = 0;
        int Smoke = 0;
        int Flame = 0;



    public:
        int r_window1 = 0;
        int r_window2 = 0;
        int r_door = 0;
        int r_LED_OnOff_state = 0;
        int r_LED_mode = 0;
        int r_LED_R = 0;
        int r_LED_G = 0;
        int r_LED_B = 0;
        int r_LED_total = 0;

        void set_datetime(QDateTime);
        void set_send_message(QString);
        void set_temperature(int);
        void set_humidity(int);


        void set_door(int);
        void set_window1(int);
        void set_window2(int);

        void set_LED_OnOff_state(int);
        void set_LED_mode(int);
        void set_LED_R(int);
        void set_LED_G(int);
        void set_LED_B(int);
        void set_LED_total(int);


        void set_Co2(int);
        void set_Smoke(int);
        void set_Flame(int);


        QDateTime get_datetime();
        QString get_send_message();
        int get_temperature();
        int get_humidity();


        int get_door();
        int get_window1();
        int get_window2();


        int get_LED_OnOff_state();
        int get_LED_mode();
        int get_LED_R();
        int get_LED_G();
        int get_LED_B();
        int get_LED_total();

        int get_Co2();
        int get_Smoke();
        int get_Flmae();
};

#endif // BASE_ROOM_H
