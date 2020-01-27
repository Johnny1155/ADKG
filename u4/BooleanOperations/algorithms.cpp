#include "algorithms.h"
#include <cmath>
#include <map>
#include "qpointfb.h"
#include "types.h"

Algorithms::Algorithms()
{

}

TPointLinePosition Algorithms::getPointLinePosition(QPointFB &q, QPointFB &p1, QPointFB &p2)
{
    //Analyze point and line position
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    //Determinant test
    double t = ux * vy - uy * vx;

    //Point in the left half plane
    if (t > 0)
        return LeftHp;

    //Point in the right half plane
    if (t < 0)
        return RightHp;

    //Otherwise, colinear point
    return Colinear;
}


double Algorithms::getAngle2Vectors(QPointFB &p1, QPointFB &p2, QPointFB &p3, QPointFB &p4)
{
    //Calculate angle betwen 2 vectors
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    //Dot product
    double scalar = ux * vx + uy * vy;

    double angle = fabs(acos(scalar/(nu*nv)));
    return angle;
}


TPointPolygonPosition Algorithms::positionPointPolygonWinding(QPointFB &q, std::vector<QPointFB> &pol)
{
    //Analyze position of the point and the polygon
    double wn = 0.0;

    // Tolerance
    double eps = 1.0e-2;

    // The size of polygon
    unsigned long long n = pol.size();

    //Process all points of polygon
    for (unsigned int i = 0; i < n; i++)
    {
        //Measure angle betweeen two vectors
        double omega = getAngle2Vectors(pol[i], q, pol[(i+1)%n], q);

        // Point on boundary
        if ((fabs(fabs(omega) - M_PI) <= eps) || (count(pol.begin(), pol.end(), q)))
            return On;


        //Get orientation of the point and the polygon edge
        int orient = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

        //Point in the left half plane
        if (orient == LeftHp)
            wn += omega;

        //Point in the right half plane
        else
            wn -= omega;
    }

    //Point inside polygon
    if (fabs(fabs(wn) - 2 * M_PI) <= eps)
        return Inner;

    //Point outside polygon
    if (fabs(fabs(wn)) <= eps)
        return Outer;
}


T2LinesPosition Algorithms::get2LinesPosition(QPointFB &p1, QPointFB &p2, QPointFB &p3, QPointFB &p4, QPointFB &pi)
{
    //Analyze position of two lines
    double eps = 1.0e-6;

    //Compute vectors
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();
    double wx = p1.x() - p3.x();
    double wy = p1.y() - p3.y();

    //Compute k1-k3
    double k1 = vx * wy - vy * wx;
    double k2 = ux * wy - uy * wx;
    double k3 = ux * vy - uy * vx;

    //Colinear lines
    if ((fabs(k1) < eps) && (fabs(k2) < eps) && (fabs(k3) < eps))
        return Identical;

    //Parallel lines
    if ((fabs(k1) < eps) && (fabs(k2) < eps))
        return Paralel;

    //Compute alpha, beta
    double alpha = k1 / k3;
    double beta = k2 / k3;

    //Intersection exists
    if((alpha >=0) && (alpha <= 1) && (beta >=0) && (beta <= 1))
    {
        //Coordinates of the intersection
        double xpi = p1.x() + alpha * ux;
        double ypi = p1.y() + alpha * uy;

        pi.setX(xpi);
        pi.setY(ypi);
        pi.setAlpha(alpha);
        pi.setBeta(beta);

        return Intersected;
    }

    //Intersection does not exist
    return NonIntersected;
}


