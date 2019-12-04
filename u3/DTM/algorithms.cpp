#include "algorithms.h"

Algorithms::Algorithms()
{

}

double Algorithms::getPointLineDistance(QPoint3D &q, QPoint3D &p1, QPoint3D &p2){
    //Calculate point and line distance
    double numerator = q.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - q.y()) + p2.x() * (q.y() - p1.y());
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double denumenator = sqrt(dx * dx + dy * dy);

    double dist = fabs(numerator)/denumenator;
    return dist;
}


double Algorithms::get2PointsAzimuth(QPoint3D &p1, QPoint3D &p2)
{
    double dx=p2.x()-p1.x();
    double dy=p2.y()-p1.y();

    double angle=atan2(dy,dx);

    return angle;
}


double Algorithms::getAngle2Vectors(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &p4)
{
    // Calculate Vector betwen 2 vectors.
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

int Algorithms::getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2)
{
    //Analyze point and line position
    //1 point in the left half plane
    //0 point in the right half plane
    //-1 point on the line
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    double t = ux * vy - uy * vx;

    //Point in the left half plane
    if (t>0)
        return 1;
    if (t<0)
        return 0;
    return -1;
}


double Algorithms::getCircleRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &c){

    //Return radius of the circle given by 3 points
    double x1 = p1.x();
    double y1 = p1.y();

    double x2 = p2.x();
    double y2 = p2.y();

    double x3 = p3.x();
    double y3 = p3.y();

    //Calculate coeficients k1-k12
    double k1 = x1*x1 + y1*y1;
    double k2 = x2*x2 + y2*y2;
    double k3 = x3*x3 + y3*y3;
    double k4 = y1 - y2;
    double k5 = y1 - y3;
    double k6 = y2 - y3;
    double k7 = x1 - x2;
    double k8 = x1 - x3;
    double k9 = x2 - x3;
    double k10 = x1 * x1;
    double k11 = x2 * x2;
    double k12 = x3 * x3;

    //Midpoint of the circle
    double m_numerator = k12 * (-k4) + k11 * k5 - (k10 + k4*k5)*k6;
    double m_denominator = x3 * (-k4) + x2 * k5 + x1 * (-k6);
    double m = 0.5 * m_numerator / m_denominator;

    double n_numerator = k1 * (-k9) + k2 * k8 + k3 * (-k7);
    double n_denominator = y1 * (-k9) + y2 * k8 + y3 * (-k7);
    double n = 0.5 * n_numerator / n_denominator;

    //Set circle center
    c.setX(m);
    c.setY(n);

    //Radius of the circle
    return sqrt((x1 - m) * (x1 - m) + (y1 - n) * (y1 - n));
}

double Algorithms::distance2Points(QPoint3D &p1, QPoint3D &p2)
{
     // Get distance of two points
     double dx = (p1.x() - p2.x());
     double dy = (p1.y() - p2.y());

     return sqrt(dx*dx + dy*dy);
}

int Algorithms::getNearestpoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    // Find nearest point
    int i_min = 1;
    double d_min = distance2Points(p, points[1]);

    //Browses all points
    for (unsigned int i = 2; i < points.size(); i++)
    {
        double d = distance2Points(p, points[i]);

        //Actualize minimum i and distance
        if (d < d_min)
        {
            d_min=d;
            i_min=i;
        }
    }

    return i_min;
}

int Algorithms::getDelaunayPoint(QPoint3D &s, QPoint3D &e, std::vector<QPoint3D> &points)
{
    //Find optimal Delaunay point
    int i_min = -1;
    double r_min = INFINITY;

    //Browse all points
    for (unsigned int i = 0; i < points.size(); i++ )
    {
        //Test if points[i] is different from start, end
        if((s != points[i]) && (e != points[i]))
        {
            //Points[i] in the left half plane
            if(getPointLinePosition(points[i], s, e) == 1)
            {
                //Get circle radius
                QPoint3D c;
                double r = getCircleRadius(s, e, points[i], c);

                //Circle center in the right half plane
                if (getPointLinePosition(c, s, e) == 0)
                    r = -r;

                //Actualize minimum
                if(r < r_min)
                {
                    r_min = r;
                    i_min = i;
                }
            }
        }
    }

    return i_min;
}

