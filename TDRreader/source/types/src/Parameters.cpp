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

#include "Parameters.h"

#include <fstream>

Parameters::Parameters(){ }

Parameters::~Parameters(){ }

bool Parameters::next_line(std::istream &in, std::string &cmd_line)
{
    cmd_line = "";
    std::string line;
    while ( getline(in, line) ){
        size_t ls = line.size();
        if (ls == 0)
            break;
        else if ( line[ls-1] != '\\' ){
            cmd_line += line;
            break;
        } else
            cmd_line += line.substr(0, ls-1);
    }
    return in || !cmd_line.empty();
}
