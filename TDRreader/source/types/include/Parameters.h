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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iosfwd>

class Parameters
{
public:
    //! Empty constructor.
    Parameters();

    //! Construct from file.
    Parameters(const std::string &filename);

    //! Virtual destructor.
    virtual ~Parameters();

    //! Set mapping from file.
    virtual bool setAll(const std::string &filename) = 0;

protected:
    //! Helper function for the reading from file.
    bool next_line(std::istream &in, std::string &cmd_line);

};

#endif // PARAMETERS_H