std::vector<Edge> Algorithms::DT(std::vector<QPoint3D> &points)
{
    //Delaunay triangulation
    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Sort points by X
    sort(points.begin(), points.end(), SortbyX());

    //Pivot
    QPoint3D q = points[0];

    //Find nearest point
    int index = getNearestpoint(q, points);
    QPoint3D qn = points[index];

    //First Delaunay edge
    Edge e1(q, qn);

    //Find optimal Delaunay point
    int i_o = getDelaunayPoint(q, qn, points);

    //No suitable point
    if (i_o == -1)
    {
        //Change orientation
        e1.changeOrientation();

        //Find optimal Delaunay point
        i_o = getDelaunayPoint(e1.getStart(), e1.getEnd(), points);
    }

    //Third point of the triangle
    QPoint3D q3 = points[i_o];

    //Create initial triangle
    Edge e2(e1.getEnd(), q3);
    Edge e3(q3, e1.getStart());

    //Add first triangle to DT
    dt.push_back(e1);
    dt.push_back(e2);
    dt.push_back(e3);

    //Add edges to active edges list
    ael.push_back(e1);
    ael.push_back(e2);
    ael.push_back(e3);

    //Repeat until any triangle exists
    while (!ael.empty())
    {
        Edge ed1=ael.back();
        ael.pop_back();

        //Change orientation of edge
        ed1.changeOrientation();

        //Find optinal Delaunay point
        int i2 = getDelaunayPoint(ed1.getStart(), ed1.getEnd(), points);

        if (i2 != -1)
        {
            // Third point of the triangle
            QPoint3D q3n = points[i2];

            //Create new triangle
            Edge ed2(ed1.getEnd(), q3n);
            Edge ed3(q3n, ed1.getStart());

            //Add triangle to DT
            dt.push_back(ed1);
            dt.push_back(ed2);
            dt.push_back(ed3);

            //Change orientation
            ed2.changeOrientation();
            ed3.changeOrientation();

            //Are ed2 and ed3 in active edges list
            std::list<Edge>::iterator ie2 = find(ael.begin(), ael.end(), ed2);
            std::list<Edge>::iterator ie3 = find(ael.begin(), ael.end(), ed3);

            //Add edge ed2 to active edge list
            if (ie2 == ael.end())
            {
                ed2.changeOrientation();
                ael.push_back(ed2);
            }

            //Edge ed2 already in active edge list, erase
            else ael.erase(ie2);

            //Add edge ed3 to active edge list
            if (ie3 == ael.end())
            {
                ed3.changeOrientation();
                ael.push_back(ed3);
            }

            //Edge ed3 already in active edge list, erase
            else ael.erase(ie3);
        }
    }

    return  dt;
}

QPoint3D Algorithms::getContourPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    //Compute contour point
    double x = (p2.x() - p1.x())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.x();
    double y = (p2.y() - p1.y())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.y();
    QPoint3D A(x, y, z);

    return A;
}

