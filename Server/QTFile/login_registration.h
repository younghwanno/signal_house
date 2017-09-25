#ifndef LOGIN_REGISTRATION_H
#define LOGIN_REGISTRATION_H

#include <QString>


class login_registration
{

    private :
        QString login_ID;
        QString login_PW;
        QString login_name;

    public:
        void set_login_ID(QString);
        void set_login_PW(QString);
        void set_login_name(QString);

        QString get_login_ID();
        QString get_login_PW();
        QString get_login_name();

};

#endif // LOGIN_REGISTRATION_H