std::vector<Edge> Algorithms::booleanOperations(std::vector<QPointFB> &polygonA, std::vector<QPointFB> &polygonB, TBooleanOperation operation)
{
    //Create polygon overlay
    std::vector<Edge> result;

    //Find intersections
    computePolygonIntersection(polygonA, polygonB);

    //Set positions of edges
    setPositionsAB(polygonA, polygonB);

    //Select edges by position:

    //Union
    if (operation == Union)
    {
        selectEdges(polygonA, Outer, result);
        selectEdges(polygonB, Outer, result);
    }

    //Intersect
    else if(operation == Intersect)
    {
        selectEdges(polygonA, Inner, result);
        selectEdges(polygonB, Inner, result);
        selectEdges(polygonA, On, result);
        selectEdges(polygonB, On, result);
    }

    //Difference A - B
    else if(operation == DifferenceAB)
    {
        selectEdges(polygonA, Outer, result);
        selectEdges(polygonB, Inner, result);
        selectEdges(polygonA, On, result);
        selectEdges(polygonB, On, result);
    }

    //Difference B - A
    else if (operation == DifferenceBA)
    {
        selectEdges(polygonA, Inner, result);
        selectEdges(polygonB, Outer, result);
        selectEdges(polygonA, On, result);
        selectEdges(polygonB, On, result);
    }

    return result;
}


void Algorithms::processIntersection(QPointFB &pi, double &t, std::vector<QPointFB> &polygon, unsigned int &i)
{

    //Process and add intersection
    double eps = 1.0e-6;
    if ((t >= eps) && (t <= (1-eps)))
    {
        //Add point to the list
        i += 1;
        polygon.insert(polygon.begin() + i, pi);
    }
}


void Algorithms::computePolygonIntersection(std::vector<QPointFB> &pa, std::vector<QPointFB> &pb)
{
    double eps = 1.0e-6;

    //Compute intersection of two polygons
    for (unsigned int i = 0; i < pa.size(); i++)
    {
        //Create map of intersections
        std::map<double, QPointFB> intersections;
        //Polygon B
        for (unsigned int j = 0; j < pb.size(); j++)
        {
            QPointFB pi;
            if (get2LinesPosition(pa[i], pa[(i+1)%pa.size()], pb[j], pb[(j+1)%pb.size()], pi) == Intersected)
            {
                //Get alpha and beta
                double alfa = pi.getAlpha();
                double beta = pi.getBeta();

                //Add intersection to map according to alpha
                intersections[alfa] = pi;

                //Process intersection
                processIntersection(pi, beta, pb, j);
            }

            else if (get2LinesPosition(pa[i], pa[(i+1)%pa.size()], pb[j], pb[(j+1)%pb.size()], pi) == Identical)
            {
                double betaA1 = (pa[i].x() - pb[j].x()) / (pb[(j+1)%pb.size()].x() - pb[j].x());
                double betaA2 = (pa[(i+1)%pa.size()].x() - pb[j].x()) / (pb[(j+1)%pb.size()].x() - pb[j].x());
                double alphaB1 = (pb[j].x() - pa[i].x()) / (pa[(i+1)%pa.size()].x() - pa[i].x());
                double alphaB2 = (pb[(j+1)%pb.size()].x() - pa[i].x()) / (pa[(i+1)%pa.size()].x() - pa[i].x());

                if ((betaA1 >= eps) && (betaA1 <= (1 - eps)))
                {
                    pi.setX(pa[i].x());
                    pi.setY(pa[i].y());
                    pi.setBeta(betaA1);
                    pi.setAlpha(0);
                    processIntersection(pi, betaA1, pb, j);
                }

                if ((betaA2 >= eps) && (betaA2 <= (1 - eps)))
                {
                    pi.setX(pa[(i+1)%pa.size()].x());
                    pi.setY(pa[(i+1)%pa.size()].y());
                    pi.setBeta(betaA1);
                    pi.setAlpha(1);
                    processIntersection(pi, betaA1, pb, j);
                }

                if ((alphaB1 >= eps) && (alphaB1 <= (1 - eps)))
                {
                    pi.setX(pb[j].x());
                    pi.setY(pb[j].y());
                    pi.setAlpha(alphaB1);
                    pi.setBeta(0);
                    intersections[alphaB1] = pi;
                }

                if ((alphaB2 >= eps) && (alphaB2 <= (1 - eps)))
                {
                    pi.setX(pb[(j+1)%pb.size()].x());
                    pi.setY(pb[(j+1)%pb.size()].y());
                    pi.setAlpha(alphaB2);
                    pi.setBeta(1);
                    intersections[alphaB2] = pi;
                }
            }
        }

        // Intersections has been found
        if (intersections.size() > 0)
        {
            //Browse all intersections
            for (std::pair<double, QPointFB> item:intersections){

                //Intersection
                QPointFB pi = item.second;
                double alfa = pi.getAlpha();

                //Process intersection
                processIntersection(pi, alfa, pa, i);
            }
        }
    }
}


