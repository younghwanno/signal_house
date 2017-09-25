#include "bath_room_table.h"

bath_room_table::bath_room_table(QSqlQuery *query)
{
    this->query = query;
    this->table = NULL;
}

void bath_room_table::create_table_inDB()
{
    query->exec("create table if not exists bath_room_sensor(date datetime not null primary key,"
                " temp int not null, humi int not null)");

    query->exec("create table if not exists bath_room_act(date datetime not null primary key,"
                " R int not null, G int not null, B int not null, LED_ON_OFF int not null, door int not null, ventilating_fan int not null)");

    query->exec("create or replace view bath_room_view as select a.date, a.temp, a.humi,"
                " b.R, b.G, b.B, b.LED_ON_OFF, b.door, b.ventilating_fan from bath_room_sensor a, bath_room_act b where a.date = b.date");
}


bool bath_room_table::insert_view(bath_room *room)
{
query->exec("START TRANSACTION");

bool x = insert_sensor_table(room->get_datetime(),room->get_temperature(),room->get_humidity());
bool y = insert_act_table(room->get_datetime(),room->get_LED_R(),room->get_LED_G(),room->get_LED_B(),
                          room->get_LED_OnOff_state(),room->get_door(),room->get_ventilating_fan());

if(x && y)
{query->exec("COMMIT");
qDebug() << "commit";
}
else
{query->exec("ROLLBACK");
qDebug() << "rollback";
}

return x && y;
}


