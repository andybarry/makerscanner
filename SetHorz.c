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

/*
* Standard USB calls to the PIC.  This comes from http://ece.olin.edu/poe/
*/

#include <usb.h>
#include <stdio.h>

#include "SetHorz.h"

#define FALSE       0
#define TRUE        1

#define SET_DUTY    0x04
#define SERVO_POS	0x05
#define LASER_INTENSITY 0x06

#define HORIZ_MIN 0
#define HORIZ_MAX 255

/*
* Return codes:
*	 0 = success
*	-1 = no matching device
*	-2 = unable to send vendor request
*	-3 = illegal offset
*/

int LaserIntensity = DEFAULT_LASER_INTENSITY;

// Send the USB call for moving the mirror
int SetHorz(int horz)
{
	horz = HORIZ_MAX - horz;
	if ((horz < HORIZ_MIN) || (horz > HORIZ_MAX)) {
        printf("Horizontal laser offset specified, (%d), is out of range (0 - 155).\n", horz);
		return -3;
    }
	return SetUSB(horz, SET_DUTY);
}

// Move the servo
int SetServo(int servo)
{
	if (servo < 50 || servo > 150) {
        printf("Illegal offset (%d) specified.\n", servo);
		return -3;
    }
	return SetUSB(servo, SERVO_POS);
}

// Change the laser intensity
int SendLaserIntensity(int intensity)
{
  if (intensity < 0 || intensity > 100)
  {
      printf("Intensity specified, (%d), is out of range (0-100).\n", intensity);
      return -3;
   }
   if (intensity == 0)  intensity =  LASER_ZERO_VOLTS_VALUE;
   else intensity =  (int) (   (((float) intensity) * (float) LASER_INTENSITY_RANGE) + (float) LASER_MIN_VALUE_TO_TURN_ON + 0.5);

   return SetUSB(intensity, LASER_INTENSITY);
}

void SetLaserIntensity(int intensity) { LaserIntensity = intensity; }

int GetLaserIntensity() { return LaserIntensity; }

// USB call to PIC
int SetUSB(int horz, unsigned char op)
{
    struct usb_bus *bus;
    struct usb_device *dev;
    usb_dev_handle *udev;
    int dev_found, ret;
    char buffer[8];

    usb_init();

    usb_find_busses();
    usb_find_devices();

    udev = NULL;
    dev_found = FALSE;
    for (bus = usb_get_busses(); bus && !dev_found; bus = bus->next) {
        for (dev = bus->devices; dev && !dev_found; dev = dev->next) {
            if ((dev->descriptor.idVendor == 0x6666) && (dev->descriptor.idProduct == 0x0003)) {
                dev_found = TRUE;
                udev = usb_open(dev);
            }
        }
    }
    if (!dev_found) {
        printf("No matching device found...\n");
		return -1;
    }

        ret = usb_control_msg(udev, 0x40, op, horz, 0, buffer, 0, 100);
      if (udev) {
        if (ret < 0) {
            printf("Unable to send SET_DUTY vendor request, ret = %d...\n", ret);
            return -2;
        }
        //usb_close(udev);
    }
    return 0;
}
