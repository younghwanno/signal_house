#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>
#include <QDebug>
#include <QDateTime>
#include <QVector>
#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QTableView>
#include <QComboBox>
#include <QStandardItemModel>

#include "living_room_table.h"
#include "kitchen_table.h"
#include "room1_table.h"
#include "room2_table.h"
#include "bath_room_table.h"
#include "multipurpose_room_table.h"
#include "entrance_table.h"
#include "etc_table.h"
#include "vehicle_registration_table.h"
#include "login_registration_table.h"



class home_database : public QObject
{
    Q_OBJECT

    private:
    QSqlDatabase db;
    QSqlQuery *query;

    living_room_table* living_room_table_object;
    kitchen_table * kitchen_table_object;
    room1_table * room1_table_object;
    room2_table * room2_table_object;
    bath_room_table * bath_room_table_object;
    multi_room_table * multipurpose_room_table_object;
    entrance_table * entrance_table_object;
    etc_table * etc_table_object;
    vehicle_registration_table * vehicle_registration_table_object;
    login_registration_table * login_registration_table_object;


    public:
    home_database();
    bool db_connect(); // db없으면 만들기
    void db_start();
    void create_tables();
    QSqlQuery* get_query();


    living_room_table* get_living_room_table_object();
    kitchen_table * get_kitchen_table_object();
    room1_table * get_room1_table_object();
    room2_table * get_room2_table_object();
    bath_room_table * get_bath_room_table_object();
    multi_room_table * get_multipurpose_room_table_object();
    entrance_table * get_entrance_table_object();
    etc_table * get_etc_table_object();
    vehicle_registration_table * get_vehicle_registration_table_object();
    login_registration_table* get_login_registration_table_object();

    signals:

    public slots:

};



#endif // DATABASE_H
