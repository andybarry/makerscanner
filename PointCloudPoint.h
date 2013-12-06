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

#ifndef POINT_CLOUD_POINT_H
#define POINT_CLOUD_POINT_H

#include <wx/string.h>
#include <math.h>

/*
* Holds a point cloud point and can generate a .ply point cloud string (one line of the file) for this point.
*/

class PointCloudPoint
{
	public:
		PointCloudPoint(double distIn, double thetaIn, double phiIn, int rIn, int gIn, int bIn, int wIn, int hIn, int laserPosIn)
		{
			dist = distIn;
			theta = thetaIn;
			phi = phiIn;

			r = rIn;
			g = gIn;
			b = bIn;
			w = wIn;
			h = hIn;
			laserPosition = laserPosIn;
		}
		PointCloudPoint(PointCloudPoint *point);

		// Generates the point cloud string for this point (one line)
		wxString GetPlyString();

		double dist, theta, phi;
		int r, g, b, w, h, laserPosition;
};

#endif
