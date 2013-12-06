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

#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include "PointCloudPoint.h"

#include <wx/string.h>
#include <wx/list.h>
#include <wx/hashmap.h>

WX_DECLARE_LIST(PointCloudPoint, ListOfCloudPoints);

WX_DECLARE_HASH_MAP( int, ListOfCloudPoints*, wxIntegerHash, wxIntegerEqual, CloudMap );

/*
* This class holds point cloud data that can be fitlered and printed to a .ply file
*/

class PointCloud
{
	public:
		PointCloud();
		~PointCloud();
		void AddPoint(double dist, double theta, double phi, int r, int g, int b, int w, int h, int laserPos);
		void AddPoint(PointCloudPoint *point);
		wxString GetPointCloudPly();

	private:
		// The points are uniquely identified by h and laserPos.
		// Index is -- laserPos * 1000 + h yielding an set size of 255*1000 + 1000 = 256,000
		// Each hash map container holds a linked list of PointCloudPoints, which can be combined in an average
		// for that point

		int GetKey(int h, int laserPos);
		PointCloudPoint AverageList(ListOfCloudPoints *thisList);

		CloudMap *cloudMap;
		int numberOfPoints;




};


#endif
