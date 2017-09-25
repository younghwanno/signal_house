#ifndef room2_TABLE_H
#define room2_TABLE_H

#include <QObject>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableView>
#include <QtSql>

#include "room2.h"


class room2_table : public QObject
{
    Q_OBJECT

    private:
        QWidget* table;
        QLineEdit *lineEdit;
        QComboBox *comboBox;
        QStandardItemModel *tab_model;
        QSqlQuery *query;
        QPushButton * pushButton;

    public:
        room2_table(QSqlQuery *query);
        void create_table_inDB();
        bool insert_sensor_table(QDateTime , int , int , int , int );
        bool insert_act_table(QDateTime , int , int , int , int , int , int );
        QVector<room2 *> *get_records();
        bool insert_view(room2 * );
        QWidget* show_table();
        QPushButton* get_pushbutton();

    signals:

    public slots:
        void button_slot();

};

#endif // room2_TABLE_H
