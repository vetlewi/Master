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

#ifndef USERSINGLES_H
#define USERSINGLES_H

#include "TDRRoutine.h"

struct Event;

class TTree;
class TFile;
class TBranch;

class UserSingles : public TDRRoutine
{
public:
    UserSingles() : makeTree( false ), doppler( false ) { }
    ~UserSingles() {}

	bool SortSingles(const Event& event);

    bool End();

    //! Command for making ROOT tree.
    /*! \return true if valid command, false otherwise.
     */
    bool UserCommand(const std::string &cmd);

protected:
	void CreateSpectra();

private:
    struct clover_event
    {
        int nA;
        uint16_t adcdataA[256];
        int64_t timestampA[256];
        double e_calA[256];

        int nB;
        uint16_t adcdataB[256];
        int64_t timestampB[256];
        double e_calB[256];

        int nC;
        uint16_t adcdataC[256];
        int64_t timestampC[256];
        double e_calC[256];

        int nD;
        uint16_t adcdataD[256];
        int64_t timestampD[256];
        double e_calD[256];

    };

    //! Particle telescopes.
    Histogram1Dp h_DSSD_A_dX[16];
	Histogram1Dp h_DSSD_A_dY[16];
	Histogram1Dp h_DSSD_A_E[16];
    Histogram1Dp h_DSSD_B_dX[16];
    Histogram1Dp h_DSSD_B_dY[16];
    Histogram1Dp h_DSSD_B_E[16];

    //! Particle telescopes (uncalibrated).
    Histogram1Dp h_DSSD_A_dX_raw[16];
    Histogram1Dp h_DSSD_A_dY_raw[16];
    Histogram1Dp h_DSSD_A_E_raw[16];
    Histogram1Dp h_DSSD_B_dX_raw[16];
    Histogram1Dp h_DSSD_B_dY_raw[16];
    Histogram1Dp h_DSSD_B_E_raw[16];

    //! Clover detectors.
    Histogram1Dp h_Clover_E[8]; // Each detector with add-back.
    Histogram1Dp h_Clover_crystal[8][4]; // Each crystal withought add-back.
    Histogram1Dp h_Clover_crystal_raw[8][4]; // Raw (uncalibrated) spectra.

    //! LaBr detectors.
    Histogram1Dp h_LaBr_E[2];
    Histogram1Dp h_LaBr_E_raw[2];

    //! All gamma detectors in one histogram:
    Histogram2Dp m_Gamma_E;

    //! All particle detectors in one histogram:
    Histogram2Dp m_Particle_E;

    bool makeTree;

    bool doppler;
    double beta;
    double gamma;

    inline double Dop(double Eg, double omg, double sig){
        return gamma*Eg*(1 - beta*sin(omg*3.14159265359/180.)*sin(sig*3.14159265359/180.));
    }


    TTree *tree;
    TFile *file;

    clover_event cEv[8];
};

#endif // USERSSINGLES_H
