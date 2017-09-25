#include "kitchen_table.h"

kitchen_table::kitchen_table(QSqlQuery *query)
{
    this->query = query;
    this->table = NULL;
}

void kitchen_table::create_table_inDB()
{
    query->exec("create table if not exists kitchen_sensor(date datetime not null primary key,"
                " temp int not null, humi int not null, co int ,LPG int, state int not null)");

    query->exec("create table if not exists kitchen_act(date datetime not null primary key,"
                " R int not null, G int not null, B int not null, LED_ON_OFF int not null, window1 int not null, window2 int not null, ventilating_fan int not null)");

    query->exec("create or replace view kitchen_view as select a.date, a.temp, a.humi,"
                " a.co, a.LPG, a.state, b.R, b.G, b.B, b.LED_ON_OFF, b.window1, b.window2, b.ventilating_fan from kitchen_sensor a, kitchen_act b where a.date = b.date");
}


bool kitchen_table::insert_view(kitchen* room)
{
    query->exec("START TRANSACTION");

    bool x = insert_sensor_table(room->get_datetime(),room->get_temperature(),room->get_humidity(),
                                 room->get_Co2(),room->get_LPG(),room->get_Flmae());
    bool y = insert_act_table(room->get_datetime(),room->get_LED_R(),room->get_LED_G(),room->get_LED_B(),
                              room->get_LED_OnOff_state(),room->get_window1(),room->get_window2(),room->get_ventilating_fan());

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



bool kitchen_table::insert_sensor_table(QDateTime date, int temp, int humi, int co, int LPG, int state)
{
    bool val = false;

    query->prepare("insert into kitchen_sensor(date, temp, humi, co, LPG, state)"
                   " VALUES(?, ?, ?, ?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(temp);
    query->addBindValue(humi);
    query->addBindValue(co);
    query->addBindValue(LPG);
    query->addBindValue(state);

    if(query->exec()) val = true;

    return val;
}


bool kitchen_table::insert_act_table(QDateTime date, int R, int G, int B, int LED_ON_OFF, int window1, int window2, int ventilating_fan)
{
    bool val = false;

    query->prepare("insert into kitchen_act(date, R, G, B, LED_ON_OFF, window1, window2, ventilating_fan)"
                   " VALUES(?, ?, ?, ?, ?, ?, ?, ?)");

    query->addBindValue(date.toString("yyyy-MM-dd hh:mm:ss"));
    query->addBindValue(R);
    query->addBindValue(G);
    query->addBindValue(B);
    query->addBindValue(LED_ON_OFF);
    query->addBindValue(window1);
    query->addBindValue(window2);
    query->addBindValue(ventilating_fan);

    if(query->exec()) val = true;

    return val;
}


QVector<kitchen *> *kitchen_table::get_records()
{
    QVector<kitchen *> *list = new QVector<kitchen *>;
    query->prepare("select * from kitchen_view");
    query->exec();

    while(query->next())
    {
        kitchen * temp = new kitchen();

        QString str = query->value(0).toString();
        QDateTime dateTime = QDateTime::fromString(str, "yyyy-MM-ddThh:mm:ss");

        temp->set_datetime(dateTime);
        temp->set_temperature(query->value(1).toInt());
        temp->set_humidity(query->value(2).toInt());
        temp->set_Co2(query->value(3).toInt());
        temp->set_LPG(query->value(4).toInt());
        temp->set_Flame(query->value(5).toInt());

        temp->set_LED_R(query->value(6).toInt());
        temp->set_LED_G(query->value(7).toInt());
        temp->set_LED_B(query->value(8).toInt());
        temp->set_LED_OnOff_state(query->value(9).toInt());

        temp->set_window1(query->value(10).toInt());
        temp->set_window2(query->value(11).toInt());
        temp->set_ventilating_fan(query->value(12).toInt());

        list->append(temp);
    }
    return list;
}



QWidget* kitchen_table::show_table()
{
    if(this->table == NULL)
    {
    comboBox = new QComboBox();
    comboBox->addItem("All");
    comboBox->addItem("date");
    comboBox->addItem("temp");
    comboBox->addItem("humi");
    comboBox->addItem("co2");
    comboBox->addItem("LPG");
    comboBox->addItem("state");

    comboBox->addItem("R");
    comboBox->addItem("G");
    comboBox->addItem("B");
    comboBox->addItem("LED_ON_OFF");

    comboBox->addItem("window1");
    comboBox->addItem("window2");
    comboBox->addItem("ventilating_fan");

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
    tab_model->setHeaderData(3, Qt::Horizontal, "co2");
    tab_model->setHeaderData(4, Qt::Horizontal, "LPG");
    tab_model->setHeaderData(5, Qt::Horizontal, "state");

    tab_model->setHeaderData(6, Qt::Horizontal, "R");
    tab_model->setHeaderData(7, Qt::Horizontal, "G");
    tab_model->setHeaderData(8, Qt::Horizontal, "B");
    tab_model->setHeaderData(9, Qt::Horizontal, "LED_ON");

    tab_model->setHeaderData(10, Qt::Horizontal, "window1");
    tab_model->setHeaderData(11, Qt::Horizontal, "window2");
    tab_model->setHeaderData(12, Qt::Horizontal, "v_fan");


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
    table_view->setColumnWidth(10,80);
    table_view->setColumnWidth(11,80);
    table_view->setColumnWidth(12,80);



    QItemSelectionModel* tab_selectionModel = new QItemSelectionModel(tab_model);
    table_view->setSelectionModel(tab_selectionModel);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

   this->table = new QWidget();
   this->table->resize(950,200);
   this->table->setWindowTitle("kitchen_table");

    QVBoxLayout *mainLayout = new QVBoxLayout(this->table);
    mainLayout->setSpacing(1);
    mainLayout->setMargin(1);
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(table_view);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(button_slot()));
    }

    return this->table;
}




void kitchen_table::button_slot()
{
    QString text;

    if(comboBox->currentIndex() == 0){ // 전체검색
        text = QString("select * from kitchen_view");
    }else if(comboBox->currentIndex() == 1){ // 시간검색
        text = QString("select * from kitchen_view where date like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 2){ // 온도검색
        text = QString("select * from kitchen_view where temp like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 3){ // 습도검색
        text = QString("select * from kitchen_view where humi like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 4){ // CO검색
        text = QString("select * from kitchen_view where CO like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 5){ // LPG검색
        text = QString("select * from kitchen_view where LPG like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 6){ // state검색
        text = QString("select * from kitchen_view where state like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 7){ // R검색
        text = QString("select * from kitchen_view where R like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 8){ // G검색
        text = QString("select * from kitchen_view where G like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 9){ // B검색
        text = QString("select * from kitchen_view where B like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 10){ // LED_ON_OFF검색
        text = QString("select * from kitchen_view where LED_ON_OFF like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 11){ // windo1검색
        text = QString("select * from kitchen_view where window1 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 12){ // window2검색
        text = QString("select * from kitchen_view where window2 like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 13){ // ventilating_fan검색
        text = QString("select * from kitchen_view where ventilating_fan like '%%1%'").arg(lineEdit->text());
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
        QString co2                 = query->value(3).toString();
        QString LPG                 = query->value(4).toString();
        QString state               = query->value(5).toString();

        QString R                   = query->value(6).toString();
        QString G                   = query->value(7).toString();
        QString B                   = query->value(8).toString();
        QString LED_ON_OFF          = query->value(9).toString();

        QString window1             = query->value(10).toString();
        QString window2             = query->value(11).toString();
        QString ventilating_fan		= query->value(12).toString();

        tab_model->insertRows(i, 1, QModelIndex());
        tab_model->setData(tab_model->index(i, 0, QModelIndex()), date);
        tab_model->setData(tab_model->index(i, 1, QModelIndex()), temp);
        tab_model->setData(tab_model->index(i, 2, QModelIndex()), humi);
        tab_model->setData(tab_model->index(i, 3, QModelIndex()), co2);
        tab_model->setData(tab_model->index(i, 4, QModelIndex()), LPG);
        tab_model->setData(tab_model->index(i, 5, QModelIndex()), state);

        tab_model->setData(tab_model->index(i, 6, QModelIndex()), R);
        tab_model->setData(tab_model->index(i, 7, QModelIndex()), G);
        tab_model->setData(tab_model->index(i, 8, QModelIndex()), B);
        tab_model->setData(tab_model->index(i, 9, QModelIndex()), LED_ON_OFF);

        tab_model->setData(tab_model->index(i, 10, QModelIndex()), window1);
        tab_model->setData(tab_model->index(i, 11, QModelIndex()), window2);
        tab_model->setData(tab_model->index(i, 12, QModelIndex()), ventilating_fan);

        i++;
    }
}

QPushButton* kitchen_table::get_pushbutton()
{
    return this->pushButton;
}
