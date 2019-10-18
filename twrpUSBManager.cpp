/*
	Copyright 2013 to 2019 TeamWin
	This file is part of TWRP/TeamWin Recovery Project.

	TWRP is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	TWRP is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with TWRP.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "twcommon.h"
#include "twrpUSBManager.hpp"


int twrpUSBManager::initDeviceScanner() {
    libusb_device **devs;
    int ret =  libusb_init(nullptr);
    if (ret != 0) {
        LOGINFO("Error initializing USB scanning\n");
        return ret;
    }   

    size_t count = libusb_get_device_list(nullptr, &devs);
    if (count < 0) {
        LOGERR("Error getting USB device list\n");
        return (int) count;
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(nullptr);
    return 0;
}

int twrpUSBManager::getInfo(libusb_device *dev) {
    struct libusb_device_descriptor desc;
    libusb_device_handle *handle = NULL;
    uint8_t string[256];
    char description[260];

    int ret = libusb_get_device_descriptor(dev, &desc);
    if (ret < 0) {
        LOGERR("Error getting device descriptor\n");
    }

    ret = libusb_open(dev, &handle);

    if (LIBUSB_SUCCESS == ret) {
        if (desc.iManufacturer) {
			ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, string, sizeof(string));
			if (ret > 0)
				snprintf(description, sizeof(description), "%s - ", string);
			else
				snprintf(description, sizeof(description), "%04X - ", desc.idVendor);
		}
    }
    return ret;
}