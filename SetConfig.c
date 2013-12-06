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

#include <stdio.h>
#include <usb.h>

#include "SetConfig.h"

#define FALSE       0
#define TRUE        1

// return codes
// 0: success
// -1: No matching device found
// -2: Unable to send SET_CONFIGURATION request

int SetConfig() {
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
    if (udev) {
        ret = usb_control_msg(udev, USB_TYPE_STANDARD | USB_RECIP_DEVICE, USB_REQ_SET_CONFIGURATION, 1, 0, buffer, 0, 100);
        if (ret < 0) {
            printf("Unable to send SET_CONFIGURATION standard request, ret = %d...\n", ret);
            return -2;
        }
        usb_close(udev);
    }
    return 0;
}
