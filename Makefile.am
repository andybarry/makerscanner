##Process this file with automake to create Makefile.in

bin_PROGRAMS = makerscanner
makerscanner_SOURCES = ActiveStereoApp.cpp CaptureThread.h ScanStatus.h ActiveStereoApp.h DistanceToReferenceDialog.cpp ScanThread.cpp ActiveStereoMain.cpp DistanceToReferenceDialog.h ScanThread.h ActiveStereoMain.h IndividualPixels.h Cameras.cpp PointCloud.cpp Cameras.h PointCloud.h camview.cpp PointCloudPoint.cpp camview.h PointCloudPoint.h SetHorz.h CaptureThread.cpp ScanStatus.cpp

EXTRA_DIST = laserIcon.png
pixmapdir = $(datadir)/pixmaps
pixmap_DATA = laserIcon.png



##AM_CPPFLAGS = $(DEPS_CFLAGS) -DPIXMAPS_DIR=\""$(datadir)/pixmaps"\"

## Add the OpenCV flags that we got from PKG_CHECK_MODULES([OPENCV], [opencv >= 2.0]) in configure.ac
AM_CPPFLAGS = $(OPENCV_CFLAGS) $(DEPS_CFLAGS) -DPIXMAPS_DIR=\""$(datadir)/pixmaps"\"

## Add the OpenCV linking libraries that we got from PKG_CHECK_MODULES([OPENCV], [opencv >= 2.0]) in configure.ac
makerscanner_LDADD = $(OPENCV_LIBS)
##libfoo_la_LDFLAGS = -version-info @version_info@

