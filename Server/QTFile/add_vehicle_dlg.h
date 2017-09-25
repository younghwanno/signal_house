#ifndef ADD_VEHICLE_DLG_H
#define ADD_VEHICLE_DLG_H

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
#include <QMessageBox>
#include <QFormLayout>
#include <QVBoxLayout>


class MainWindow;

class vehicle_add_dlg : public QDialog
{
    Q_OBJECT

public:
    vehicle_add_dlg(QSqlQuery *);

private:
    QTextCodec 	*codec;
    QGroupBox *formGroupBox;
    QGroupBox *vGroupBox;

    QLabel *vehicle_number;
    QLabel *vehicle_owner;
    QLabel *vehicle_rfid;

    QLineEdit *te_vehicle_number;
    QLineEdit *te_vehicle_owner;
    QLineEdit *te_vehicle_rfid;

    QPushButton *pbt_add;
    QSqlQuery *query;

    void init();
    void createAction();

private slots:
    void db_insert();
};

#endif // ADD_VEHICLE_DLG_H