std::vector<Edge> Algorithms::createContourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz)
{
    //Generate contour lines
    std::vector<Edge> contours;

    //Browse all triangles
    for (unsigned int i=0; i<dt.size(); i+=3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

    /*    qDebug()<<"trojak";
        qDebug()<<p1;
        qDebug()<<p2;
        qDebug()<<p3;*/



        // Find all plane - triangle intersections.
        for (double z = z_min; z <= z_max; z += dz)
        {
            Edge e;
            QPoint3D a;
            QPoint3D b;

            // Get height differences
            double dz1 = z - p1.getZ();
            double dz2 = z - p2.getZ();
            double dz3 = z - p3.getZ();

            //Test criteria
            double t12 = dz1 * dz2;
            double t23 = dz2 * dz3;
            double t31 = dz3 * dz1;

            //Triangle in the plane(coplanar)
            if((dz1 == 0) && (dz2 == 0) && (dz3 == 0))
                continue;

            //Edge e12 of the triangle in the plane
            else if ((dz1 == 0) && (dz2 == 0))
            {
                    contours.push_back(dt[i]);
            }

            //Edge e23 of the triangle in the plane
            else if ((dz2 == 0) && (dz3 == 0))
            {
                    contours.push_back(dt[i+1]);
            }

            //Edge e31 of the triangle in the plane
            else if ((dz3 == 0) && (dz1 == 0))
            {
                    contours.push_back(dt[i+2]);
            }

            //Egdes e12, e13 are intersected
            else if ((t12 < 0 && t23 <= 0) || (t12 <= 0 && t23 < 0))
            {
                //Compute intersection points
                a = getContourPoint(p1, p2, z);
                b = getContourPoint(p2, p3, z);

                // Create fragment of contour line.

                if (z>=p1.getZ())
                {
                    e.setStart(a);
                    e.setEnd(b);
                }
                else
                {
                    e.setStart(b);
                    e.setEnd(a);
                }


                contours.push_back(e);
            }

            //Egdes e23, e31 are intersected
            else if ((t23 < 0 && t31 <= 0) || (t23 <= 0 && t31 < 0))
            {
                //Compute intersection points
                a = getContourPoint(p2, p3, z);
                b = getContourPoint(p3, p1, z);

                // Create fragment of contour line.


                if (z>=p2.getZ())
                {
                    e.setStart(a);
                    e.setEnd(b);
                }
                else
                {
                    e.setStart(b);
                    e.setEnd(a);
                }

                contours.push_back(e);
            }

            //Egdes e31, e12 are intersected
            else if ((t31 < 0 && t12 <= 0) || (t31 <= 0 && t12 < 0))
            {
                //Compute intersection points
                a = getContourPoint(p3, p1, z);
                b = getContourPoint(p1, p2, z);

                // Create fragment of contour line

                if (z>=p3.getZ())
                {
                    e.setStart(a);
                    e.setEnd(b);
                }
                else
                {
                    e.setStart(b);
                    e.setEnd(a);
                }

                contours.push_back(e);
            }
        }
    }

    return contours;
}

int Algorithms::positionPointPolygonWinding(QPoint3D &q, QPolygonF &pol)
{
    // Analyze Position of the Point and the Polygon by Winding method

    // Winding number
    double wn = 0.0;

    // Tolerance
    double eps = 1.0e-6;

    // The size of polygon
    unsigned long long n = pol.size();

    QPoint3D q_f, p_i, p_ii;

    // Browse all points of polygon
    for (unsigned int i = 0; i < n; i++){

        q_f.setX(q.x());
        q_f.setY(q.y());
        p_i.setX(pol[i].x());
        p_i.setY(pol[i].y());
        p_ii.setX(pol[(i+1)%n].x());
        p_ii.setY(pol[(i+1)%n].y());

        // Measure angle
        double omega = getAngle2Vectors(p_i, q_f, p_ii, q_f);

        // Get orientation of the point and the polygon edge
        int orient = getPointLinePosition(q_f, p_i, p_ii);

        // Point on the edge
        if (orient == -1)
            return -1;

        // Point on the line but not on the edge
        if (orient == -2)
            return -2;

        // Point in the left half plane
        if (orient == 1)
            wn += omega;

        // Point in the right half plane
        else
            wn -= omega;
    }

    // Point inside polygon
    if (fabs(fabs(wn) - 2 * M_PI) <= eps)
        return 1;

    // Point outside polygon
    return 0;
}


