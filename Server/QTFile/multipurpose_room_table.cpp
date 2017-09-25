#include "multipurpose_room_table.h"

multi_room_table::multi_room_table(QSqlQuery *query)
{
    this->query = query;
    this->table = NULL;
}

void multi_room_table::create_table_inDB()
{
    query->exec("create table if not exists multi_room_sensor(date datetime not null primary key,"
                " temp int not null, humi int not null)");

    query->exec("create table if not exists multi_room_act(date datetime not null primary key,"
                " R int not null, G int not null, B int not null, LED_ON_OFF int not null, window1 int not null, window2 int not null, door int not null)");

    query->exec("create or replace view multi_room_view as select a.date, a.temp, a.humi,"
                " b.R, b.G, b.B, b.LED_ON_OFF, b.window1, b.window2, b.door from multi_room_sensor a, multi_room_act b where a.date = b.date");
}


bool multi_room_table::insert_view(multipurpose_room* room)
    {
    query->exec("START TRANSACTION");

    bool x = insert_sensor_table(room->get_datetime(),room->get_temperature(),room->get_humidity());
    bool y = insert_act_table(room->get_datetime(),room->get_LED_R(),
                              room->get_LED_G(),room->get_LED_B(),room->get_LED_OnOff_state(),
                              room->get_window1(),room->get_window2(),room->get_door());

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


bool multi_room_table::insert_sensor_table(QDateTime date, int temp, int humi)
{
    bool val = false;

    query->prepare("insert into multi_room_sensor(date, temp, humi)"
                   " VALUES(?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(temp);
    query->addBindValue(humi);

    if(query->exec()) val = true;

    return val;
}


bool multi_room_table::insert_act_table(QDateTime date, int R, int G, int B, int LED_ON_OFF, int window1, int window2, int door)
{
    bool val = false;

    query->prepare("insert into multi_room_act(date, R, G, B, LED_ON_OFF, window1, window2, door)"
                   " VALUES(?, ?, ?, ?, ?, ?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(R);
    query->addBindValue(G);
    query->addBindValue(B);
    query->addBindValue(LED_ON_OFF);
    query->addBindValue(window1);
    query->addBindValue(window2);
    query->addBindValue(door);

    if(query->exec()) val = true;

    return val;
}



QVector<multipurpose_room *> *multi_room_table::get_records()
{
    QVector<multipurpose_room *> *list = new QVector<multipurpose_room *>;
    query->prepare("select * from multi_room_view");
    query->exec();

    while(query->next())
    {
        multipurpose_room * temp = new multipurpose_room();

        QString str = query->value(0).toString();
        QDateTime dateTime = QDateTime::fromString(str, "yyyy-MM-ddThh:mm:ss");

        temp->set_datetime(dateTime);
        temp->set_temperature(query->value(1).toInt());
        temp->set_humidity(query->value(2).toInt());

        temp->set_LED_R(query->value(3).toInt());
        temp->set_LED_G(query->value(4).toInt());
        temp->set_LED_B(query->value(5).toInt());
        temp->set_LED_OnOff_state(query->value(6).toInt());

        temp->set_window1(query->value(7).toInt());
        temp->set_window2(query->value(8).toInt());
        temp->set_door(query->value(9).toInt());

        list->append(temp);
    }
    return list;
}



QWidget* multi_room_table::show_table()
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

        comboBox->addItem("window1");
        comboBox->addItem("window2");
        comboBox->addItem("door");

        lineEdit = new QLineEdit();
        pushButton = new QPushButton("검색");
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(comboBox);
        hLayout->addWidget(lineEdit);
        hLayout->addWidget(pushButton);


        tab_model = new QStandardItemModel(0, 10, NULL);
        QTableView * table_view = new QTableView;
        table_view->setModel(tab_model);

        tab_model->setHeaderData(0, Qt::Horizontal, "date");
        tab_model->setHeaderData(1, Qt::Horizontal, "temp");
        tab_model->setHeaderData(2, Qt::Horizontal, "humi");

        tab_model->setHeaderData(3, Qt::Horizontal, "R");
        tab_model->setHeaderData(4, Qt::Horizontal, "G");
        tab_model->setHeaderData(5, Qt::Horizontal, "B");
        tab_model->setHeaderData(6, Qt::Horizontal, "LED_ON");

        tab_model->setHeaderData(7, Qt::Horizontal, "window1");
        tab_model->setHeaderData(8, Qt::Horizontal, "window2");
        tab_model->setHeaderData(9, Qt::Horizontal, "door");


        table_view->setColumnWidth(0,160);
        table_view->setColumnWidth(1,60);
        table_view->setColumnWidth(2,60);
        table_view->setColumnWidth(3,60);
        table_view->setColumnWidth(4,60);
        table_view->setColumnWidth(5,60);
        table_view->setColumnWidth(6,90);
        table_view->setColumnWidth(7,90);
        table_view->setColumnWidth(8,90);
        table_view->setColumnWidth(9,90);


        QItemSelectionModel* tab_selectionModel = new QItemSelectionModel(tab_model);
        table_view->setSelectionModel(tab_selectionModel);
        table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

        this->table = new QWidget();
        this->table->resize(840,200);
        this->table->setWindowTitle("multi_room_table");

        QVBoxLayout *mainLayout = new QVBoxLayout(this->table);
        mainLayout->setSpacing(1);
        mainLayout->setMargin(1);
        mainLayout->addLayout(hLayout);
        mainLayout->addWidget(table_view);

        connect(pushButton, SIGNAL(clicked()), this, SLOT(button_slot()));
    }

    return this->table;
}




void multi_room_table::button_slot()
{
    QString text;

    if(comboBox->currentIndex() == 0){ // 전체검색
        text = QString("select * from multi_room_view");
    }else if(comboBox->currentIndex() == 1){ // 시간검색
        text = QString("select * from multi_room_view where date like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 2){ // 온도검색
        text = QString("select * from multi_room_view where temp like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 3){ // 습도검색
        text = QString("select * from multi_room_view where humi like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 4){ // R검색
        text = QString("select * from multi_room_view where R like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 5){ // G검색
        text = QString("select * from multi_room_view where G like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 6){ // B검색
        text = QString("select * from multi_room_view where B like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 7){ // LED_ON_OFF검색
        text = QString("select * from multi_room_view where LED_ON_OFF like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 8){ // window1검색
        text = QString("select * from multi_room_view where window1 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 9){ // window2검색
        text = QString("select * from multi_room_view where window2 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 10){ // door검색
        text = QString("select * from multi_room_view where door like '%%1%'").arg(lineEdit->text());
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

        QString R                   = query->value(3).toString();
        QString G                   = query->value(4).toString();
        QString B                   = query->value(5).toString();
        QString LED_ON_OFF          = query->value(6).toString();

        QString window1             = query->value(7).toString();
        QString window2             = query->value(8).toString();
        QString door                = query->value(9).toString();

        tab_model->insertRows(i, 1, QModelIndex());
        tab_model->setData(tab_model->index(i, 0, QModelIndex()), date);
        tab_model->setData(tab_model->index(i, 1, QModelIndex()), temp);
        tab_model->setData(tab_model->index(i, 2, QModelIndex()), humi);

        tab_model->setData(tab_model->index(i, 3, QModelIndex()), R);
        tab_model->setData(tab_model->index(i, 4, QModelIndex()), G);
        tab_model->setData(tab_model->index(i, 5, QModelIndex()), B);
        tab_model->setData(tab_model->index(i, 6, QModelIndex()), LED_ON_OFF);

        tab_model->setData(tab_model->index(i, 7, QModelIndex()), window1);
        tab_model->setData(tab_model->index(i, 8, QModelIndex()), window2);
        tab_model->setData(tab_model->index(i, 9, QModelIndex()), door);

        i++;
    }
}


QPushButton* multi_room_table::get_pushbutton()
{
    return this->pushButton;
}
