#ifndef ENTRANCE_TABLE_H
#define ENTRANCE_TABLE_H

#include <QObject>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableView>
#include <QtSql>

#include "entrance.h"


class entrance_table : public QObject
{
    Q_OBJECT

    private:
        QWidget *table;
        QLineEdit *lineEdit;
        QComboBox *comboBox;
        QStandardItemModel *tab_model;
        QSqlQuery * query;
        QPushButton * pushButton;

    public:
        entrance_table(QSqlQuery *);
        void create_table_inDB();
        bool insert_sensor_table(QDateTime , int , int );
        bool insert_act_table(QDateTime , int , int , int , int , int );
        bool insert_view(entrance*);
        QVector<entrance *> *get_records();
        QWidget* show_table();
        QPushButton* get_pushbutton();

    signals:

    public slots:
        void button_slot();

};

#endif // ENTRANCE_TABLE_H
