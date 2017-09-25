#include "open_cv_thread.h"
#include <QDebug>
#include <QThread>




open_cv_worker::open_cv_worker(QSqlQuery *db)//graph_draw *temp)
{
    query = db;
    this->open_cv_widget = new open_cv();
    //tagCloudScrollArea = new QScrollArea();
    //tagCloudScrollArea->setWidget(this->open_cv_widget);

}



void open_cv_worker::process_image_data()
{
    QCamera* temp =  this->get_open_cv_widget()->get_camera();

    temp->setCaptureMode(QCamera::CaptureStillImage);
    temp->start();
    temp->searchAndLock();
    this->get_open_cv_widget()->get_image_capture()->capture("/home/csy/test/images/still_shot2.jpg");

    temp->unlock();

    qDebug() << "still shot";

     if(this->get_open_cv_widget()->image_processing())
     {
         qDebug() << "번호 추출 성공";

         QString qrc =  QString("select count(*) from vehicle_registration_table where vehicle_number ='%1'")
                 .arg(this->get_open_cv_widget()->get_output_text_num());

        query->exec(qrc);
        QString value;

        while(query->next())
        {
            int count = query->value(0).toInt();

            if(count == 1)
            {
                value = "DB 에 존재합니다 TRUE";
                emit this->notice_open_door();
            }
            else
            {
                value = "DB 에 존재하지 않습니다FALSE";
            }
            qDebug() << value;
        }
     }
     else
     {
         qDebug() << "번호 추출 실패";
         this->get_open_cv_widget()->set_bool_text_num("fail");
     }

    qDebug() <<  "open_CV thread" << QThread::currentThreadId();

}


open_cv* open_cv_worker::get_open_cv_widget()
{
    return this->open_cv_widget;
}

QScrollArea* open_cv_worker::get_scrollarea()
{
    return this->tagCloudScrollArea;
}
