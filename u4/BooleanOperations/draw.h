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

public:
    explicit Draw(QWidget *parent = nullptr);
    void changePolygon(){ab = !ab;}
    void setA (std::vector<QPointFB> &a_){a = a_;}
    void setB (std::vector<QPointFB> &b_){b = b_;}
    void setRes (std::vector<Edge> res_){res = res_;}
    void setPath(std::string &path_){path=path_;}

    std::vector<QPointFB> getA(){return a;}
    std::vector<QPointFB> getB(){return b;}
    std::vector<Edge> getRes(){return res;}

    void clearResults() {res.clear();}
    void clearAll() {res.clear(); a.clear(); b.clear();}

    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    void drawPolygon(QPainter &painter, std::vector<QPointFB> &polygon);
    void loadTxtFile(std::vector<QPointFB> &a_, std::vector<QPointFB> &b_, std::string &path);

signals:

public slots:
};

#endif // DRAW_H
