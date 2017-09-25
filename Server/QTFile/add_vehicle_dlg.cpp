#include <QtGui>

#include "add_vehicle_dlg.h"

vehicle_add_dlg::vehicle_add_dlg(QSqlQuery *query)
{
    this->query = query;
    init();
    createAction();
}


void vehicle_add_dlg::init()
{
    vehicle_number      = new QLabel("vehicle_number");
    vehicle_owner       = new QLabel("vehicle_ownver");
    vehicle_rfid       = new QLabel("vehicle_rfid");

    te_vehicle_number 	= new QLineEdit("");
    te_vehicle_owner	= new QLineEdit("");
    te_vehicle_rfid	= new QLineEdit("");

    formGroupBox = new QGroupBox("차량 번호 등록시 띄어쓰기 없이..");
    QFormLayout *flayout = new QFormLayout;
    flayout->addRow(vehicle_number, te_vehicle_number);
    flayout->addRow(vehicle_owner, te_vehicle_owner);
    flayout->addRow(vehicle_rfid, te_vehicle_rfid);


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


void vehicle_add_dlg::createAction()
{
    connect(pbt_add, SIGNAL(clicked()), this, SLOT(db_insert()));
}


void vehicle_add_dlg::db_insert()
{
    QString number = te_vehicle_number->text();
    QString owner	 = te_vehicle_owner->text();
    QString rfid	 = te_vehicle_rfid->text();

    if(number.length() < 1)
    {
        QMessageBox::warning( this, "Error Message","차량번호를 입력해 주세요.");
    }
    else if(number.length() != 7 )
    {
        QMessageBox::warning( this, "Error Message","차량번호 7자를 입력해 주세요.");
    }
    else if(owner.length() < 1)
    {
        QMessageBox::warning( this, "Error Message", "차주 이름을 입력해 주세요.");
    }
    else if(rfid.length() < 1)
    {
        QMessageBox::warning( this, "Error Message", "rfid 번호를 입력해 주세요.");
    }
    else if(!rfid.toInt())
    {
        QMessageBox::warning( this, "Error Message", "rfid 숫자를 입력하세요.");
    }
    else{

        query->exec("START TRANSACTION");

        QString text = QString("insert into vehicle_registration_table(vehicle_number, vehicle_owner, vehicle_rfid) VALUES('%1','%2',%3)").arg(number).arg(owner).arg(rfid.toInt());

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



