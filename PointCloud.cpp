/*
* Copyright 2009-2010, Andrew Barry
*
* This file is part of MakerScanner.
*
* MakerScanner is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License (Version 2, June 1991) as published by
* the Free Software Foundation.
*
* MakerScanner is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PointCloud.h"

#include <wx/listimpl.cpp>

// define a linked list of PointCloudPoints
WX_DEFINE_LIST(ListOfCloudPoints);

// PointCloud holds PointCloudPoints which are individual lines in a pointcloud file
// Points are identified by height in the image and by laser position
PointCloud::PointCloud()
{
	cloudMap = new CloudMap();
	numberOfPoints = 0;
}

// Destructor -- clear the hash map and delete all PointCloudPoints
PointCloud::~PointCloud()
{

	// empty the hash map

	CloudMap::iterator it;

	for( it = cloudMap->begin(); it != cloudMap->end(); ++it )
	{
		delete it->second;
	}

	delete cloudMap;
}

// The points are uniquely identified by h and laserPos.
// Index is -- laserPos * 1000 + h yielding an set size of 255*1000 + 1000 = 256,000
// Each hash map container holds a linked list of PointCloudPoints, which can be combined in an average
// for that point
int PointCloud::GetKey(int h, int laserPos)
{
	return laserPos * 1000 + h;
}

// Helper function to add a point that creates the PointCloudPoint for you
void PointCloud::AddPoint(double dist, double theta, double phi, int r, int g, int b, int w, int h, int laserPos)
{
	// make a new point
	PointCloudPoint *newPoint = new PointCloudPoint(dist, theta, phi, r, g, b, w, h, laserPos);
	AddPoint(newPoint);
}

// Add a point to the point cloud.  If there is not already a point at this h, and laserPos, make
// a new linked list.  Otherwise, add to the linked list that we already have for this location
void PointCloud::AddPoint(PointCloudPoint *point)
{
	// first, get the list
	ListOfCloudPoints *thisList;

	if (cloudMap->find(GetKey(point->h, point->laserPosition)) == cloudMap->end())
	{
		// this is a new point, create a new list
		thisList = new ListOfCloudPoints();
		(*cloudMap)[GetKey(point->h, point->laserPosition)] = thisList;
		numberOfPoints ++;

	} else {
		thisList = (*cloudMap)[GetKey(point->h, point->laserPosition)];
	}

	// add the point to the list

	thisList->Append(point);


}

// Return a .ply file containing the entire point cloud (large string!)
wxString PointCloud::GetPointCloudPly()
{
	wxString cloudString = wxT("");

	wxString headerString = wxT("ply");
	headerString += wxT("\nformat ascii 1.0");

	headerString += wxT("\nelement vertex ");
	headerString << numberOfPoints;

	headerString += wxT("\nproperty float x");
	headerString += wxT("\nproperty float y");
	headerString += wxT("\nproperty float z");
	headerString += wxT("\nproperty uchar diffuse_red");
	headerString += wxT("\nproperty uchar diffuse_green");
	headerString += wxT("\nproperty uchar diffuse_blue");
	headerString += wxT("\nelement face 0");
	headerString += wxT("\nproperty list uchar int vertex_indices");
	headerString += wxT("\nend_header");

	CloudMap::iterator it;
	ListOfCloudPoints *thisList;

	for( it = cloudMap->begin(); it != cloudMap->end(); ++it )
	{
		thisList = it->second;

		PointCloudPoint thisPoint = AverageList(thisList);

		cloudString += wxT("\n") + thisPoint.GetPlyString();

	}

	return headerString + cloudString;
}

// average a linked list of points to handle a case where we have more than one point per h, laserPos
PointCloudPoint PointCloud::AverageList(ListOfCloudPoints *thisList)
{
	double number = double(thisList->GetCount());

	double sumDist = 0;

	PointCloudPoint *point = NULL;

	// iterate over the list and average all values
	for ( ListOfCloudPoints::Node *node = thisList->GetFirst(); node; node = node->GetNext() )
	{

		point = node->GetData();
		sumDist += point->dist;
	}

	if (point == NULL)
	{
		return PointCloudPoint(0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	return PointCloudPoint(sumDist/number, point->theta, point->phi, point->r, point->g, point->b, point->w, point->h, point->laserPosition);

}
