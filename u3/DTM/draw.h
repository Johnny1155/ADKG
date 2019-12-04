#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <cmath>

#include "qpoint3d.h"
#include "edge.h"
#include "qcontdescr.h"
#include "orcuntours.h"
#include "triangle.h"

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint3D> points;
    std::vector<QPoint3D> points_inside_pol;
    std::vector<Edge> dt;
    std::vector<Edge> contours, main_contours;
    std::vector<QContDescr> contours_descr;
    std::vector<Triangle> dtm;
    std::vector<double> angles;

    bool slope=false;
    bool aspect=false;
    bool polygon_draw = false;

    int color;
    int color_2;

    QPolygonF polygon;


public:
    explicit Draw(QWidget *parent = nullptr);
    void setPoints(std::vector<QPoint3D> &points_){points=points_;}
    void setDt(std::vector<Edge> &dt_){dt=dt_;}
    void setContours(std::vector<Edge> &contours_){contours=contours_;}
    void setMainContours(std::vector<Edge> &main_contours_){main_contours=main_contours_;}
    void setAngles(std::vector<double> angles_){angles=angles_;}
    void setPolygonDraw(bool polygon_draw_){polygon_draw=polygon_draw_;}
    void setPolygon(QPolygonF &polygon_){polygon=polygon_;}
    void setPointsInsidePol(std::vector<QPoint3D> points_){points_inside_pol=points_;}
    void setContoursDesc(std::vector<QContDescr> &cd_){contours_descr=cd_;}
    void setDTM(std::vector<Triangle> & dtm_){dtm=dtm_;}
    void setSlope(bool slope_){slope=slope_;}
    void setAspect(bool aspect_){aspect=aspect_;}
    void setColor(int &color_){color=color_;}
    void setColor2(int &color2_){color_2=color2_;}

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    std::vector<QPoint3D> getPoints(){return points;}
    std::vector<QPoint3D> getPointsInsidePol(){return points_inside_pol;}
    QPolygonF getPolygon(){return polygon;}
    std::vector<Edge> getMainContours(){return main_contours;}
    std::vector<Edge> getContours(){return contours;}
    std::vector<Edge> getDt(){return dt;}
    int getDtSize(){return dt.size();}

    void clearPoints(){points.clear(); }
    void clearDT(){dt.clear();}
    void clearContours(){contours.clear();}
    void clearMainContours(){main_contours.clear();}
    void clearAngles(){angles.clear();}
    void clearPolygon(){polygon.clear();}
    void clearPointsInsidePol(){points_inside_pol.clear();}
    void clearContDesc(){contours_descr.clear();}

    void clearDtm(){dtm.clear();}


signals:

public slots:
};

#endif // DRAW_H
