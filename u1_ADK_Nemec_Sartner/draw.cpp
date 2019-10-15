#include "draw.h"


draw::draw(QWidget *parent) : QWidget(parent)
{

    // Set coordinates outside visible window
    q.setX(-100);
    q.setY(-100);

    // First calling for method for loading polygon
    loadTxtFile(polygons, path);

}


void draw::mousePressEvent(QMouseEvent *e)
{
    // Getting coordinates from mouse cursor
    int x = e->x();
    int y = e->y();

    q.setX(x);
    q.setY(y);

    repaint();
}

void draw::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    qp.begin(this);

    // Diameter of analyzed point mark
    int r_1=10;

    // Styles of painting
    qp.setPen(Qt::black);

    QBrush polStyleFill;
    polStyleFill.setColor(Qt::red);
    polStyleFill.setStyle(Qt::SolidPattern);

    QBrush polStyleNotFill;
    polStyleNotFill.setColor(Qt::white);
    polStyleNotFill.setStyle(Qt::SolidPattern);

    QBrush transpStyle;
    transpStyle.setColor(QColor::fromRgb(0,0,0,0));


    // Painting of polygons

    for (unsigned int i=0; i<polygons.size(); i++)
    {

        // If vector of colours is empty, paint nothing by red colour
        if (colorResult.size()==0)
        {
            qp.setBrush(polStyleNotFill);
            qp.drawPolygon(polygons[i]);
        }
        else
        {

            // Paint polygon by red colour
            if (colorResult[i]==true)
            {
                qp.setBrush(polStyleFill);
                qp.drawPolygon(polygons[i]);
            }

            // Don't fill polygon
            else
            {
                qp.setBrush(polStyleNotFill);
                qp.drawPolygon(polygons[i]);
            }
        }
    }

    // Draw point "q" by black colour
    qp.setPen(Qt::black);
    qp.setBrush(transpStyle);
    qp.drawEllipse(q.x()-r_1/2, q.y()-r_1/2, r_1, r_1);
    qp.end();
}

// Loads text file with polygons
void draw::loadTxtFile(vector<QPolygon> &polygons, string &path)
{

    // First clearing of privates variables
    polygons.clear();
    colorResult.clear();

    // Declaration of needed variables

    // Point from row
    QPoint P;

    // Polygon for points from file]
    QPolygon Pol;

    // Variables for separated coordinates in string format
    QString xs;
    QString ys;
    QString zk;
    QStringList zk_l;
    std::string line;

    // Variables for coordinates
    double x;
    double y;

    int j=0;

    // Loading file
    ifstream file(path);

    // Loading and sorting of data from file .txt
    if (file.is_open()){

        while (getline(file,line)) {
           zk=QString::fromStdString(line);
           zk_l=zk.split(" ");
           if (zk_l.size()!=2)
           {

               // Loading points
               xs=zk_l[1];
               ys=zk_l[2];
               x=xs.toDouble();
               y=ys.toDouble();
               P.setX(x);
               P.setY(y);
               Pol.append(P);
           }
           else if (j!=0)
           {

               // Loading polygons
               polygons.push_back(Pol);
               Pol.clear();
           }
           j++;
        }
        file.close();
    }
}
