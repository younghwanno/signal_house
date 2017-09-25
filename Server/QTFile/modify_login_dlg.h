#ifndef MODIFY_LOGIN_DLG_H
#define MODIFY_LOGIN_DLG_H

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

#include "login_registration.h"

class MainWindow;


class login_modify_dlg : public QDialog
{
    Q_OBJECT

public:
    login_modify_dlg(QSqlQuery *,int );

private:
    QString pre_ID;

    QSqlQuery *query;
    QTextCodec 	*codec;

    QGroupBox *formGroupBox;
    QGroupBox *vGroupBox;

    QLabel *login_ID;
    QLabel *login_PW;
    QLabel *login_name;

    QLineEdit *te_login_ID;
    QLineEdit *te_login_PW;
    QLineEdit *te_login_name;


    QPushButton *pbt_modify;

    int modify_num;

    void init(int num);
    void createAction();
    QVector<login_registration *> *get_records();

private slots:
    void record_modify();
};


#endif // MODIFY_LOGIN_DLG_H
