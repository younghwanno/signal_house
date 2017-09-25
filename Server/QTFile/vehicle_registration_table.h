#ifndef VEHICLE_REGISTRATION_TABLE_H
#define VEHICLE_REGISTRATION_TABLE_H

#include <QObject>
#include <QVector>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableView>
#include <QtSql>
#include <QToolBar>
#include <QItemSelectionModel>


#include "vehicle_registration.h"
#include "add_vehicle_dlg.h"
#include "modify_vehicle_dlg.h"


class vehicle_registration_table : public QObject
{
    Q_OBJECT

    private:
        QWidget* table;
        QLineEdit *lineEdit;
        QComboBox *comboBox;
        QStandardItemModel *tab_model;
        QSqlQuery *query;
        QPushButton * pushButton;
        QPushButton * pushButton_modify;
        QPushButton * pushButton_del;
        QPushButton * pushButton_add;
        QLabel *query_result;
        QItemSelectionModel* tab_selectionModel;
        QTableView * table_view;
        int selected_num;

    public:
        QVector<vehicle_registration *> *get_records();
        vehicle_registration_table(QSqlQuery *);
        void create_table_inDB();
        bool insert_table(QString, QString);
        bool insert_view(vehicle_registration* );
        QWidget* show_table();
        QPushButton* get_pushbutton();
        QSqlQuery * get_query();

    signals:

    private slots:
        void slot_table_view(const QModelIndex &index);
        void button_slot();
        void slot_add();
        void slot_modify();
        void slot_delete();

};

#endif // VEHICLE_REGISTRATION_TABLE_H
