#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    //Draw points
    for (int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x() - 2, points[i].y() - 2, 4, 4);
    }


    //Draw edges
    QPen p(Qt::green, 1);
    painter.setPen(p);

    for (int i = 0; i < dt.size(); i++)
    {
        painter.drawLine(dt[i].getStart(), dt[i].getEnd());
    }

    //Draw countour lines
    QPen q(Qt::gray, 1);
    painter.setPen(q);

    for (int i = 0; i < contours.size(); i++)
    {
        painter.drawLine(contours[i].getStart(), contours[i].getEnd());
    }

    //Draw main contour lines
    QPen cm(Qt::gray, 3);
    QPen text(Qt::blue,2);

    painter.setPen(cm);
    for (int i=0; i<main_contours.size(); i++)
    {
        painter.drawLine(main_contours[i].getStart(), main_contours[i].getEnd());
    }

    //Draw text on contours
    painter.setPen(text);
    for (int i=0; i<contours_descr.size(); i++)
    {
        QContDescr d=contours_descr[i];

        QString text=QString::number(d.getZ());

        painter.save();
        painter.translate(d.getP().x(),d.getP().y());
        painter.rotate(d.getAngle());
        painter.drawText(-3, 3, text);
        painter.restore();
        
    }

    QPen slap(Qt::gray, 0);
    painter.setPen(slap);

    //Draw slope and aspect
    if (slope)
    {
        double k=255.0/180;

        for (Triangle t:dtm)
        {
            QPoint3D p1=t.getP1();
            QPoint3D p2=t.getP2();
            QPoint3D p3=t.getP3();

            int sl=255-t.getSlope()*k;

            QColor c;

            if (color==0)
            {
                c.setRed(sl);
                c.setGreen(0);
                c.setBlue(0);
            }
            else if (color==1)
            {
                c.setRed(0);
                c.setGreen(sl);
                c.setBlue(0);
            }
            else if (color==2)
            {
                c.setRed(0);
                c.setGreen(0);
                c.setBlue(sl);
            }

            painter.setBrush(c);

            QPolygonF triangle;
            triangle.append(QPointF(p1.x(), p1.y()));
            triangle.append(QPointF(p2.x(), p2.y()));
            triangle.append(QPointF(p3.x(), p3.y()));

            painter.drawPolygon(triangle);
        }
    }

    if (aspect)
    {
        double k=255.0/360;

        for (Triangle t:dtm)
        {
            QPoint3D p1=t.getP1();
            QPoint3D p2=t.getP2();
            QPoint3D p3=t.getP3();

            double asp=t.getAspect();

            if (asp<0)
                asp=asp+360;

            int sl=255-asp*k;

            QColor c;

            if (color_2==0)
            {
                c.setRed(sl);
                c.setGreen(0);
                c.setBlue(0);
            }
            else if (color_2==1)
            {
                c.setRed(0);
                c.setGreen(sl);
                c.setBlue(0);
            }
            else if (color_2==2)
            {
                c.setRed(0);
                c.setGreen(0);
                c.setBlue(sl);
            }

            painter.setBrush(c);

            QPolygonF triangle;
            triangle.append(QPointF(p1.x(), p1.y()));
            triangle.append(QPointF(p2.x(), p2.y()));
            triangle.append(QPointF(p3.x(), p3.y()));

            painter.drawPolygon(triangle);
        }
    }


    //Draw boundary polygon and its points
    QPen pen_1(Qt::red, 4);
    painter.setPen(pen_1);

    for (int i=0; i<polygon.size(); i++)
    {
        painter.drawEllipse(polygon[i].x() - 3, polygon[i].y() - 3, 6, 6);

    }
    painter.drawPolygon(polygon);


    painter.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates of cursor

    if (polygon_draw)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            QPointF p;
            p.setX(event->x());
            p.setY(event->y());
            polygon.push_back(p);
        }
    }

    repaint();
}
