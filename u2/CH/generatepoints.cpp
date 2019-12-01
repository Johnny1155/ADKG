#include "generatepoints.h"


GeneratePoints::GeneratePoints()
{

}

std::vector<QPoint> GeneratePoints::generatingRandomField(int &n)
{
	//Generate n-number of random points
	std::vector<QPoint> points;
	QPoint p;
	int x;
	int y;
	for (int i = 0; i < n; i++)
	{
        x = rand() % 400 + 10;
        y = rand() % 400 + 10;
		p.setX(x);
		p.setY(y);
		points.push_back(p);
	}
	return points;
}

std::vector<QPoint> GeneratePoints::generatingCircle(int &n)
{
	//Generate n-number of points on circle
	std::vector<QPoint> points;
	QPoint p;
    int xc = 210;
    int yc = 210;
	int x;
	int y;
	double angle = (2*M_PI)/n;
    double radius = 200;

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

std::vector<QPoint> GeneratePoints::generatingSquare(int &n)
{
	//Generate grid
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
			x = i * (400.0 / (n_new - 1)) + 10;

			for (int j = 0; j < n_new; j++)
			{
				y = j * (400.0 / (n_new - 1)) + 10;
				p.setX(x);
				p.setY(y);
				points.push_back(p);
			}
		}
	}
	return points;
}

std::vector<QPoint> GeneratePoints::generatingEllipse(int &n)
{
	//Generate n-number of points on ellipse
	std::vector<QPoint> points;
	QPoint p;
    int xc = 210;
    int yc = 210;
	int x;
	int y;
	double angle = (2 * M_PI) / n;
	double a = 200;
	double b = 100;

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

std::vector<QPoint> GeneratePoints::generatingStar(int &n)
{
	//Generate n-number of points on star shape
	std::vector<QPoint> points;
	QPoint p;
    int xc = 210;
    int yc = 210;
	int x;
	int y;
	double angle = (2 * M_PI) / n;
	double radius_1 = rand() % 200 + 1;
	double radius_2 = rand() % 200 + 1;
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
