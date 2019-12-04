#ifndef EDGE_H
#define EDGE_H
#include "qpoint3d.h"


class Edge
{
private:
    QPoint3D s,e;
public:
    Edge():s(0,0,0), e(0,0,0){}
    Edge(QPoint3D &start, QPoint3D &end):s(start), e(end){}

    QPoint3D & getStart(){return s;}
    void setStart(QPoint3D &s){this->s = s;}
    QPoint3D & getEnd(){return e;}
    void setEnd(QPoint3D &e){this->e = e;}

    QPointF getFStart(){QPointF fs;
                       fs.setX(s.x());
                       fs.setY(s.y());
                       return fs;}
    QPointF getFEnd()  {QPointF fe;
                       fe.setX(e.x());
                       fe.setY(e.y());
                       return fe;}
    double getLength(){double dx, dy;
                       dx=s.x()-e.x();
                       dy=s.y()-e.y();

                       return sqrt(dx*dx + dy*dy);}

    void changeOrientation(){
        QPoint3D help = s;
        s = e;
        e = help;
    }

    bool operator == (const Edge &h) const {
        return (h.s == s) && (h.e == e);
    }

};

#endif // EDGE_H
