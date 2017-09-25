#include "living_room_table.h"


living_room_table::living_room_table(QSqlQuery *query)
{
    this->query = query;
    this->table = NULL;
}

void living_room_table::create_table_inDB()
{
    query->exec("create table if not exists living_room_sensor(date datetime not null primary key,"
                " temp int not null, humi int not null, co int , state int not null)");

    query->exec("create table if not exists living_room_act(date datetime not null primary key,"
                " R int not null, G int not null, B int not null, LED_ON_OFF int not null, window1 int not null, window2 int not null)");

    query->exec("create or replace view living_room_view as select a.date, a.temp, a.humi,"
                " a.co, a.state, b.R, b.G, b.B, b.LED_ON_OFF, b.window1, b.window2 from living_room_sensor a, living_room_act b where a.date = b.date");
}


bool living_room_table::insert_view(living_room * room)
{
    query->exec("START TRANSACTION");

    bool x = insert_sensor_table(room->get_datetime(),room->get_temperature(),room->get_humidity(),
                                 room->get_Co2(),room->get_Flmae());
    bool y = insert_act_table(room->get_datetime(),room->get_LED_R(),room->get_LED_G(),room->get_LED_B(),
                              room->get_LED_OnOff_state(),room->get_window1(),room->get_window2());

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


bool living_room_table::insert_sensor_table(QDateTime date, int temp, int humi, int co, int state)
{
    bool val = false;

    query->prepare("insert into living_room_sensor(date, temp, humi, co, state)"
                   " VALUES(?, ?, ?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(temp);
    query->addBindValue(humi);
    query->addBindValue(co);
    query->addBindValue(state);

    if(query->exec()) val = true;

    return val;
}


bool living_room_table::insert_act_table(QDateTime date, int R, int G, int B, int LED_ON_OFF, int window1, int window2)
{
    bool val = false;

    query->prepare("insert into living_room_act(date, R, G, B, LED_ON_OFF, window1, window2)"
                   " VALUES(?, ?, ?, ?, ?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(R);
    query->addBindValue(G);
    query->addBindValue(B);
    query->addBindValue(LED_ON_OFF);
    query->addBindValue(window1);
    query->addBindValue(window2);

    if(query->exec()) val = true;

    return val;
}


QVector<living_room *> *living_room_table::get_records()
{
    QVector<living_room *> *list = new QVector<living_room *>;
    query->prepare("select * from living_room_view");
    query->exec();

    while(query->next())
    {
        living_room * temp = new living_room();

        QString str = query->value(0).toString();
        QDateTime dateTime = QDateTime::fromString(str, "yyyy-MM-ddThh:mm:ss");
        temp->set_datetime(dateTime);
        temp->set_temperature(query->value(1).toInt());
        temp->set_humidity(query->value(2).toInt());
        temp->set_Co2(query->value(3).toInt());
        temp->set_Flame(query->value(4).toInt());

        temp->set_LED_R(query->value(5).toInt());
        temp->set_LED_G(query->value(6).toInt());
        temp->set_LED_B(query->value(7).toInt());
        temp->set_LED_OnOff_state(query->value(8).toInt());

        temp->set_window1(query->value(9).toInt());
        temp->set_window2(query->value(10).toInt());

        list->append(temp);
    }
    return list;
}



QWidget* living_room_table::show_table()
{
    if(table == NULL)
    {
    comboBox = new QComboBox();
    comboBox->addItem("All");
    comboBox->addItem("date");
    comboBox->addItem("temp");
    comboBox->addItem("humi");
    comboBox->addItem("co2");
    comboBox->addItem("state");
    comboBox->addItem("R");
    comboBox->addItem("G");
    comboBox->addItem("B");
    comboBox->addItem("LED_ON_OFF");
    comboBox->addItem("window1");
    comboBox->addItem("window2");

    lineEdit = new QLineEdit();
    pushButton = new QPushButton("검색");
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(comboBox);
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(pushButton);

    tab_model = new QStandardItemModel(0, 11, NULL);
    QTableView * table_view = new QTableView;
    table_view->setModel(tab_model);

    tab_model->setHeaderData(0, Qt::Horizontal, "date");
    tab_model->setHeaderData(1, Qt::Horizontal, "temp");
    tab_model->setHeaderData(2, Qt::Horizontal, "humi");
    tab_model->setHeaderData(3, Qt::Horizontal, "co2");
    tab_model->setHeaderData(4, Qt::Horizontal, "state");
    tab_model->setHeaderData(5, Qt::Horizontal, "R");
    tab_model->setHeaderData(6, Qt::Horizontal, "G");
    tab_model->setHeaderData(7, Qt::Horizontal, "B");
    tab_model->setHeaderData(8, Qt::Horizontal, "LED_ON");
    tab_model->setHeaderData(9, Qt::Horizontal, "window1");
    tab_model->setHeaderData(10, Qt::Horizontal, "window2");


    table_view->setColumnWidth(0,160);
    table_view->setColumnWidth(1,60);
    table_view->setColumnWidth(2,60);
    table_view->setColumnWidth(3,60);
    table_view->setColumnWidth(4,60);
    table_view->setColumnWidth(5,60);
    table_view->setColumnWidth(6,60);
    table_view->setColumnWidth(7,60);
    table_view->setColumnWidth(8,80);
    table_view->setColumnWidth(9,80);
    table_view->setColumnWidth(10,80);


    QItemSelectionModel* tab_selectionModel = new QItemSelectionModel(tab_model);
    table_view->setSelectionModel(tab_selectionModel);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

   this->table = new QWidget();
   this->table->resize(850,200);
   this->table->setWindowTitle("living_room_table");

    QVBoxLayout *mainLayout = new QVBoxLayout(this->table);
    mainLayout->setSpacing(1);
    mainLayout->setMargin(1);
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(table_view);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(button_slot()));
    }

    return this->table;
}




void living_room_table::button_slot()
{
    QString text;

    if(comboBox->currentIndex() == 0){ // 전체검색
        text = QString("select * from living_room_view");
    }else if(comboBox->currentIndex() == 1){ // 시간검색
        text = QString("select * from living_room_view where date like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 2){ // 온도검색
        text = QString("select * from living_room_view where temp like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 3){ // 습도검색
        text = QString("select * from living_room_view where humi like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 4){ // Co검색
        text = QString("select * from living_room_view where CO like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 5){ // state검색
        text = QString("select * from living_room_view where state like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 6){ // R검색
        text = QString("select * from living_room_view where R like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 7){ // G검색
        text = QString("select * from living_room_view where G like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 8){ // B검색
        text = QString("select * from living_room_view where B like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 9){ // LED_ON_OFF검색
        text = QString("select * from living_room_view where LED_ON_OFF like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 10){ // window1검색
        text = QString("select * from living_room_view where window1 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 11){ // window2검색
        text = QString("select * from living_room_view where window2 like '%%1%'").arg(lineEdit->text());
    }

    int size = tab_model->rowCount();
    tab_model->removeRows(0, size, QModelIndex());
    query->exec(text);

    int i=0;

    while(query->next())
    {

        QString date 		= query->value(0).toString();
        QString temp        = query->value(1).toString();
        QString humi 		= query->value(2).toString();
        QString co2         = query->value(3).toString();
        QString state 		= query->value(4).toString();
        QString R           = query->value(5).toString();
        QString G           = query->value(6).toString();
        QString B           = query->value(7).toString();
        QString LED_ON_OFF 	= query->value(8).toString();
        QString window1     = query->value(9).toString();
        QString window2		= query->value(10).toString();

        tab_model->insertRows(i, 1, QModelIndex());
        tab_model->setData(tab_model->index(i, 0, QModelIndex()), date);
        tab_model->setData(tab_model->index(i, 1, QModelIndex()), temp);
        tab_model->setData(tab_model->index(i, 2, QModelIndex()), humi);
        tab_model->setData(tab_model->index(i, 3, QModelIndex()), co2);
        tab_model->setData(tab_model->index(i, 4, QModelIndex()), state);

        tab_model->setData(tab_model->index(i, 5, QModelIndex()), R);
        tab_model->setData(tab_model->index(i, 6, QModelIndex()), G);
        tab_model->setData(tab_model->index(i, 7, QModelIndex()), B);
        tab_model->setData(tab_model->index(i, 8, QModelIndex()), LED_ON_OFF);

        tab_model->setData(tab_model->index(i, 9, QModelIndex()), window1);
        tab_model->setData(tab_model->index(i, 10, QModelIndex()), window2);

        i++;
    }
}


QPushButton* living_room_table::get_pushbutton()
{
    return this->pushButton;
}
