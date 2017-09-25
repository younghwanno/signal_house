#include "login_registration_table.h"


login_registration_table::login_registration_table(QSqlQuery *query)
{
    this->query = query;
    this->table = NULL;
}

void login_registration_table::create_table_inDB()
{
    query->exec("create table if not exists login_registration_table(login_ID varchar(20) not null primary key, login_PW varchar(60) not null, login_name varchar(20))");
}


bool login_registration_table::insert_view(login_registration *room)
{
    query->exec("START TRANSACTION");

    bool x = insert_table(room->get_login_ID(),room->get_login_PW());

    if(x)
    {
        query->exec("COMMIT");
        qDebug() << "commit";
    }
    else
    {
        query->exec("ROLLBACK");
        qDebug() << "rollback";
    }

    return x;
}


bool login_registration_table::insert_table(QString ID, QString PW)
{

    bool val = false;
     /*
        query->prepare("insert into login_registration_table(Num, login_ID, login_PW)"
                       " VALUES(?, ?, ?)");

        query->addBindValue(0);
        query->addBindValue(ID);
        query->addBindValue(PW);

        if(query->exec()) val = true;
    */
    return val;
}



QWidget* login_registration_table::show_table()
{
    if(this->table == NULL)
    {
    comboBox = new QComboBox();
    comboBox->addItem("All");
    comboBox->addItem("login_ID");
    comboBox->addItem("login_PW");


    lineEdit = new QLineEdit();
    pushButton = new QPushButton("검색");

    pushButton_add = new QPushButton("추가");
    pushButton_modify = new QPushButton("수정");
    pushButton_del = new QPushButton("삭제");

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(comboBox);
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(pushButton);


    query_result = new QLabel("Query Result..");
    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(query_result);


    QHBoxLayout *hLayout_bottom = new QHBoxLayout;
    hLayout_bottom->addWidget(pushButton_add);
    hLayout_bottom->addWidget(pushButton_modify);
    hLayout_bottom->addWidget(pushButton_del);

    tab_model = new QStandardItemModel(0, 3, NULL);

    table_view = new QTableView;
    table_view->setModel(tab_model);


    tab_model->setHeaderData(0, Qt::Horizontal, "login_ID");
    tab_model->setHeaderData(1, Qt::Horizontal, "login_PW");
    tab_model->setHeaderData(2, Qt::Horizontal, "login_name");


    table_view->setColumnWidth(0,160);
    table_view->setColumnWidth(1,160);
    table_view->setColumnWidth(2,160);


    tab_selectionModel = new QItemSelectionModel(tab_model);
    table_view->setSelectionModel(tab_selectionModel);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->table = new QWidget();
    this->table->resize(500,200);
    this->table->setWindowTitle("login_registration_table");

    QVBoxLayout *mainLayout = new QVBoxLayout(this->table);
    mainLayout->setSpacing(1);
    mainLayout->setMargin(1);
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(table_view);
    mainLayout->addLayout(hLayout_bottom);
    mainLayout->addLayout(hLayout2);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(button_slot()));
    connect(pushButton_add, SIGNAL(clicked()), this, SLOT(slot_add()));
    connect(pushButton_modify, SIGNAL(clicked()), this, SLOT(slot_modify()));
    connect(pushButton_del, SIGNAL(clicked()), this, SLOT(slot_delete()));

    connect(table_view, SIGNAL(clicked(const QModelIndex&)),this, SLOT(slot_table_view(QModelIndex)));
    }

    return this->table;
}




void login_registration_table::button_slot()
{
    QString text;

    if(comboBox->currentIndex() == 0){ // 전체검색
        text = QString("select * from login_registration_table");
    }else if(comboBox->currentIndex() == 1){ // 번호
        text = QString("select * from login_registration_table where login_ID like '%%1%'").arg(lineEdit->text());
    }else if(comboBox->currentIndex() == 2){ // 주인
        text = QString("select * from login_registration_table where login_PW like '%%1%'").arg(lineEdit->text());
    }

    int size = tab_model->rowCount();
    tab_model->removeRows(0, size, QModelIndex());
    query->exec(text);

    int i=0;

    while(query->next())
    {

        QString ID                  = query->value(0).toString();
        QString PW                  = query->value(1).toString();
        QString name                = query->value(2).toString();

        tab_model->insertRows(i, 1, QModelIndex());
        tab_model->setData(tab_model->index(i, 0, QModelIndex()), ID);
        tab_model->setData(tab_model->index(i, 1, QModelIndex()), PW);
        tab_model->setData(tab_model->index(i, 2, QModelIndex()), name);

        i++;
    }

    this->query_result->setText(QString("%1%2").arg(query->size()).arg("건의 레코드가 검색되었습니다."));
}




void login_registration_table::slot_add()
{
    login_add_dlg *add_dialog = new login_add_dlg(this->query);

    if(add_dialog->exec() == QDialog::Accepted)
    {
        emit this->button_slot();
        this->query_result->setText("Add Success!");
    }
    else
    {
        this->query_result->setText("Add Fail!");
    }
}



void login_registration_table::slot_modify()
{

    QList<QModelIndex> list;
    QModelIndex index;

    list = tab_selectionModel->selectedIndexes();

    if(list.size() == 0)
    {
        this->query_result->setText("select record!!");
    }
    else
    {
       login_modify_dlg *modify_dialog = new login_modify_dlg(this->query,this->selected_num);


        if(modify_dialog->exec() == QDialog::Accepted)
        {
            emit this->button_slot();
            this->query_result->setText("Modify Success!!");
        }
        else
        {
            this->query_result->setText("Modify Fail!!");
        }
    }

}


QVector<login_registration *> *login_registration_table::get_records()
{
    QVector<login_registration *> *list = new QVector<login_registration *>;
    query->prepare("select * from login_registration_table");
    query->exec();

    while(query->next())
    {
        login_registration * temp = new login_registration();
        temp->set_login_ID(query->value(0).toString());
        temp->set_login_PW(query->value(1).toString());

        list->append(temp);
    }
    return list;
}



void login_registration_table::slot_delete()
{
    QVector<login_registration *> *list2 = this->get_records();
    QList<QModelIndex> list;
    QModelIndex index;

    list = tab_selectionModel->selectedIndexes();

    if(list.size() == 0 )
    {
        this->query_result->setText("select record!!");
    }
    else{

        QMessageBox msgBox;
        msgBox.setText("Delete login_info");
        msgBox.setInformativeText("정말로 삭제 하시겠습니까?");
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        QString text;

        switch (ret)
        {
           case QMessageBox::Yes:

               query->exec("START TRANSACTION");

               text = QString("delete from login_registration_table where login_ID = '%1'").arg(list2->at(selected_num)->get_login_ID());

               if(query->exec(text))
               {
                   emit this->button_slot();
                   this->query_result->setText("Delete Success!");
                   query->exec("COMMIT");
               }
               else
               {
                   query->exec("ROLLBACK");
               }

               break;
           case QMessageBox::No:
                this->query_result->setText("Delete Fail!");
               break;
           default:
               break;
         }
    }
}


void login_registration_table::slot_table_view(const QModelIndex &index)
{
    int row = index.row();
    this->selected_num = row;
}



QPushButton* login_registration_table::get_pushbutton()
{
    return this->pushButton;
}


QSqlQuery * login_registration_table::get_query()
{
    return this->query;
}
