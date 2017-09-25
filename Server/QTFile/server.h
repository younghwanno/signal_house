#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>


#include "client_thread.h"
#include "bath_room.h"
#include "entrance.h"
#include "kitchen.h"
#include "living_room.h"
#include "multipurpose_room.h"
#include "room1.h"
#include "room2.h"
#include "etc.h"




class chatServer : public QTcpServer
{
    Q_OBJECT
    private :
        QString pre_text;
        QSqlQuery* query;
        QMutex* my_mutex;

    public:
        QMutex* get_mutex();

        void set_query(QSqlQuery *query);
        QSqlQuery* get_query();

        bath_room Bath_room;
        entrance Entrance;
        kitchen Kitchen;
        living_room Living_room;
        multipurpose_room Multipurpose_room;
        room1 Room1;
        room2 Room2;
        etc ETC;

        CountWorker* client_list[5] = {NULL,};
        chatServer();

        QString A_message;
        QString B_message;
        QString E_message;

        void set_A_message(QString);
        void set_B_message(QString);
        void set_E_message(QString);

        QString get_A_message();
        QString get_B_message();
        QString get_E_message();

     public slots:
        void send_value_E_client();


    private slots:    
        void disconnected();
        void update_value_C();
        void update_value_D();
        void update_value_E();

        void send_value_A_client();
        void send_value_B_client();


    protected slots:        
        void incomingConnection();


    signals:
        void update_value_C_client();
        void update_value_D_client();
        void update_value_E_client();

};
