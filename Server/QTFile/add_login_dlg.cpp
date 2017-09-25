#include <QtGui>

#include "add_login_dlg.h"

login_add_dlg::login_add_dlg(QSqlQuery *query)
{
    this->query = query;
    init();
    createAction();
}


void login_add_dlg::init()
{
    login_ID       = new QLabel("login_ID");
    login_PW       = new QLabel("login_PW");
    login_name     = new QLabel("login_name");

    te_login_ID 	= new QLineEdit("");
    te_login_PW     = new QLineEdit("");
    te_login_name	= new QLineEdit("");

    formGroupBox = new QGroupBox("ID 추가");
    QFormLayout *flayout = new QFormLayout;
    flayout->addRow(login_ID, te_login_ID);
    flayout->addRow(login_PW, te_login_PW);
    flayout->addRow(login_name, te_login_name);

    formGroupBox->setLayout(flayout);

    pbt_add = new QPushButton("등록");
    vGroupBox = new QGroupBox(tr("Action"));
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(pbt_add);
    vGroupBox->setLayout(vlayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(vGroupBox);
    setLayout(mainLayout);
}


void login_add_dlg::createAction()
{
    connect(pbt_add, SIGNAL(clicked()), this, SLOT(db_insert()));
}



void login_add_dlg::db_insert()
{
    QString ID          = te_login_ID->text();
    QString PW          = te_login_PW->text();
    QString name        = te_login_name->text();

    if(ID.length() < 1)
    {
        QMessageBox::warning( this, "Error Message","ID를 입력해 주세요.");
    }
    if(ID.length() < 4)
    {
        QMessageBox::warning( this, "Error Message","ID를 4 입력해 주세요.");
    }
    else if(PW.length() < 1)
    {
        QMessageBox::warning( this, "Error Message","PW를 입력해 주세요.");
    }
    else if(PW.length() < 8)
    {
        QMessageBox::warning( this, "Error Message","PW를 8자이상 입력해 주세요.");
    }
    else if(name.length() < 1)
    {
        QMessageBox::warning( this, "Error Message","이름을 입력해 주세요.");
    }
    else{

        query->exec("START TRANSACTION");

        QByteArray tp = PW.toLocal8Bit();
        tp = QCryptographicHash::hash(PW.toLocal8Bit(),QCryptographicHash::Sha256);
        PW = QString(tp.toBase64());
        PW.resize(PW.length() - 1);


        QString text = QString("insert into login_registration_table(login_ID, login_PW, login_name)"
                               " VALUES('%1','%2','%3')").arg(ID).arg(PW).arg(name);

        if(query->exec(text))
        {
            query->exec("COMMIT");
            accept();
        }
        else
        {
            query->exec("ROLLBACK");
            reject();
        }

    }

}



