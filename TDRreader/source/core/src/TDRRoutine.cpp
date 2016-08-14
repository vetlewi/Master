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

/*!
 * \file TDRRoutine.cpp
 * \brief Implementation of TDRRoutine.
 * \author Vetle W. Ingeberg
 * \version 0.8.0
 * \date 2015-2016
 * \copyright GNU Public License v. 3
 */


#include "TDRRoutine.h"

#include "Event.h"
#include "Histogram1D.h"
#include "Histogram2D.h"

#include <sstream>
#include <iostream>


TDRRoutine::TDRRoutine() { }

bool TDRRoutine::Start()
{
	CreateSpectra();
	return true;
}

bool TDRRoutine::End()
{
	return true;
}

bool TDRRoutine::Command(const std::string& cmd)
{
    std::istringstream icmd(cmd.c_str());

    std::string name, tmp;

    icmd >> name;

    if (name == "gain"){
        icmd >> tmp;
        std::string filename;
        icmd >> filename;
        if (tmp == "file"){
            gain_param = std::unique_ptr<GainParameters>(new GainParameters());
            if (gain_param->setAll(filename))
                return true;
        } else {
            std::cerr << "Gain file: Error reading '" << filename << "'\n";
            return false;
        }
    } else if (name == "telescope") {
        icmd >> tmp;
        if (tmp == "gate"){
            char tel;
            icmd >> tel;
            std::string gate;
            icmd >> gate;
            Polynomial up, down;
            if (gate == "upper" || gate == "UPPER"){
                int n;
                icmd >> n;
                double *vals = new double[n];
                for (int i = 0 ; i < n ; ++i)
                    icmd >> vals[i];
                up = Polynomial(vals, n);
                delete[] vals;
            }
            icmd >> gate;
            if (gate == "lower" || gate == "LOWER"){
                int n;
                icmd >> n;
                double *vals = new double[n];
                for (int i = 0 ; i < n ; ++i)
                    icmd >> vals[i];
                down = Polynomial(vals, n);
                delete[] vals;
            }
            if (tel == 'A'){
                telGateA.reset(new TelescopeGate(up, down));
                return true;
            } else if (tel == 'B'){
                telGateB.reset(new TelescopeGate(up, down));
                return true;
            } else {
                std::cerr << "TDRRoutine: Unknown command '" << cmd << "'\n";
                return false;
            }
        } else if (tmp == "strips" || tmp == "STRIPS"){
            char tel;
            icmd >> tel;
            std::string vFilename, hFilename;
            icmd >> vFilename;
            icmd >> hFilename;
            if (tel == 'A'){
                kinA.reset(new Telescope(vFilename, hFilename));
                return true;
            } else if (tel == 'B'){
                kinB.reset(new Telescope(vFilename, hFilename));
                return true;
            } else {
                std::cerr << "TDRRoutine: Unknown command '" << cmd << "'\n";
                return false;
            }
        } else if (tmp == "excitation" || tmp == "EXCITATION"){
            char tel;
            icmd >> tel;
            std::string fname;
            icmd >> fname;
            if ( tel == 'A' ){
                return kinA->SetExcitation(fname);
            } else if ( tel == 'B' ){
                return kinB->SetExcitation(fname);
            } else {
                std::cerr << "TDRRoutine: Unknown command '" << cmd << "'\n";
                return false;
            }
        } else if (tmp == "beta" || tmp == "BETA"){
            char tel;
            icmd >> tel;
            double beta;
            icmd >> beta;
            if (tel == 'A'){
                kinA->SetBeta(beta);
                return true;
            } else if (tel == 'B'){
                kinB->SetBeta(beta);
                return true;
            } else {
                std::cerr << "TDRRoutine: Unknown command '" << cmd << "'\n";
                return false;
            }
        }
        std::cerr << "TDRRoutine: Unknown command '" << cmd << "'\n";
        return false;
    } else if (name == "TimeGateMain"){
        char tel;
        icmd >> tel;
        std::string fname;
        icmd >> fname;
        if ( tel == 'A' ){
            tGateA_main.reset(new TimeGate(fname));
            return true;
        } else if ( tel == 'B' ){
            tGateB_main.reset(new TimeGate(fname));
            return true;
        } else {
            std::cerr << "TDRRoutine: Unknown command '" << cmd << "'\n";
            return false;
        }
    } else if (name == "TimeGateSec") {
        char tel;
        icmd >> tel;
        std::string fname;
        icmd >> fname;
        if ( tel == 'A' ){
            tGateA_sec.reset(new TimeGate(fname));
            return true;
        } else if ( tel == 'B' ){
            tGateB_sec.reset(new TimeGate(fname));
            return true;
        }
    } else if (name == "TimeGateBG") {
        char tel;
        icmd >> tel;
        std::string fname;
        icmd >> fname;
        if ( tel == 'A' ){
            tGateA_bg.reset(new TimeGate(fname));
            return true;
        } else if ( tel == 'B' ){
            tGateB_bg.reset(new TimeGate(fname));
            return true;
        } else {
            std::cerr << "TDRRoutine: Unknown command '" << cmd << "'\n";
            return false;
        }
    } else {
        //std::cerr << "TDRRoutine: Unknown command '" << cmd << "'\n";
        return UserCommand(cmd);
    }
    return false;
}


