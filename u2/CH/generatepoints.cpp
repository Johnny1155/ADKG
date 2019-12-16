#include "generatepoints.h"


GeneratePoints::GeneratePoints()
{

}

std::vector<QPoint> GeneratePoints::generatingRandomField(int &n, double &w, double &h)
{
	//Generate n-number of random points
    std::vector<QPoint> points;
	QPoint p;
	int x;
	int y;

    int width = round(w) - 10;
    int height = round(h) - 10;

	for (int i = 0; i < n; i++)
	{
        x = rand() % width + 10;
        y = rand() % height + 10;

		p.setX(x);
		p.setY(y);
		points.push_back(p);
	}
	return points;
}

std::vector<QPoint> GeneratePoints::generatingCircle(int &n, double &w, double &h)
{
	//Generate n-number of points on circle
	std::vector<QPoint> points;
	QPoint p;

    int width = round(w) - 10;
    int height = round(h) - 10;

    int xc = width/2;
    int yc = height/2;
	int x;
	int y;
	double angle = (2*M_PI)/n;
    double radius;

    if(width < height)
        radius = width/2 - 10;
    else
        radius = height/2 - 10;

	for (int i = 0; i < n; i++)
	{
		x = xc + radius * cos((i * angle));
		y = yc + radius * sin((i * angle));
		p.setX(x);
		p.setY(y);
		points.push_back(p);
	}
	return points;
}

std::vector<QPoint> GeneratePoints::generatingSquare(int &n, double &w, double &h)
{
	//Generate grid
    int width = round(w) - 10;
    int height = round(h) - 10;

	std::vector<QPoint> points;
	QPoint p;
	int x;
	int y;

	//Root of n rounded down
	int n_new = sqrt(n);

	for (int i = 0; i < n_new; i++)
	{

		if (n_new == 1)
		{
			p.setX(210);
			p.setY(210);
			points.push_back(p);
		}
		else
		{
            x = i * (width / (n_new - 1)) + 10;

			for (int j = 0; j < n_new; j++)
			{
                y = j * (height / (n_new - 1)) + 10;
				p.setX(x);
				p.setY(y);
				points.push_back(p);
			}
		}
	}
	return points;
}

std::vector<QPoint> GeneratePoints::generatingEllipse(int &n, double &w, double &h)
{
	//Generate n-number of points on ellipse
	std::vector<QPoint> points;
	QPoint p;

    int width = round(w) - 10;
    int height = round(h) - 10;

    int xc = width/2;
    int yc = height/2;
	int x;
	int y;
	double angle = (2 * M_PI) / n;
    double a = width/2 - 10;
    double b = height/2 - 10;

	for (int i=0; i<n; i++)
	{
		x = xc + a * cos(i * angle);
		y = yc + b * sin(i * angle);
		p.setX(x);
		p.setY(y);
		points.push_back(p);
	}
	return points;
}

std::vector<QPoint> GeneratePoints::generatingStar(int &n, double &w, double &h)
{
	//Generate n-number of points on star shape
	std::vector<QPoint> points;
	QPoint p;

    int width = round(w) - 10;
    int height = round(h) - 10;

    int xc = width/2;
    int yc = height/2;
	int x;
	int y;
    int radius_max;

    if(width < height)
        radius_max = width/2 - 10;
    else
        radius_max = height/2 - 10;

	double angle = (2 * M_PI) / n;
    double radius_1 = rand() % radius_max + 1;
    double radius_2 = rand() % radius_max + 1;
    while(radius_2 - radius_1 < 20)
    {
        radius_2 = rand() % 200 + 1;
    }

	for (int i = 0; i < n; i++)
	{
		if (i % 2)
		{
			x = xc + radius_1 * cos(i * angle);
			y = yc + radius_1 * sin(i * angle);
			p.setX(x);
			p.setY(y);
			points.push_back(p);
		}
		else
		{
			x = xc + radius_2 * cos(i * angle);
			y = yc + radius_2 * sin(i * angle);
			p.setX(x);
			p.setY(y);
			points.push_back(p);
		}
	}
	return points;
}
