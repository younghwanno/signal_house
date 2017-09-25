#include "database.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QPushButton>
#include <QLineEdit>
#include <QCryptographicHash>


home_database::home_database()
{
    this->query = NULL;
}


void home_database::db_start()
{
    if(db_connect())
    {
        living_room_table_object = new living_room_table(this->query);
        kitchen_table_object = new kitchen_table(this->query);
        room1_table_object = new room1_table(this->query);
        room2_table_object = new room2_table(this->query);
        bath_room_table_object = new bath_room_table(this->query);
        multipurpose_room_table_object = new multi_room_table(this->query);
        entrance_table_object = new entrance_table(this->query);
        etc_table_object = new etc_table(this->query);
        vehicle_registration_table_object = new vehicle_registration_table(this->query);
        login_registration_table_object = new login_registration_table(this->query);

        create_tables();
    }
}


void home_database::create_tables()
{
    living_room_table_object->create_table_inDB();
    kitchen_table_object->create_table_inDB();
    room1_table_object->create_table_inDB();
    room2_table_object->create_table_inDB();
    bath_room_table_object->create_table_inDB();
    multipurpose_room_table_object->create_table_inDB();
    entrance_table_object->create_table_inDB();
    etc_table_object->create_table_inDB();
    vehicle_registration_table_object->create_table_inDB();
    login_registration_table_object->create_table_inDB();
}


bool home_database::db_connect()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    bool val = false;

    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("epqldks");

    if (!db.open()){
        qDebug() << "db접속 실패..";        
    }else{
        qDebug() << "db접속 성공..";

        this->query = new QSqlQuery(db);
        this->query->exec("create database if not exists home_database");
        this->query->exec("use home_database;");

        val = true;
    }

    return val;
}


living_room_table* home_database::get_living_room_table_object()
{
    return this->living_room_table_object;
}

kitchen_table * home_database::get_kitchen_table_object()
{
    return this->kitchen_table_object;
}

room1_table * home_database::get_room1_table_object()
{
    return this->room1_table_object;
}

room2_table * home_database::get_room2_table_object()
{
    return this->room2_table_object;
}

bath_room_table * home_database::get_bath_room_table_object()
{
    return this->bath_room_table_object;
}

multi_room_table * home_database::get_multipurpose_room_table_object()
{
    return this->multipurpose_room_table_object;
}

entrance_table * home_database::get_entrance_table_object()
{
    return this->entrance_table_object;
}

etc_table * home_database::get_etc_table_object()
{
    return this->etc_table_object;
}

vehicle_registration_table * home_database::get_vehicle_registration_table_object()
{
    return this->vehicle_registration_table_object;
}

login_registration_table* home_database::get_login_registration_table_object()
{
    return this->login_registration_table_object;
}



QSqlQuery* home_database::get_query()
{
    return this->query;
}

