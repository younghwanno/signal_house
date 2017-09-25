#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QVector>


class graph_draw : public QWidget
{

    friend class testing;

    private :
         QPainter *paint;
         int graph_height = 110;
         int graph_width = 240;

         int x_count = 288;

         QVector<int> * vect_x;
         QVector<int> * vect_y;

    public :
         bool toggle = false;
         int x_index;
         graph_draw();
         void paintEvent(QPaintEvent *);
         void draw_base();
         void draw_point_line();
         void set_current(int y);

};

#endif // GRAPH_H
