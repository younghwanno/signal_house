#include <QtGui>
#include <QTcpSocket>
#include <QRegExp>
#include "server.h"
#include "client_thread.h"




chatServer::chatServer()
{
    QFont font( "unifont", 10, QFont::Bold);
    my_mutex = new QMutex();
    connect(this, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}


QSqlQuery* chatServer::get_query()
{
    return this->query;
}


void chatServer::set_query(QSqlQuery *query)
{
    this->query = query;
}

void chatServer::incomingConnection()
{
    QTcpSocket *client = this->nextPendingConnection();

    qDebug() << "New client from:" << client->peerAddress().toString();

    QThread     *workerThread = new QThread;
    CountWorker *worker = new CountWorker(this,client);

    worker->moveToThread(workerThread);
    workerThread->start();
}


void chatServer::update_value_C() {emit update_value_C_client();}
void chatServer::update_value_D() {emit update_value_D_client();}
void chatServer::update_value_E() {emit update_value_E_client();}


void chatServer::disconnected()
{
}

QMutex* chatServer::get_mutex()
{
    return this->my_mutex;
}


void chatServer::send_value_A_client()
{

    if(this->client_list[0] != NULL)
    {
        QString temp = this->get_A_message();
        this->client_list[0]->get_client_sock()->write(temp.toStdString().data());
        qDebug() << "A :" + temp;
    }
}


void chatServer::send_value_B_client()
{
    if(this->client_list[1] != NULL)
    {
        QString temp = this->get_B_message();
        this->client_list[1]->get_client_sock()->write(temp.toStdString().data());
        qDebug() << "B :" + temp;
    }
}


void chatServer::send_value_E_client()
{
    if(this->client_list[4] != NULL)
    {
        if(pre_text !=  this->get_E_message())
        {
        QString temp = this->get_E_message();
        this->client_list[4]->get_client_sock()->write(temp.toStdString().data());
        qDebug() << "E :" + temp;

        pre_text = temp;
        }
    }
}


void chatServer::set_A_message(QString message) {this->A_message = message;}
void chatServer::set_B_message(QString message) {this->B_message = message;}
void chatServer::set_E_message(QString message) {this->E_message = message;}


QString chatServer::get_A_message() {return this->A_message;}
QString chatServer::get_B_message() {return this->B_message;}
QString chatServer::get_E_message() {return this->E_message;}
