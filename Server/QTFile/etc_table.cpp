#include "etc_table.h"

etc_table::etc_table(QSqlQuery *query)
{
    this->query = query;
    this->table = NULL;
}

void etc_table::create_table_inDB()
{
    query->exec("create table if not exists etc_sensor(date datetime not null primary key,"
                " temp int not null, humi int not null, UV int, dust int, rain1 int not null, rain2 int not null, voltage1 float, voltage2 float, current1 float, current2 float)");

    query->exec("create table if not exists etc_act(date datetime not null primary key, pump int not null, heating int not null)");

    query->exec("create view etc_view as select a.date, a.temp, a.humi,"
                " a.UV, a.dust, a.rain1, a.rain2, a.voltage1, a.voltage2, a.current1, a.current2,"
                " b.pump, b.heating from etc_sensor a, etc_act b where a.date = b.date");
}


bool etc_table::insert_view(etc* room)
{
    query->exec("START TRANSACTION");

    bool x = insert_sensor_table(room->get_datetime(),room->get_temperature(),
                                 room->get_humidity(),room->get_UV(),room->get_dust(),room->get_rain1(),
                                 room->get_rain2(),room->get_voltage(),room->get_voltage2(),room->get_current(),room->get_current2());
    bool y = insert_act_table(room->get_datetime(),room->get_pump(),room->get_boiler());

    if(x && y)
    {
        query->exec("COMMIT");
        qDebug() << "commit";
    }
    else
    {
        query->exec("ROLLBACK");
        qDebug() << "rollback";
    }

    return x && y;
}


