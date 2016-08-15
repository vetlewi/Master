/*******************************************************************************
 * Copyright (C) 2016 Vetle W. Ingeberg                                        *
 * Author: Vetle Wegner Ingeberg, v.w.ingeberg@fys.uio.no                      *
 *                                                                             *
 * --------------------------------------------------------------------------- *
 * This program is free software; you can redistribute it and/or modify it     *
 * under the terms of the GNU General Public License as published by the       *
 * Free Software Foundation; either version 3 of the license, or (at your      *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but         *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                  *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General   *
 * Public License for more details.                                            *
 *                                                                             *
 * You should have recived a copy of the GNU General Public License along with *
 * the program. If not, see <http://www.gnu.org/licenses/>.                    *
 *                                                                             *
 *******************************************************************************/

/*! \mainpage TDRreader
 * This is the automatically created documentation for TDRreader. To explore the
 * software I would suggest to start with the Sort function in the UserSort class.
 */

#include "TDRRoutine.h"
#include "OfflineSorting.h"

#include <iostream>

#include "UserSingles.h"
#include "UserSort.h"

int main(int argc, char* argv[])
{
#ifdef SINGLES
    return OfflineSorting::Run(new UserSingles(), argc, argv);
#else
    return OfflineSorting::Run(new UserSort(), argc, argv);
#endif
}
