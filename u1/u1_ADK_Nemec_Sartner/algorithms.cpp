#include "algorithms.h"

Algorithms::Algorithms()
{

}

int Algorithms::getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2)
{
	// Analyze point and line position
	// 1 for point in the left half plane
	// 0 for point in the right half plane
	// -1 for point on the edge
	// -2 for point on the line but not on the edge

	// Tolerance
	double eps = 1.0e-6;

	// Vector p1p2
	double ux = p2.x() - p1.x();
	double uy = p2.y() - p1.y();

	// Vector p1q
	double vx = q.x() - p1.x();
	double vy = q.y() - p1.y();

	// Multiple of the directional vector (p1p2) for coordinates of point "q"
	double m = (q.x() - p1.x()) / ux;

	// Cross product
	double t = ux * vy - uy * vx;

	// Point in the left half plane
	if (t > eps)
		return 1;
	// Point in the right half plane
	if (t < -eps)
		return 0;
	// Point on the edge
	if (t >= -eps and t <= eps and m >= 0 and m <= 1)
		return -1;
	// Point on the line but not on the edge
	return -2;
}

double Algorithms::getAngle2Vectors(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4)
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

int Algorithms::positionPointPolygonWinding(QPoint &q, std::vector<QPoint> &pol)
{
	// Analyze Position of the Point and the Polygon by Winding method

	// Winding number
	double wn = 0.0;

	// Tolerance
	double eps = 1.0e-6;

	// The size of polygon
	unsigned long long n = pol.size();

	// Browse all points of polygon
	for (unsigned int i = 0; i < n; i++)
	{

		// Measure angle
		double omega = getAngle2Vectors(pol[i], q, pol[(i+1)%n], q);

		// Get orientation of the point and the polygon edge
		int orient = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

		// Point on the edge
		if (orient == -1)
			return -1;

		// Point on the line but not on the edge
		if (orient == -2)
			return 0;

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

int Algorithms::positionPointPolygonRayCrossing(QPoint &q, std::vector<QPoint> &pol)
{
	// Analyze Position of the Point and the Polygon by method Ray Crossing

	// Amount of intersections on the right half plane
	int k = 0;

	// Amount of intersection on the left half plane
	int l = 0;

	// Size of polygon
	unsigned long long n = pol.size();

	// Tolerance
	double eps = 1.0e-6;

	// Reduce first point
	double xir = pol[0].x() - q.x();
	double yir = pol[0].y() - q.y();

	for (unsigned int i = 1; i < n+1 ; i++)
	{
		// Reduce coordinates
		double xiir = pol[i%n].x() - q.x();
		double yiir = pol[i%n].y() - q.y();

		// Edge is crossing or touching the ray
		if (((yir > 0) and (yiir <= 0)) or ((yiir > 0) and (yir <= 0)))
		{

			// Compute intersection
			double xm = (xir * yiir - xiir * yir) / (yir - yiir);

			// Point in the right half plane
			if (xm > -eps)
				k += 1;
			// Point in the left half plane
			if (xm < eps)
				l += 1;
		}

		// Assign coordinates
		yir = yiir;
		xir = xiir;
	}

	// Point on the edge
	if (k%2 > l%2 or l%2 > k%2)
		return -1;

	// Odd/even amount of intersections
	return k%2;
}
