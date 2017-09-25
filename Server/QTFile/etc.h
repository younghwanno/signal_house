#ifndef ETC_H
#define ETC_H

#include <QString>
#include "base_room.h"

class etc : public base_room
{
    private :
        int rain1 = 0;
        int rain2 = 0;

        int human1 = 0;
        int human2 = 0;


        int vibration = 0;
        int dust = 0;
        float UV = 0;

        float load_cell = 0;

        int pump = 0;
        int boiler = 0;

        int windows = 0;
        int doors = 0;
        int home_mode = 0;
        int garage_door = 0;

        int solar_pannel_angle1 = 0; //상하
        int solar_pannel_angle2 = 0; //좌우

        int solar_pannel_angle3 = 0; //상하
        int solar_pannel_angle4 = 0; //좌우




        int park_sensor_flag = 0;

        int current = 0;
        float voltage = 0;

        int current2 = 0;
        float voltage2 = 0;

        float battery1_capacity;
        float battery2_capacity;

        int Solar_to_Battery = 0;
        int Solar_to_DC = 0;
        int Batter_to_DC = 0;

        int rfid = 0;




    public:

        int light[8];

        int r_solar_pannel_angle = 0;
        int r_garage_door = 0;

        void set_park_sensor_flag(int);

        void set_solar_pannel_angle1(int);
        void set_solar_pannel_angle2(int);
        void set_solar_pannel_angle3(int);
        void set_solar_pannel_angle4(int);


        void set_garage_door(int);    
        void set_UV(float);

        void set_rain1(int);
        void set_rain2(int);

        void set_human1(int);
        void set_human2(int);

        void set_current(int);
        void set_current2(int);

        void set_vibration(int);
        void set_dust(int);
        void set_pump(int);
        void set_boiler(int);
        void set_windows(int);
        void set_doors(int);
        void set_home_mode(int);

        void set_voltage(float);
        void set_voltage2(float);

        void set_load_cell(float);

        void set_Solar_to_Battery(int);
        void set_Solar_to_DC(int);
        void set_Batter_to_DC(int);
        void set_rfid(int);

        void set_light(int a[]);
        void set_battery1_capacity(float);
        void set_battery2_capacity(float);



        int get_park_sensor_flag();

        int get_solar_pannel_angle1();
        int get_solar_pannel_angle2();
        int get_solar_pannel_angle3();
        int get_solar_pannel_angle4();

        int get_garage_door();

        float get_UV();

        int get_rain1();
        int get_rain2();

        int get_human1();
        int get_human2();

        int get_current();
        int get_current2();

        int get_vibration();
        int get_dust();
        int get_pump();
        int get_boiler();
        int get_windows();
        int get_doors();
        int get_home_mode();

        float get_voltage();
        float get_voltage2();

        float get_load_cell();


        int get_Solar_to_Battery();
        int get_Solar_to_DC();
        int get_Batter_to_DC();
        int get_rfid();

        int* get_light();

        float get_battery1_capacity();
        float get_battery2_capacity();



};


#endif // ETC_H
