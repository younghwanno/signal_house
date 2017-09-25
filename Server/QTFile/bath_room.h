#ifndef BATH_ROOM_H
#define BATH_ROOM_H

#include "base_room.h"

class bath_room : public base_room
{
    private :
        int ventilating_fan = 0;

    public :
        int r_ventilating_fan = 0;

        void set_ventilating_fan(int);
        int get_ventilating_fan();
};

#endif // BATH_ROOM_H