std::vector<QPoint3D> Algorithms::calculatePointsInsidePolygon(std::vector<QPoint3D> &points, QPolygonF &pol)
{
    std::vector<QPoint3D> points_new;

    //Searching, which points are in the polygon
    for (int i=0; i<points.size(); i++)
    {

        int result = positionPointPolygonWinding(points[i], pol);

        if (result==1)
            points_new.push_back(points[i]);
    }

    return points_new;
}

std::vector<Edge> Algorithms::calculateContoursInsidePolygon(std::vector<Edge> &conts, QPolygonF &pol)
{
    std::vector<Edge> contours;
    QPoint3D b, e;

    int res_b;
    int res_e;

    for (int i=0; i<conts.size(); i++)
    {
        b.setX(conts[i].getStart().x());
        b.setY(conts[i].getStart().y());
        e.setX(conts[i].getEnd().x());
        e.setY(conts[i].getEnd().y());

        res_b=positionPointPolygonWinding(b, pol);
        res_e=positionPointPolygonWinding(e, pol);

        if ((res_b==1) && (res_e==1))
        {
            contours.push_back(conts[i]);
        }
    }


    return contours;
}

void Algorithms::getLineParameters(QPoint3D &p1, QPoint3D &p2, double &m, double &b, double &dist)
{
    double x1=p1.x();
    double x2=p2.x();
    double y1=p1.y();
    double y2=p2.y();

    double dx=x2-x1;
    double dy=y2-y1;

    dist=sqrt(dx*dx + dy*dy);

    m=dy/dx;
    b=y1-(m*x1);
}

void Algorithms::getLineParameters(QPoint3D &p1, QPoint3D &p2, double &m, double &b)
{
    double x1=p1.x();
    double x2=p2.x();
    double y1=p1.y();
    double y2=p2.y();

    double dx=x2-x1;
    double dy=y2-y1;

    m=dy/dx;
    b=y1-(m*x1);
}

double Algorithms::calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Calculate slope of the triangle
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy * vz - uz * vy;
    double ny = -(ux * vz - vx * uz);
    double nz = ux * vy - uy * vx;
    double nt = sqrt(nx * nx + ny * ny + nz * nz);

    return acos(nz / nt) / M_PI * 180;
}

double Algorithms::calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Calculate slope of the triangle
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy * vz - uz * vy;
    double ny = -(ux * vz - vx * uz);

    return atan2(nx, ny) / M_PI * 180;
}

std::vector<Triangle> Algorithms:: analyzeDTM(std::vector<Edge> & dt)
{
    //Analyze DTM compute slope and aspect
    std::vector<Triangle> triangles;

    //Browse triangulation one by one
    for (int i = 0; i < dt.size(); i += 3)
    {
        //Vertices of the triangle
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Slope and aspect
        double slope = calculateSlope(p1, p2, p3);
        double aspect = calculateAspect(p1, p2, p3);

        //Create triangle
        Triangle t(p1, p2, p3, slope, aspect);

        //Add triangle to vector
        triangles.push_back(t);
    }

    return triangles;
}