void Algorithms::setPositionsAB(std::vector<QPointFB> &pa, std::vector<QPointFB> &pb)
{
    //Set positions of edges of both polygons one another
    setPositions(pa, pb);
    setPositions(pb, pa);
}


void Algorithms::setPositions(std::vector<QPointFB> &pa, std::vector<QPointFB> &pb)
{
    //Set position of edges of polygon to another polygon
    unsigned long long n = pa.size();
    for(unsigned int i = 0; i < n; i++)
    {
        //Calculate mid-point of edge
        double mx = (pa[i].x() + pa[(i + 1)%n].x()) / 2;
        double my = (pa[i].y() + pa[(i + 1)%n].y()) / 2;

        //Find position of point m and polygon B
        QPointFB m(mx, my);
        TPointPolygonPosition position = positionPointPolygonWinding(m, pb);

        //Store position of m in the start point of the edge
        pa[i].setPosition(position);
    }
}


void Algorithms::selectEdges(std::vector<QPointFB> &pol, TPointPolygonPosition position, std::vector<Edge> &edges)
{
    //Select edges according to position
    for(unsigned int i = 0; i < pol.size(); i++)
    {
        //Apropriate edge found
        if (pol[i].getPosition() == position)
        {
            //Create edge and add to the list
            Edge e (pol[i], pol[(i+1)%pol.size()]);
            edges.push_back(e);
        }
    }
}


std::vector<QPointF> Algorithms::solve0DProblems(std::vector<QPointFB> &points, std::vector<QPointFB> &pol, std::vector<QPointF> intersections_alone)
{
    //Find intersections which are important in boolean operations but not part of resultant edges

    QPointF q;

    //Middle-points of edges (i,i+1) and (i+1, i+2)
    QPointFB m1;
    QPointFB m2;

    computePolygonIntersection(points,pol);

    for (unsigned int i = 0; i < points.size(); i++)
    {
        m1.setX((points[i].x() + points[(i+1)%points.size()].x()) / 2);
        m1.setY((points[i].y() + points[(i+1)%points.size()].y()) / 2);

        m2.setX((points[(i+1)%points.size()].x() + points[(i+2)%points.size()].x()) / 2);
        m2.setY((points[(i+1)%points.size()].y() + points[(i+2)%points.size()].y()) / 2);

        //Positions of three neighbors
        TPointPolygonPosition pos0 = positionPointPolygonWinding(m1,pol);
        TPointPolygonPosition pos1 = positionPointPolygonWinding(points[(i+1)%points.size()],pol);
        TPointPolygonPosition pos2 = positionPointPolygonWinding(m2,pol);

        //If the middle one is on the border and edges are outer
        if ((pos0 == Outer) && (pos1 == On) && (pos2 == Outer))
        {
            double x = points[(i + 1)%points.size()].x();
            double y = points[(i + 1)%points.size()].y();
            q.setX(x);
            q.setY(y);

            if (count(intersections_alone.begin(), intersections_alone.end(), q) == 0)
                intersections_alone.push_back(q);

        }
    }

    return intersections_alone;
}