bool bath_room_table::insert_sensor_table(QDateTime date, int temp, int humi)
{
    bool val = false;

    query->prepare("insert into bath_room_sensor(date, temp, humi)"
                   " VALUES(?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(temp);
    query->addBindValue(humi);

    if(query->exec()) val = true;

    return val;
}


bool bath_room_table::insert_act_table(QDateTime date, int R, int G, int B, int LED_ON_OFF, int door, int ventilating_fan)
{
    bool val = false;

    query->prepare("insert into bath_room_act(date, R, G, B, LED_ON_OFF, door, ventilating_fan)"
                   " VALUES(?, ?, ?, ?, ?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(R);
    query->addBindValue(G);
    query->addBindValue(B);
    query->addBindValue(LED_ON_OFF);
    query->addBindValue(door);
    query->addBindValue(ventilating_fan);

    if(query->exec()) val = true;

    return val;
}



QVector<bath_room *> *bath_room_table::get_records()
{
    QVector<bath_room *> *list = new QVector<bath_room *>;
    query->prepare("select * from bath_room_view");
    query->exec();

    while(query->next())
    {
        bath_room * temp = new bath_room();

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
        temp->set_ventilating_fan(query->value(8).toInt());

        list->append(temp);
    }
    return list;
}



QWidget* bath_room_table::show_table()
{
    if(this->table == NULL)
    {
    comboBox = new QComboBox();
    comboBox->addItem("All");
    comboBox->addItem("date");
    comboBox->addItem("temp");
    comboBox->addItem("humi");

    comboBox->addItem("R");
    comboBox->addItem("G");
    comboBox->addItem("B");
    comboBox->addItem("LED_ON_OFF");

    comboBox->addItem("door");
    comboBox->addItem("ventilating_fan");

    lineEdit = new QLineEdit();
    pushButton = new QPushButton("검색");
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(comboBox);
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(pushButton);


    tab_model = new QStandardItemModel(0, 9, NULL);
    QTableView * table_view = new QTableView;
    table_view->setModel(tab_model);

    tab_model->setHeaderData(0, Qt::Horizontal, "date");
    tab_model->setHeaderData(1, Qt::Horizontal, "temp");
    tab_model->setHeaderData(2, Qt::Horizontal, "humi");

    tab_model->setHeaderData(3, Qt::Horizontal, "R");
    tab_model->setHeaderData(4, Qt::Horizontal, "G");
    tab_model->setHeaderData(5, Qt::Horizontal, "B");
    tab_model->setHeaderData(6, Qt::Horizontal, "LED_ON");

    tab_model->setHeaderData(7, Qt::Horizontal, "door");
    tab_model->setHeaderData(8, Qt::Horizontal, "v_fan");

    table_view->setColumnWidth(0,160);
    table_view->setColumnWidth(1,60);
    table_view->setColumnWidth(2,60);
    table_view->setColumnWidth(3,60);
    table_view->setColumnWidth(4,60);
    table_view->setColumnWidth(5,60);
    table_view->setColumnWidth(6,60);
    table_view->setColumnWidth(7,60);
    table_view->setColumnWidth(8,60);
    table_view->setColumnWidth(9,60);
    table_view->setColumnWidth(10,60);


    QItemSelectionModel* tab_selectionModel = new QItemSelectionModel(tab_model);
    table_view->setSelectionModel(tab_selectionModel);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

   this->table = new QWidget();
   this->table->resize(670,200);
   this->table->setWindowTitle("bath_room_table");

    QVBoxLayout *mainLayout = new QVBoxLayout(this->table);
    mainLayout->setSpacing(1);
    mainLayout->setMargin(1);
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(table_view);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(button_slot()));
    }

    return this->table;
}




void bath_room_table::button_slot()
{
    QString text;

    if(comboBox->currentIndex() == 0){ // 전체검색
        text = QString("select * from bath_room_view");
    }else if(comboBox->currentIndex() == 1){ // 시간검색
        text = QString("select * from bath_room_view where date like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 2){ // 온도검색
        text = QString("select * from bath_room_view where temp like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 3){ // 습도검색
        text = QString("select * from bath_room_view where humi like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 4){ // R검색
        text = QString("select * from bath_room_view where R like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 5){ // G검색
        text = QString("select * from bath_room_view where G like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 6){ // B검색
        text = QString("select * from bath_room_view where B like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 7){ // LED_ON_OFF검색
        text = QString("select * from bath_room_view where LED_ON_OFF like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 8){ // door검색
        text = QString("select * from bath_room_view where door like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 9){ // ventilating_fan검색
        text = QString("select * from bath_room_view where ventilating_fan like '%%1%'").arg(lineEdit->text());
    }

    int size = tab_model->rowCount();
    tab_model->removeRows(0, size, QModelIndex());
    query->exec(text);

    int i=0;
    while(query->next()){

        QString date                = query->value(0).toString();
        QString temp                = query->value(1).toString();
        QString humi                = query->value(2).toString();

        QString R                   = query->value(3).toString();
        QString G                   = query->value(4).toString();
        QString B                   = query->value(5).toString();
        QString LED_ON_OFF          = query->value(6).toString();

        QString door                = query->value(7).toString();
        QString ventilating_fan     = query->value(8).toString();

        tab_model->insertRows(i, 1, QModelIndex());
        tab_model->setData(tab_model->index(i, 0, QModelIndex()), date);
        tab_model->setData(tab_model->index(i, 1, QModelIndex()), temp);
        tab_model->setData(tab_model->index(i, 2, QModelIndex()), humi);

        tab_model->setData(tab_model->index(i, 3, QModelIndex()), R);
        tab_model->setData(tab_model->index(i, 4, QModelIndex()), G);
        tab_model->setData(tab_model->index(i, 5, QModelIndex()), B);
        tab_model->setData(tab_model->index(i, 6, QModelIndex()), LED_ON_OFF);

        tab_model->setData(tab_model->index(i, 7, QModelIndex()), door);
        tab_model->setData(tab_model->index(i, 8, QModelIndex()), ventilating_fan);

        i++;
    }

    //QString temp = QString("%1%2").arg(query.size()).arg("건의 레코드가 검색되었습니다.");
}


QPushButton* bath_room_table::get_pushbutton()
{
    return this->pushButton;
}
