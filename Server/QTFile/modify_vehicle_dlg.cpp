#include <QtGui>
#include "modify_vehicle_dlg.h"


vehicle_modify_dlg::vehicle_modify_dlg(QSqlQuery *query, int num)
{
    this->query = query;
    setWindowTitle("수정");

    init(num);
    createAction();

}


void vehicle_modify_dlg::init(int num)
{

    vehicle_number  = new QLabel("vehicle_number");
    vehicle_owner   = new QLabel("vehicle_owner");
    vehicle_rfid   = new QLabel("vehicle_rfid");


    te_vehicle_number       = new QLineEdit("");
    te_vehicle_owner        = new QLineEdit("");
    te_vehicle_rfid        = new QLineEdit("");

    formGroupBox = new QGroupBox("주소 입력란");

    QFormLayout *flayout = new QFormLayout;
    flayout->addRow(vehicle_number, te_vehicle_number);
    flayout->addRow(vehicle_owner, te_vehicle_owner);
    flayout->addRow(vehicle_rfid, te_vehicle_rfid);


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

    QVector<vehicle_registration *> *list2 = this->get_records();
    pre_number = list2->at(num)->get_vehicle_number();

    QString text = QString("select * from vehicle_registration_table where vehicle_number = '%1'").arg(pre_number);
    query->exec(text);
    query->next();

    QString number 		= query->value(0).toString();
    QString owner       = query->value(1).toString();
    QString rfid       = query->value(2).toString();

    te_vehicle_number->setText(number);
    te_vehicle_owner->setText(owner);
    te_vehicle_rfid->setText(rfid);

    modify_num = num;

}



void vehicle_modify_dlg::createAction()
{
    connect(pbt_modify, SIGNAL(clicked()), this, SLOT(record_modify()));
}



void vehicle_modify_dlg::record_modify()
{
    QString number   = te_vehicle_number->text();
    QString owner	 = te_vehicle_owner->text();
    QString rfid	 = te_vehicle_rfid->text();


    if(number.length() < 1)
    {
        QMessageBox::warning( this, "Error Message", "차량번호를 입력해 주세요.");
    }
    else if(number.length() != 7 )
    {
        QMessageBox::warning( this, "Error Message","차량번호 7자를 입력해 주세요.");
    }
    else if(owner.length() < 1)
    {
        QMessageBox::warning( this, "Error Message", "차주 이름을 입력해 주세요.");
    }
    else if(rfid.length() < 1 && !rfid.toInt())
    {
        QMessageBox::warning( this, "Error Message", "rfid를 입력해주세요(숫자로)");
    }
    else{

        QString text;
        text = QString("update vehicle_registration_table set vehicle_number = '%1', vehicle_owner ='%2', vehicle_rfid = %3 "
                       " where vehicle_number = '%4'").arg(number).arg(owner).arg(rfid.toInt()).arg(pre_number);

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



QVector<vehicle_registration *> *vehicle_modify_dlg::get_records()
{

     QVector<vehicle_registration *> *list = new QVector<vehicle_registration *>;
    query->prepare("select * from vehicle_registration_table");
    query->exec();

    while(query->next())
    {
        vehicle_registration * temp = new vehicle_registration();
        temp->set_vehicle_number(query->value(0).toString());
        temp->set_vehicle_owner(query->value(1).toString());

        list->append(temp);
    }

    return list;
}