std::vector<QPoint3D> Algorithms::generateValley(int &max_x, int &max_y)
{

    //Generator of valley
    std::vector<QPoint3D> points;

    QPoint3D a, b;
    a.setX(30);
    a.setY(30);
    double az=(double)rand()/RAND_MAX*120;
    double bz=(double)rand()/RAND_MAX*2;
    a.setZ(az);

    b.setX(max_y-50);
    b.setY(max_x-50);
    b.setZ(a.getZ()+bz);

    points.push_back(a);
    points.push_back(b);

    int n_1;
    int n_2;
    double dist_ab;
    double m;
    double bl;

    getLineParameters(a, b, m, bl, dist_ab);

    n_1=20;
    n_2=10;

    //Generating bottom points of valley
    double dx=(b.x()-a.x())/n_2;
    double dz=b.getZ()-a.getZ();

    QPoint3D point_new;

    for (int i=0; i<n_2; i++)
    {
        double random=(double)rand()/RAND_MAX*80;
        double random_2=(double)rand()/RAND_MAX;
        point_new.setX((a.x()+dx*(i+1))+random);

        double base_y=m*point_new.x()+bl;
        point_new.setY(base_y-random);

        point_new.setZ(a.getZ()+dz*random_2);

        points.push_back(point_new);
    }

    double diff_x=(b.x())/n_1;
    double diff_y=(b.y())/n_1;

    //Generate points around
    for (int i=0; i<n_1; i++)
    {
        for (int j=0; j<n_1; j++)
        {
            double r_x=(double)rand()/RAND_MAX*20;
            double r_y=(double)rand()/RAND_MAX*20;
            double r_z=(double)rand()/RAND_MAX*2;

            point_new.setX((i+1)*diff_x+r_x);
            point_new.setY((j+1)*diff_y+r_y);

            double dist=getPointLineDistance(point_new, a, b)/50;
            point_new.setZ(a.getZ()+dist*dist+r_z);

            points.push_back(point_new);
        }
    }




    return points;
}

std::vector<QPoint3D> Algorithms::generateHill(int &max_x, int &max_y)
{
    std::vector<QPoint3D> points;

    QPoint3D a, p;
    a.setX(max_y/2);
    a.setY(max_x/2);
    double r_z=(double)rand()/RAND_MAX*200+200;
    a.setZ(r_z);

    points.push_back(a);

    double dx=max_y/20;
    double dy=max_x/20;

    for (int i=0; i<20; i++)
    {
        for (int j=0; j<20; j++)
        {
            r_z=(double)rand()/RAND_MAX*1;
            double r_x=(double)rand()/RAND_MAX*20;
            double r_y=(double)rand()/RAND_MAX*20;

            p.setX((i+1)*dx+r_x);
            p.setY((j+1)*dy+r_y);

            double dist=distance2Points(a, p)/70;

            if (dist>2.5)
                p.setZ(a.getZ()-2.6*2.6+r_z);
            else
                p.setZ(a.getZ()-dist*dist+r_z);

            points.push_back(p);
        }
    }

    return points;
}

std::vector<QPoint3D> Algorithms::generateRidge(int &max_x, int &max_y)
{
    //Generator of ridge
    std::vector<QPoint3D> points;

    QPoint3D a, b;
    a.setX(30);
    a.setY(30);
    double az=(double)rand()/RAND_MAX*150;
    double bz=(double)rand()/RAND_MAX*2;
    a.setZ(az);

    b.setX(max_y-180);
    b.setY(max_x-180);
    b.setZ(a.getZ()+bz);

    points.push_back(a);
    points.push_back(b);

    int n_1;
    int n_2;
    double dist_ab;
    double m;
    double bl;


    getLineParameters(a, b, m, bl, dist_ab);

    if (dist_ab<200)
    {
        n_1=5;
        n_2=1;
    }
    else if (dist_ab>201 && dist_ab<400)
    {
        n_1=8;
        n_2=5;
    }
    else if (dist_ab>401 && dist_ab<600)
    {
        n_1=10;
        n_2=8;
    }
    else
    {
        n_1=13;
        n_2=10;
    }


    //Generating bottom points of valley
    double dx=(b.x()-a.x())/n_2;
    double dz=b.getZ()-a.getZ();

    QPoint3D point_new;

    for (int i=0; i<n_2; i++)
    {
        double random=(double)rand()/RAND_MAX*80;
        double random_2=(double)rand()/RAND_MAX;
        point_new.setX((a.x()+dx*(i+1))+random);

        double base_y=m*point_new.x()+bl;
        point_new.setY(base_y-random);

        point_new.setZ(a.getZ()+dz*random_2);

        points.push_back(point_new);
    }

    double diff_x=(max_y)/n_1;
    double diff_y=(max_x)/n_1;

    //Generate points around
    for (int i=0; i<n_1; i++)
    {
        for (int j=0; j<n_1; j++)
        {
            double r_x=(double)rand()/RAND_MAX*20;
            double r_y=(double)rand()/RAND_MAX*20;
            double r_z=(double)rand()/RAND_MAX*2;

            point_new.setX((i+1)*diff_x+r_x);
            point_new.setY((j+1)*diff_y+r_y);

            double dist=getPointLineDistance(point_new, a, b)/50;
            point_new.setZ(a.getZ()-dist*dist+r_z);

            points.push_back(point_new);
        }
    }




    return points;
}

