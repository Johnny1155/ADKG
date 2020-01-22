#include "draw.h"
#include "qpointfb.h"
#include <QtGui>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
}

void Draw::mousePressEvent(QMouseEvent *event)
{
     QPointFB q(event->x(), event->y());

     //Add to A
     if(ab)
         a.push_back(q);

     //Add to B
     else
         b.push_back(q);

     repaint();
}

void Draw::paintEvent(QPaintEvent *event)
{
    double r = 5;

    QPainter painter(this);
    painter.begin(this);

    //Draw A
    QPen pA(Qt::green);
    painter.setPen(pA);
    drawPolygon(painter, a);

    //Draw B
    QPen pB(Qt::blue);
    painter.setPen(pB);
    drawPolygon(painter, b);

    //Draw edges
    QPen pE(Qt::red);
    pE.setWidth(2);
    painter.setPen(pE);
    for(unsigned int i = 0; i < res.size(); i++)
    {
        painter.drawLine(res[i].getStart(), res[i].getEnd());
    }

    QBrush bA;
    bA.setColor(Qt::green);
    bA.setStyle(Qt::SolidPattern);

    QBrush bB;
    bB.setColor(Qt::blue);
    bB.setStyle(Qt::SolidPattern);

    QBrush bE;
    bE.setColor(Qt::red);
    bE.setStyle(Qt::SolidPattern);

    painter.setPen(pA);
    painter.setBrush(bA);
    for (unsigned int i = 0; i < intersections_alone_a.size(); i++)
    {
        painter.drawEllipse(intersections_alone_a[i].x() - r / 2, intersections_alone_a[i].y() - r / 2, r, r);
    }

    painter.setPen(pB);
    painter.setBrush(bB);
    for (unsigned int i = 0; i < intersections_alone_b.size(); i++)
    {
        painter.drawEllipse(intersections_alone_b[i].x() - r / 2, intersections_alone_b[i].y() - r / 2, r, r);
    }

    painter.setPen(pE);
    painter.setBrush(bE);
    for (unsigned int i = 0; i < intersections_alone_res.size(); i++)
    {
        painter.drawEllipse(intersections_alone_res[i].x() - r / 2, intersections_alone_res[i].y() - r / 2, r, r);
    }

    painter.end();
}

void Draw::drawPolygon(QPainter &painter, std::vector<QPointFB> &polygon)
{
    //Draw polygon
    QPolygon polyg;
    for(unsigned int i = 0; i < polygon.size(); i++)
    {
        polyg.append(QPoint (polygon[i].x(), polygon[i].y()));
    }

    painter.drawPolygon(polyg);
}

void Draw::loadTxtFile(std::vector<QPointFB> &a_, std::vector<QPointFB> &b_, std::string &path)
{
    QPointFB p;

    QString xs, ys, lineQ;
    QStringList sour;
    std::string line;

    bool next_pol=true;

    double x,y;

    std::ifstream file(path);

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            lineQ=QString::fromStdString(line);
            sour=lineQ.split(" ");

            if (sour.size()==1)
            {
                next_pol=false;
                continue;
            }

            if (next_pol)
            {
                xs=sour[0];
                ys=sour[1];

                x=xs.toDouble();
                y=ys.toDouble();

                p.setX(x);
                p.setY(y);

                a_.push_back(p);
            }
            else
            {
                xs=sour[0];
                ys=sour[1];

                x=xs.toDouble();
                y=ys.toDouble();

                p.setX(x);
                p.setY(y);

                b_.push_back(p);
            }

        }

        file.close();
    }
}



































