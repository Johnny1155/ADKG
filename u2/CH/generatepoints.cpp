#include "generatepoints.h"

GeneratePoints::GeneratePoints()
{

}

std::vector<QPoint> GeneratePoints::generatingRandomField(int &n)
{
	//Generate n random points
	std::vector<QPoint> points;
	QPoint p;
	int x;
	int y;
	for (int i = 0; i < n; i++)
	{
		x = rand() % 400 + 1;
		y = rand() % 400 + 1;
		p.setX(x);
		p.setY(y);
		points.push_back(p);
	}

	return points;
}

std::vector<QPoint> GeneratePoints::generatingCircle(int &n)
{
	//Generate n points on circle
	std::vector<QPoint> points;
	QPoint p;
	int xc = 400;
	int yc = 400;
	int x;
	int y;
	double angle = (2 * M_PI)/n;
	double radius = 210;

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
	//Generate n x n size square
	std::vector<QPoint> points;
	QPoint p;
	int x;
	int y;

	for (int i = 0; i < n; i++)
	{
		x = i * 2 + 10;
		if (n > 1)
		{
			//Square with more than 1 point
			for (int j = 0; j < n; j++)
			{
				y = j * 2 + 10;
				p.setX(x);
				p.setY(y);
				points.push_back(p);
			}
		}
		else
		{
			//1 point
			y = i * 2 + 10;
			p.setX(x);
			p.setY(y);
			points.push_back(p);
		}
	}
	return points;
}

std::vector<QPoint> GeneratePoints::generatingEllipse(int &n)
{
	//Generate n points on ellipse
	std::vector<QPoint> points;
	QPoint p;
	int xc = 400;
	int yc = 400;
	int x;
	int y;
	double angle = (2 * M_PI)/n;
	double a = 200;
	double b = 100;

	for (int i = 0; i < n; i++)
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
	//Generate n points on star shape
	std::vector<QPoint> points;
	QPoint p;
	int xc = 400;
	int yc = 400;
	int x;
	int y;
	double angle = (2 * M_PI) / n;
	double radius_1 = rand() % 200 + 1;
	double radius_2 = rand() % 200 + 1;

	for (int i = 0; i < n; i++)
	{
		if (i%2)
		{
			//Odd points - firts radius
			x = xc + radius_1 * cos(i * angle);
			y = yc + radius_1 * sin(i * angle);
			p.setX(x);
			p.setY(y);
			points.push_back(p);
		}
		else
		{
			//Even points - second radius
			x = xc + radius_2 * cos(i * angle);
			y = yc + radius_2 * sin(i * angle);
			p.setX(x);
			p.setY(y);
			points.push_back(p);
		}
	}
	return points;
}
