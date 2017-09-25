#ifndef MODIFY_VEHICLE_DLG_H
#define MODIFY_VEHICLE_DLG_H
#include <QDialog>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


#include <QAction>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "vehicle_registration.h"

class MainWindow;


class vehicle_modify_dlg : public QDialog
{
    Q_OBJECT

public:
    vehicle_modify_dlg(QSqlQuery *,int );

private:
    QString pre_number;

    QSqlQuery *query;
    QTextCodec 	*codec;

    QGroupBox *formGroupBox;
    QGroupBox *vGroupBox;

    QLabel *vehicle_number;
    QLabel *vehicle_owner;
    QLabel *vehicle_rfid;

    QLineEdit *te_vehicle_number;
    QLineEdit *te_vehicle_owner;
    QLineEdit *te_vehicle_rfid;

    QPushButton *pbt_modify;

    int modify_num;

    void init(int num);
    void createAction();
    QVector<vehicle_registration *> *get_records();

private slots:
    void record_modify();
};

#endif // MODIFY_VEHICLE_DLG_H
