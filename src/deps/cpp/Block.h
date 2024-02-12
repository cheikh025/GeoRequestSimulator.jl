#ifndef BLOCK_H
#define BLOCK_H

#include "Point.h"
#include <vector>
#include <set>
#include <string>
#include <map>
#include <algorithm>
#include <math.h>
using namespace std;

class Block
{
public:
	
	int id;												// ID of the Block
	int nbInhabitants ;									// Number of inhabitants
	
	vector <Point> verticesPoints;						// Copy of the points that delimit the Block
	
	double minX = 1.e30;								// Minimum X coordinate of the vertices of the Block
	double maxX = -1.e30;								// Maximum X coordinate of the vertices of the Block
	double minY = 1.e30;								// Minimum Y coordinate of the vertices of the Block
	double maxY = -1.e30;								// Maximum Y coordinate of the vertices of the Block

	
	vector<vector<Point>> trainScenarios;				// Scenarios for each target size of distrcit

	// Distance of a Point to the closest Point in the Block
	double distance(const Point & p1);
};


#endif
