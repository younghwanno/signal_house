#include <QtGui>
#include "modify_login_dlg.h"


login_modify_dlg::login_modify_dlg(QSqlQuery *query, int num)
{
    this->query = query;
    setWindowTitle("수정");

    init(num);
    createAction();

}


void login_modify_dlg::init(int num)
{
    login_PW    = new QLabel("login_PW");
    login_name  = new QLabel("login_name");

    te_login_PW         = new QLineEdit("");
    te_login_name       = new QLineEdit("");

    formGroupBox = new QGroupBox("로그인 정보 수정");

    QFormLayout *flayout = new QFormLayout;
    flayout->addRow(login_PW, te_login_PW);
    flayout->addRow(login_name, te_login_name);


    formGroupBox->setLayout(flayout);

    pbt_modify = new QPushButton("수정");
    vGroupBox = new QGroupBox(tr("Action"));
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(pbt_modify);
    vGroupBox->setLayout(vlayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(vGroupBox);
    setLayout(mainLayout);

    QVector<login_registration *> *list2 = this->get_records();
    pre_ID = list2->at(num)->get_login_ID();


    QString text = QString("select * from login_registration_table where login_ID = '%1'").arg(pre_ID);
    query->exec(text);
    query->next();

    QString PW           = query->value(1).toString();
    QString name         = query->value(2).toString();

    te_login_PW->setText(PW);
    te_login_name->setText(name);

    modify_num = num;
}



void login_modify_dlg::createAction()
{
    connect(pbt_modify, SIGNAL(clicked()), this, SLOT(record_modify()));
}



void login_modify_dlg::record_modify()
{
    QString PW	 = te_login_PW->text();
    QString name = te_login_name->text();

    if(PW.length() < 1)
    {
        QMessageBox::warning( this, "Error Message", "비밀번를 입력해 주세요.");
    }
    else if(PW.length() < 8)
    {
        QMessageBox::warning( this, "Error Message", "비밀번를 8자 이상 입력해 주세요.");
    }
    else if(name.length() < 1)
    {
        QMessageBox::warning( this, "Error Message", "이름을 입력해 주세요.");
    }
    else{

        QString text;
        text = QString("update login_registration_table set login_PW = '%1', login_name ='%2' "
                       " where login_ID = '%3'").arg(PW).arg(name).arg(pre_ID);

        query->exec("START TRANSACTION");

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



QVector<login_registration *> *login_modify_dlg::get_records()
{
     QVector<login_registration *> *list = new QVector<login_registration *>;
    query->prepare("select * from login_registration_table");
    query->exec();

    while(query->next())
    {
        login_registration * temp = new login_registration();

        temp->set_login_ID(query->value(0).toString());
        temp->set_login_PW(query->value(1).toString());
        temp->set_login_name(query->value(2).toString());

        list->append(temp);
    }

    return list;
}
