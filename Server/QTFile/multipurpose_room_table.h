#ifndef MULTIPURPOSE_ROOM_TABLE_H
#define MULTIPURPOSE_ROOM_TABLE_H

#include <QObject>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableView>
#include <QtSql>

#include "multipurpose_room.h"


class multi_room_table : public QObject
{
    Q_OBJECT

    private:
        QWidget * table;
        QLineEdit *lineEdit;
        QComboBox *comboBox;
        QStandardItemModel *tab_model;
        QSqlQuery *query;
        QPushButton * pushButton;

    public:
        multi_room_table(QSqlQuery *query);
        void create_table_inDB();
        bool insert_sensor_table(QDateTime , int , int );
        bool insert_act_table(QDateTime , int , int , int , int , int , int , int );
        //bool insert_view(QSqlQuery *, QDateTime , int , int , int , int , int , int , int , int , int );
        bool insert_view(multipurpose_room*);
        QVector<multipurpose_room *> *get_records();
        QWidget* show_table();
        QPushButton* get_pushbutton();

    signals:

    public slots:
        void button_slot();

};

#endif // MULTIPURPOSE_ROOM_TABLE_H
