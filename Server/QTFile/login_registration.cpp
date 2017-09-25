#include "login_registration.h"


void login_registration::set_login_ID(QString ID)
{
    this->login_ID = ID;
}

void login_registration::set_login_PW(QString PW)
{
    this->login_PW = PW;
}

void login_registration::set_login_name(QString name)
{
    this->login_name = name;
}

QString login_registration::get_login_ID()
{
    return this->login_ID;
}

QString login_registration::get_login_PW()
{
    return this->login_PW;
}

QString login_registration::get_login_name()
{
    return this->login_name;
}