std::vector<QPoint3D> Algorithms::generateVolcano(int &max_x, int &max_y)
{
    std::vector<QPoint3D> points;

    QPoint3D p;
    QPoint3D orig(0,0,0);

    double x_cof=max_y/10;
    double y_cof=max_x/10;
    double r_z=(double)rand()/RAND_MAX*200;

    for (double i=-5; i<5; i+=0.2)
    {
        for (double j=-5; j<5; j+=0.2)
        {
            QPoint3D px(i, j, 0);

            double r_x=(double)rand()/RAND_MAX*20;
            double r_y=(double)rand()/RAND_MAX*20;

            double dist=distance2Points(orig, px);
            double z;

            if (dist<M_PI/2)
            {
                z=(cos(dist*dist)+sin(dist*dist)+1)*r_z;
            }
            else
            {
                z=(1/dist+2)*r_z;
            }

            qDebug()<<z;



            p.setX((5+i)*x_cof+r_x);
            p.setY((5+j)*y_cof+r_y);
            p.setZ(z);

            points.push_back(p);
        }
    }



    return points;
}

std::vector<QPoint3D> Algorithms::generateSaddle(int &max_x, int &max_y)
{
    std::vector<QPoint3D> points;

    QPoint3D p;
    double r_z=(double)rand()/RAND_MAX*200;

    for (double i=-0.95; i<0.96; i+=0.05)
    {
        for (double j=-0.95; j<0.96; j+=0.05)
        {
            double r_zz=(double)rand()/RAND_MAX*2;
            double r_x=(double)rand()/RAND_MAX*10;
            double r_y=(double)rand()/RAND_MAX*10;
            double z=50*(sin(i*i)+cos(j*j))+r_z+r_zz;
            double x=(i+1)*max_y/2+r_x;
            double y=(j+1)*max_x/3+r_y;

            p.setX(x);
            p.setY(y);
            p.setZ(z);

            points.push_back(p);
        }
    }

    return points;
}

std::vector<QPoint3D> Algorithms::loadFile(std::string &path, int &canvas_x, int &canvas_y)
{
    std::vector<QPoint3D> points;

    double max_x=std::numeric_limits<double>::min();
    double max_y=std::numeric_limits<double>::min();
    double min_x=std::numeric_limits<double>::max();
    double min_y=std::numeric_limits<double>::max();

    QString ps;
    QString xs;
    QString ys;
    QString zs;
    QStringList point;

    double x;
    double y;
    double z;

    std::ifstream file(path);
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            ps=QString::fromStdString(line);
            point=ps.split(" ");
            xs=point[1];
            ys=point[2];
            zs=point[3];

            x=xs.toDouble();
            y=ys.toDouble();
            z=zs.toDouble();

            if (x<min_x)
                min_x=x;
            if (x>max_x)
                max_x=x;
            if (y<min_y)
                min_y=y;
            if (y>max_y)
                max_y=y;

            QPoint3D p(x, y, z);
            points.push_back(p);
        }
    }
    file.close();

    double x_cof=canvas_y/(max_x-min_x);
    double y_cof=canvas_x/(max_y-min_y);

    for (int i=0; i<points.size(); i++)
    {
        points[i].setX((points[i].x()-min_x)*x_cof);
        points[i].setY((points[i].y()-min_y)*y_cof);
    }

    return points;
}

