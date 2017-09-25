#ifndef OPEN_CV_THREAD_H
#define OPEN_CV_THREAD_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtSql>

#include <QObject>
#include <QScrollArea>
#include <QScrollBar>
#include "open_cv.h"

class open_cv_worker : public QObject
{
    Q_OBJECT

    private:
        open_cv *open_cv_widget;
        QScrollArea *tagCloudScrollArea;
        QSqlQuery *query;

    public:
        open_cv_worker(QSqlQuery *db);
        open_cv* get_open_cv_widget();
        QScrollArea* get_scrollarea();

    signals:
        void notice_image_data();
        void notice_open_door();
    public slots:
        void process_image_data();
};


#endif // OPEN_CV_THREAD_H
