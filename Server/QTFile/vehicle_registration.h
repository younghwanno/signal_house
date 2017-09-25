#ifndef VEHICLE_REGISTRATION_H
#define VEHICLE_REGISTRATION_H

#include <QString>


class vehicle_registration
{
    private :
        QString vehicle_number;
        QString vehicle_owner;
        QString vehicle_rfid;

    public:
        void set_vehicle_number(QString);
        void set_vehicle_owner(QString);
        void set_vehicle_rfid(QString);

        QString get_vehicle_number();
        QString get_vehicle_owner();
        QString set_vehicle_rfid();
};

#endif // VEHICLE_REGISTRATION_H
