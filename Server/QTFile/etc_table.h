#ifndef ETC_TABLE_H
#define ETC_TABLE_H

#include <QObject>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableView>
#include <QtSql>

#include "etc.h"


class etc_table : public QObject
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
        etc_table(QSqlQuery* );
        void create_table_inDB();
        bool insert_sensor_table(QDateTime , int , int , int , int , int , int , float, float, float, float);
        bool insert_act_table( QDateTime , int , int );
        bool insert_view(etc* );
        QVector<etc *> *get_records();
        QWidget* show_table();
        QPushButton* get_pushbutton();

    signals:

    public slots:
        void button_slot();

};

#endif // ETC_TABLE_H
