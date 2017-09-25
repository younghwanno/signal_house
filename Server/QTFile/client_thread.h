#ifndef __COUNT_WORKER_H__
#define __COUNT_WORKER_H__

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>


class CountWorker : public QObject
{
    Q_OBJECT

    private:
        QTcpServer *server;
        QTcpSocket *client;
        QString receive_message;


    public:
        CountWorker(QTcpServer* server, QTcpSocket* client);
        QTcpSocket* get_client_sock();


    public slots:

    signals:
        void updateCount(int);
        void finished();

        void update_value_A();
        void update_value_B();
        void update_value_C();
        void update_value_D();
        void update_value_E();

    private slots:
        void readyRead();
        void disconnected();
};

#endif // __COUNT_WORKER_H_
