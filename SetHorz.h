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

// Send USB calls to the PIC mircoprocessor

#ifndef SET_HORZ_H
#define SET_HORZ_H

#define LASER_INTENSITY_RANGE 0.25   // inputs to PIC for laser:  60 yields 0 volts, 170 yields 2.44 volts which is just enough to turn the laser on,
                                       // 195 yields 3 volts, so range is from 170-195 which spans 25, so the factor here is 0.25 (it will be multiplied by 0-100 to yield 0-25)
                                       // NOTE!! values outside the range 60 - 195
                                       // will produce negative voltages or voltages greater than 3 V, which can damage laser


#define LASER_MIN_VALUE_TO_TURN_ON 160 // upon further use, the value where the laser turns on seems to vary a bit.  For now, 160 is a good number
#define LASER_ZERO_VOLTS_VALUE 60
#define DEFAULT_LASER_INTENSITY 30

int SetHorz(int horz);
int SetServo(int servo);
int SendLaserIntensity(int intensity);
void SetLaserIntensity(int intensity);
int GetLaserIntensity();
int SetUSB(int horz, unsigned char op);

#endif
