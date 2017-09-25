#include "vehicle_registration.h"


void vehicle_registration::set_vehicle_number(QString number)
{
    this->vehicle_number = number;
}

void vehicle_registration::set_vehicle_owner(QString owner)
{
    this->vehicle_owner = owner;
}

void vehicle_registration::set_vehicle_rfid(QString rfid)
{
    this->vehicle_rfid = rfid;
}

QString vehicle_registration::get_vehicle_number()
{
    return this->vehicle_number;
}

QString vehicle_registration::get_vehicle_owner()
{
    return this->vehicle_owner;
}


QString vehicle_registration::set_vehicle_rfid()
{
    return this->vehicle_rfid;
}
