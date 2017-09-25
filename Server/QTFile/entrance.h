#ifndef ENTRANCE_H
#define ENTRANCE_H

#include "base_room.h"

class entrance : public base_room
{
private :
    int knock = 0;
    int body_sensor = 0;
    int body_sensor_flag = 0;
public:

    int r_body_sensor = 0;
    void set_knock(int );
    int get_knock();

    void set_body_sensor(int);
    int get_body_sensor();

    void set_body_sensor_flag(int);
    int get_body_sensor_flag();
};

#endif // ENTRANCE_H
