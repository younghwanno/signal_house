#ifndef __GRAPH_WORKER_H__
#define __GRAPH_WORKER_H__

#include <QObject>
#include "graph.h"
#include <QScrollArea>
#include <QScrollBar>

class testing : public QObject
{
    friend class graph_draw;

    Q_OBJECT

    private:
        QScrollArea *tagCloudScrollArea;
        graph_draw *graph;

    public:
        testing();
        graph_draw* get_graph();
        QScrollArea* get_scrollarea();

    signals:
        void notice_graph_data();
        void request_graph_draw();
    public slots:
        void process_graph_data();
};


#endif
