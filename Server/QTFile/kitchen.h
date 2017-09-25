#ifndef KITCHEN_H
#define KITCHEN_H

#include "base_room.h"

class kitchen : public base_room
{
    private :
        int LPG = 0;
        int ventilating_fan = 0;
    public :

        int r_ventilating_fan = 0;
        void set_LPG(int);
        void set_ventilating_fan(int);


        int get_LPG();     
        int get_ventilating_fan();
};

#endif // KITCHEN_H
