#include "graph_thread.h"
#include <QDebug>
#include <QThread>






testing::testing()//graph_draw *temp)
{
    graph = new graph_draw();

    tagCloudScrollArea = new QScrollArea();
    tagCloudScrollArea->setWidget(this->graph);

    tagCloudScrollArea->setFixedHeight(130);
    tagCloudScrollArea->setFixedWidth(240);

    connect(this, SIGNAL(notice_graph_data()), this, SLOT(process_graph_data()));
}



void testing::process_graph_data()
{
    this->graph->update();
    this->tagCloudScrollArea->horizontalScrollBar()->setValue(this->graph->x_index);
}


graph_draw* testing::get_graph()
{
    return this->graph;
}

QScrollArea* testing::get_scrollarea()
{
    return this->tagCloudScrollArea;
}
