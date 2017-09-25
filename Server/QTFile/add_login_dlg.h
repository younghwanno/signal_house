#ifndef ADD_LOGIN_DLG_H
#define ADD_LOGIN_DLG_H


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

class login_add_dlg : public QDialog
{
    Q_OBJECT

public:
    login_add_dlg(QSqlQuery *);

private:
    QTextCodec 	*codec;
    QGroupBox *formGroupBox;
    QGroupBox *vGroupBox;

    QLabel *login_ID;
    QLabel *login_PW;
    QLabel *login_name;

    QLineEdit *te_login_ID;
    QLineEdit *te_login_PW;
    QLineEdit *te_login_name;

    QPushButton *pbt_add;
    QSqlQuery *query;

    void init();
    void createAction();

private slots:
    void db_insert();
};


#endif // ADD_LOGIN_DLG_H
