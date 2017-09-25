#include "graph.h"
#include <QDebug>
#include <QTime>
#include <QScrollArea>
#include <QThread>



graph_draw::graph_draw()
{
    vect_x = new QVector<int>;
    vect_y = new QVector<int>;
    this->resize(QSize(graph_width,graph_height));

    x_index = 0;
}



void graph_draw::draw_base()
{
    paint->setPen(QPen(Qt::black,1));
    paint->drawLine(0,140,graph_width,140);

    for(int i = 0 ; i < x_count; i++)
    {
        paint->drawLine(i * 10, 95 , i * 10 , 105);
    }

   paint->setPen(QPen(Qt::black,1));
   for(int i = 130  ; i >= 10; i -=10)
   {
       paint->drawLine(0,i,graph_width,i);
   }
}



///제일 처음
void graph_draw::paintEvent(QPaintEvent *)
{
    this->resize(QSize(graph_width,graph_height));
    this->paint = new QPainter;
    paint->begin(this);


    if(graph_width < x_index + 30) graph_width += 30;

    draw_base();
    draw_point_line();

    paint->end();

}



void graph_draw::draw_point_line()
{
    paint->setPen(QPen(Qt::red,2));
    x_index = 0;

    for(int i = 1 ; i < vect_y->size() ; i++)
    {
        paint->drawPoint(x_index += 5 ,90 - vect_y->at(i));

        if(i > 0)
            paint->drawLine(x_index - 5,90 - vect_y->at(i-1), x_index,90 - vect_y->at(i));
    }
}



void graph_draw::set_current(int y)
{
    this->vect_y->append(y);
}




