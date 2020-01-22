#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <iostream>
#include <fstream>

#include "qpointfb.h"
#include "edge.h"

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPointFB> a,b;
    std::vector<Edge> res;
    bool ab;
    std::string path;
    std::vector<QPointF> intersections_alone_a, intersections_alone_b, intersections_alone_res;

public:
    explicit Draw(QWidget *parent = nullptr);
    void changePolygon(){ab = !ab;}
    void setA (std::vector<QPointFB> &a_){a = a_;}
    void setB (std::vector<QPointFB> &b_){b = b_;}
    void setRes (std::vector<Edge> res_){res = res_;}
    void setPath(std::string &path_){path = path_;}
    void setIntersectsAloneA(std::vector<QPointF> &intersections_alone_a_){intersections_alone_a = intersections_alone_a_;}
    void setIntersectsAloneB(std::vector<QPointF> &intersections_alone_b_){intersections_alone_b = intersections_alone_b_;}
    void setIntersectsAloneRes(std::vector<QPointF> &intersections_alone_res_){intersections_alone_res = intersections_alone_res_;}

    std::vector<QPointFB> getA(){return a;}
    std::vector<QPointFB> getB(){return b;}
    std::vector<Edge> getRes(){return res;}
    std::vector<QPointF> getIntersectsAloneA(){return intersections_alone_a;}
    std::vector<QPointF> getIntersectsAloneB(){return intersections_alone_b;}
    std::vector<QPointF> getIntersectsAloneRes(){return intersections_alone_res;}

    void clearResults() {res.clear(); intersections_alone_a.clear(); intersections_alone_b.clear(); intersections_alone_res.clear();}
    void clearAll() {res.clear(); a.clear(); b.clear(); intersections_alone_a.clear(); intersections_alone_b.clear(); intersections_alone_res.clear();}
    void clearInterA() {intersections_alone_a.clear();}
    void clearInterB() {intersections_alone_b.clear();}
    void clearInterRes() {intersections_alone_res.clear();}

    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    void drawPolygon(QPainter &painter, std::vector<QPointFB> &polygon);
    void loadTxtFile(std::vector<QPointFB> &a_, std::vector<QPointFB> &b_, std::string &path);

signals:

public slots:
};

#endif // DRAW_H
