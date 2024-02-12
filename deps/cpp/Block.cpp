#include "Block.h"

// Distance of a Point to the closest Point in the Block
double Block::distance(const Point & p1)
{
	double minDistance = 1.e30;
	for (int i2 = 0; i2 < verticesPoints.size(); i2++)
	{
		Point & p2 = verticesPoints[i2];
		Point & p3 = verticesPoints[(i2 + 1) % verticesPoints.size()];
		double dist = Point::distance(p1, p2, p3);
		if (dist < minDistance) minDistance = dist;
	}
	return minDistance;
};