std::vector<OrCuntours> Algorithms::orientateContours(std::vector<Edge> &cont)
{
    std::vector<OrCuntours> cont_new;
    std::vector<Edge> c;
    std::vector<int> id_used;

    for (int i=0; i<cont.size(); i++)
    {


        if (std::count(id_used.begin(), id_used.end(), i))
        {
            continue;
        }
        else
        {
            id_used.push_back(i);
            int j=i+1;
            int k=i;
            c.push_back(cont[i]);

            //Searching items that are after cont[i]
            for (j; j<cont.size(); j++)
            {
                if (std::count(id_used.begin(), id_used.end(), j))
                {
                    continue;

                }
                else
                {
                    if (cont[k].getEnd()!=cont[j].getStart())
                    {
                        continue;
                    }
                    else
                    {
                        c.push_back(cont[j]);
                        id_used.push_back(k);
                        k=j;
                        j=i+1;
                    }
                }
            }

            //Reset indexs
            j=i+1;
            k=i;

            //Searching items thare before cont[i]
            for (j; j<cont.size(); j++)
            {
                if (std::count(id_used.begin(), id_used.end(), j))
                {
                    continue;
                }
                else
                {
                    if (cont[k].getStart()!=cont[j].getEnd())
                    {
                        continue;
                    }
                    else
                    {
                        c.insert(c.begin(), cont[j]);
                        id_used.push_back(j);
                        k=j;
                        j=i+1;
                    }
                }
            }
        }

        OrCuntours x;

        if (cont_new.size()>0)
        {

            QPoint3D h=cont_new.back().getOrientCont()[0].getStart();

            if (h!=c[0].getStart())
            {
                x.setOrientCont(c);
                cont_new.push_back(x);
            }
        }
        else
        {
            x.setOrientCont(c);
            cont_new.push_back(x);
        }
    }

    return cont_new;
}

std::vector<QContDescr> Algorithms::describeContours(std::vector<OrCuntours> &cont)
{
    std::vector<QContDescr> descr;
    QContDescr d;

    for (int i=0; i<cont.size(); i++)
    {
        double dist=0.0;
        std::vector<Edge> edges=cont[i].getOrientCont();

        for (int j=0; j<edges.size(); j++)
        {
            dist+=edges[j].getLength();
        }

        QPoint3D p1, p2;

        //Every main contour must be described. It depends on the legnth of whole contour, how it will be done
        if (dist<400.0)
        {
            double d1=0;
            int j=0;

            while (dist/2>d1)
            {
                d1+=edges[j].getLength();;

                p1=edges[j].getStart();
                p2=edges[j].getEnd();

                j++;
            }
            double m;
            double b;

            getLineParameters(p1, p2, m, b);

            double xh=(p1.x()+p2.x())/2;
            double yh=m*xh+b;
            QPoint3D p;
            p.setX(xh);
            p.setY(yh);

            double angle=get2PointsAzimuth(p1,p2);
            angle=angle*(180/M_PI);

            d.setAngle(angle);
            d.setP(p);
            double z=p1.getZ();
            d.setZ(z);

            descr.push_back(d);
        }
        else
        {
            int descr_count=dist/400;
            double dist_begin;

            QPoint3D p;

            for (int j=0; j<descr_count; j++)
            {
                dist_begin=400;
                double d1=0.0;

                int k=0;



                while ((j+1)*dist_begin>d1)
                {
                    d1+=edges[i+k].getLength();

                    p1=edges[i+k].getStart();
                    p2=edges[i+k].getEnd();

                    k++;
                }

                double m, b;

                getLineParameters(p1, p2, m, b);



                double xh=(p1.x()+p2.x())/2;

                double yh=m*xh+b;
                double z=p1.getZ();
                p.setX(xh);
                p.setY(yh);

                double angle=get2PointsAzimuth(p1, p2);
                angle=angle*(180/M_PI);

                d.setAngle(angle);
                d.setP(p);
                d.setZ(z);

                descr.push_back(d);
            }
        }
    }

    return descr;
}





















