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

#include "GainParameters.h"

#include <fstream>
#include <sstream>

GainParameter GainParameters::Find(const uint16_t &address){
    params_t::iterator it = m_params.find(address);
    if ( it != m_params.end() )
        return it->second;
    else
        return GainParameter();
}

void GainParameters::Add(const uint16_t &address, GainParameter param)
{
    m_params[address] = param;
}

void GainParameters::Remove(const uint16_t &address)
{
    params_t::iterator it = m_params.find(address);
    if (it != m_params.end())
        m_params.erase( it );
}

bool GainParameters::setAll(const std::string &filename)
{
    std::ifstream input_file(filename.c_str());

    std::string line;

    while ( next_line(input_file, line) ){
        if (line.size() == 0 || line[0] == '#') continue;
        std::istringstream icmd(line.c_str());
        uint16_t addr;
        double energy[5];
        icmd >> addr;
        for (int i = 0 ; i < 5 ; i++) icmd >> energy[i];
        Add(addr, GainParameter(EnergyParameter(energy,5),TimeParameter()));
    }
    return true;
}