bool etc_table::insert_sensor_table(QDateTime date, int temp, int humi, int UV, int dust, int rain1, int rain2, float voltage1, float voltage2, float current1, float current2)
{
     bool val = false;

     query->prepare("insert into etc_sensor(date, temp, humi, UV, dust, rain1, rain2, voltage1, voltage2, current1, current2)"
                    " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

     query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
     query->addBindValue(temp);
     query->addBindValue(humi);
     query->addBindValue(UV);
     query->addBindValue(dust);
     query->addBindValue(rain1);
     query->addBindValue(rain2);

     query->addBindValue(voltage1);
     query->addBindValue(voltage2);
     query->addBindValue(current1);
     query->addBindValue(current2);

     if(query->exec())
         val = true;

     return val;
}


bool etc_table::insert_act_table(QDateTime date, int pump, int heating)
{
    bool val = false;

    query->prepare("insert into etc_act(date, pump, heating)"
                   " VALUES(?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(pump);
    query->addBindValue(heating);


    if(query->exec())
        val = true;

    return val;
}



QVector<etc *> *etc_table::get_records()
{
    QVector<etc *> *list = new QVector<etc *>;
    query->prepare("select * from etc_view");
    query->exec();

    while(query->next())
    {
        etc * temp = new etc();

        QString str = query->value(0).toString();
        QDateTime dateTime = QDateTime::fromString(str, "yyyy-MM-ddThh:mm:ss");

        temp->set_datetime(dateTime);
        temp->set_temperature(query->value(1).toInt());
        temp->set_humidity(query->value(2).toInt());

        temp->set_LED_R(query->value(3).toInt());
        temp->set_LED_G(query->value(4).toInt());
        temp->set_LED_B(query->value(5).toInt());
        temp->set_LED_OnOff_state(query->value(6).toInt());

        temp->set_door(query->value(7).toInt());

        list->append(temp);
    }
    return list;
}



QWidget* etc_table::show_table()
{
    if(this->table == NULL)
    {
        comboBox = new QComboBox();
        comboBox->addItem("All");
        comboBox->addItem("date");
        comboBox->addItem("temp");
        comboBox->addItem("humi");
        comboBox->addItem("UV");
        comboBox->addItem("dust");
        comboBox->addItem("rain1");
        comboBox->addItem("rain2");
        comboBox->addItem("voltage1");
        comboBox->addItem("voltage2");
        comboBox->addItem("current1");
        comboBox->addItem("current2");
        comboBox->addItem("pump");
        comboBox->addItem("heating");


        lineEdit = new QLineEdit();
        pushButton = new QPushButton("검색");
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(comboBox);
        hLayout->addWidget(lineEdit);
        hLayout->addWidget(pushButton);


        tab_model = new QStandardItemModel(0, 13, NULL);
        QTableView * table_view = new QTableView;
        table_view->setModel(tab_model);

        tab_model->setHeaderData(0, Qt::Horizontal, "date");
        tab_model->setHeaderData(1, Qt::Horizontal, "temp");
        tab_model->setHeaderData(2, Qt::Horizontal, "humi");

        tab_model->setHeaderData(3, Qt::Horizontal, "UV");
        tab_model->setHeaderData(4, Qt::Horizontal, "dust");
        tab_model->setHeaderData(5, Qt::Horizontal, "rain1");
        tab_model->setHeaderData(6, Qt::Horizontal, "rain2");

        tab_model->setHeaderData(7, Qt::Horizontal, "voltage1");
        tab_model->setHeaderData(8, Qt::Horizontal, "voltage2");
        tab_model->setHeaderData(9, Qt::Horizontal, "current1");
        tab_model->setHeaderData(10, Qt::Horizontal, "current2");

        tab_model->setHeaderData(11, Qt::Horizontal, "pump");
        tab_model->setHeaderData(12, Qt::Horizontal, "heating");


        table_view->setColumnWidth(0,160);
        table_view->setColumnWidth(1,60);
        table_view->setColumnWidth(2,60);
        table_view->setColumnWidth(3,60);
        table_view->setColumnWidth(4,60);
        table_view->setColumnWidth(5,60);
        table_view->setColumnWidth(6,60);
        table_view->setColumnWidth(7,90);
        table_view->setColumnWidth(8,90);
        table_view->setColumnWidth(9,90);
        table_view->setColumnWidth(10,90);
        table_view->setColumnWidth(11,60);
        table_view->setColumnWidth(12,60);

        QItemSelectionModel* tab_selectionModel = new QItemSelectionModel(tab_model);
        table_view->setSelectionModel(tab_selectionModel);
        table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

        this->table = new QWidget();
        this->table->resize(1020,200);
        this->table->setWindowTitle("etc_table");

        QVBoxLayout *mainLayout = new QVBoxLayout(this->table);
        mainLayout->setSpacing(1);
        mainLayout->setMargin(1);
        mainLayout->addLayout(hLayout);
        mainLayout->addWidget(table_view);

        connect(pushButton, SIGNAL(clicked()), this, SLOT(button_slot()));
    }

    return this->table;
}




void etc_table::button_slot()
{
    QString text;

    if(comboBox->currentIndex() == 0){ // 전체검색
        text = QString("select * from etc_view");
    }else if(comboBox->currentIndex() == 1){ // 시간검색
        text = QString("select * from etc_view where date like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 2){ // 온도검색
        text = QString("select * from etc_view where temp like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 3){ // 습도검색
        text = QString("select * from etc_view where humi like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 4){ // UV검색
        text = QString("select * from etc_view where UV like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 5){ // dust검색
        text = QString("select * from etc_view where dust like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 6){ // rain1검색
        text = QString("select * from etc_view where rain1 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 7){ // rain2검색
        text = QString("select * from etc_view where rain2 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 8){ // voltage1검색
        text = QString("select * from etc_view where voltage1 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 9){ // voltage2검색
        text = QString("select * from etc_view where voltage2 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 10){ // current1검
        text = QString("select * from etc_view where current1 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 11){ // current2검색
        text = QString("select * from etc_view where current2 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 12){ // pump검색
        text = QString("select * from etc_view where pump like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 13){ // heating검색
        text = QString("select * from etc_view where heating like '%%1%'").arg(lineEdit->text());
    }



    int size = tab_model->rowCount();
    tab_model->removeRows(0, size, QModelIndex());
    query->exec(text);

    int i=0;

    while(query->next())
    {

        QString date                = query->value(0).toString();
        QString temp                = query->value(1).toString();
        QString humi                = query->value(2).toString();

        QString UV                  = query->value(3).toString();
        QString dust                = query->value(4).toString();
        QString rain1               = query->value(5).toString();
        QString rain2               = query->value(6).toString();

        QString voltage1            = query->value(7).toString();
        QString voltage2            = query->value(8).toString();
        QString current1            = query->value(9).toString();
        QString current2            = query->value(10).toString();

        QString pump                = query->value(11).toString();
        QString heating             = query->value(12).toString();



        tab_model->insertRows(i, 1, QModelIndex());
        tab_model->setData(tab_model->index(i, 0, QModelIndex()), date);
        tab_model->setData(tab_model->index(i, 1, QModelIndex()), temp);
        tab_model->setData(tab_model->index(i, 2, QModelIndex()), humi);

        tab_model->setData(tab_model->index(i, 3, QModelIndex()), UV);
        tab_model->setData(tab_model->index(i, 4, QModelIndex()), dust);
        tab_model->setData(tab_model->index(i, 5, QModelIndex()), rain1);
        tab_model->setData(tab_model->index(i, 6, QModelIndex()), rain2);

        tab_model->setData(tab_model->index(i, 7, QModelIndex()), voltage1);
        tab_model->setData(tab_model->index(i, 8, QModelIndex()), voltage2);
        tab_model->setData(tab_model->index(i, 9, QModelIndex()), current1);
        tab_model->setData(tab_model->index(i, 10, QModelIndex()), current2);

        tab_model->setData(tab_model->index(i, 11, QModelIndex()), pump);
        tab_model->setData(tab_model->index(i, 12, QModelIndex()), heating);

        i++;
    }
}


QPushButton* etc_table::get_pushbutton()
{
    return this->pushButton;
}
