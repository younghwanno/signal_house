#ifndef ROOM1_TABLE_H
#define ROOM1_TABLE_H

#include <QObject>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableView>
#include <QtSql>

#include "room1.h"


class room1_table : public QObject
{
    Q_OBJECT

    private:
        QWidget *table;
        QLineEdit *lineEdit;
        QComboBox *comboBox;
        QStandardItemModel *tab_model;
        QSqlQuery *query;
        QPushButton * pushButton;

    public:
        room1_table(QSqlQuery *query);
        void create_table_inDB();
        bool insert_sensor_table(QDateTime , int , int , int , int );
        bool insert_act_table(QDateTime , int , int , int , int , int , int );
        //bool insert_view(QSqlQuery *, QDateTime , int , int , int , int , int , int , int , int , int , int );
        bool insert_view(room1 *);
        QVector<room1 *> *get_records();
        QWidget* show_table();
        QPushButton* get_pushbutton();

    signals:

    public slots:
        void button_slot();

};

#endif // ROOM1_TABLE_H
