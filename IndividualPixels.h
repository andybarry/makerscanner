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

#ifndef INDIVIDUAL_PIXELS
#define INDIVIDUAL_PIXELS

#ifndef OPENCV_INCLUDES
#define OPENCV_INCLUDES
#include "cv.h"
#include "highgui.h"
#endif // OPENCV_INCLUDES

/*
* Standard code for getting individual pixel access -- RgbImage, BwImage, RgbImage Float, and BwImageFloat
* Nothing especailly novel here...
*/

template<class T> class Pixels
{
  private:
  IplImage* imgp;
  public:
  Pixels(IplImage* img=0) {imgp=img;}
  ~Pixels(){imgp=0;}
  void operator=(IplImage* img) {imgp=img;}
  inline T* operator[](const int rowIndx) {
    return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};

typedef struct{
  unsigned char b,g,r;
} RgbPixel;

typedef struct{
  float b,g,r;
} RgbPixelFloat;

typedef Pixels<RgbPixel>       RgbImage;
typedef Pixels<RgbPixelFloat>  RgbImageFloat;
typedef Pixels<unsigned char>  BwImage;
typedef Pixels<float>          BwImageFloat;

#endif // INDIVIDUAL_PIXELS

/*  How to use this class
// refer to individual pixels as shown below.
// when they are changed in the class's image, they change in the parameter image as well.
// http://www.cs.iit.edu/~agam/cs512/lect-notes/opencv-intro/opencv-intro.html#SECTION00053000000000000000
# For a single-channel byte image:

IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
BwImage imgA(img);
imgA[i][j] = 111;

# For a multi-channel byte image:

IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
RgbImage  imgA(img);
imgA[i][j].b = 111;
imgA[i][j].g = 111;
imgA[i][j].r = 111;

# For a multi-channel float image:

IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,3);
RgbImageFloat imgA(img);
imgA[i][j].b = 111;
imgA[i][j].g = 111;
imgA[i][j].r = 111;

*/
