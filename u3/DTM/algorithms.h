#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <list>
#include <iostream>
#include <fstream>

#include "qpoint3d.h"
#include "edge.h"
#include "sortbyx.h"
#include "sortbyz.h"
#include "qcontdescr.h"
#include "orcuntours.h"
#include "triangle.h"

class Algorithms
{
public:
    Algorithms();
    static int getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2);
    static double getPointLineDistance(QPoint3D &q, QPoint3D &p1, QPoint3D &p2);
    static double getAngle2Vectors(QPoint3D &p1,QPoint3D &p2,QPoint3D &p3,QPoint3D &p4);
    static double get2PointsAzimuth(QPoint3D &p1, QPoint3D &p2);
    static double getCircleRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &c);
    static int getNearestpoint(QPoint3D &p, std::vector<QPoint3D> &points);
    static double distance2Points(QPoint3D &p1, QPoint3D &p2);

    static int getDelaunayPoint(QPoint3D &s, QPoint3D &e, std::vector<QPoint3D> &points);
    static std::vector<Edge> DT(std::vector<QPoint3D> &points);
    static QPoint3D getContourPoint(QPoint3D &p1, QPoint3D &p2, double z);
    static std::vector<Edge> createContourLines(std::vector<Edge> &dt,
                                                double z_min,
                                                double z_max,
                                                double dz);

    static int positionPointPolygonWinding(QPoint3D &q, QPolygonF &pol);
    static std::vector<QPoint3D> calculatePointsInsidePolygon(std::vector<QPoint3D> &points, QPolygonF &pol);
    static std::vector<Edge> calculateContoursInsidePolygon(std::vector<Edge> &conts, QPolygonF &pol);

    static void getLineParameters(QPoint3D &p1, QPoint3D &p2, double &m, double &b, double &dist);
    static void getLineParameters(QPoint3D &p1, QPoint3D &p2, double &m, double &b);

    //Generating terain shapes
    static std::vector<QPoint3D> generateValley(int &max_x, int &max_y);
    static std::vector<QPoint3D> generateHill(int &max_x, int &max_y);
    static std::vector<QPoint3D> generateRidge(int &max_x, int &max_y);
    static std::vector<QPoint3D> generateVolcano(int &max_x, int &max_y);
    static std::vector<QPoint3D> generateSaddle(int &max_x, int &max_y);

    //Load file
    static std::vector<QPoint3D> loadFile(std::string &path, int &canvas_x, int &canvas_y);

    //Contours describtion
    static std::vector<QContDescr> describeContours(std::vector<OrCuntours> &cont);
    static std::vector<OrCuntours> orientateContours(std::vector<Edge> &cont);

    //Analyze DMT
    static double calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    static double calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    static std::vector<Triangle> analyzeDTM(std::vector<Edge> & dt);
};

#endif // ALGORITHMS_H
