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

#ifndef USERSORT_H
#define USERSORT_H

#include "TDRRoutine.h"
#include "Event.h"

class TTree;
class TFile;

class UserSort : public TDRRoutine
{
public:
    UserSort() { }
    ~UserSort() { }

    bool Sort(const Event& event);

    bool End();

    //! We have no user commands that needs to be set.
    /*! \return true allways.
     */
    bool UserCommand(const std::string &cmd);

protected:
    void CreateSpectra();

private:
    /*! Some arrays and stuff we are going to use.
     * Allocating them at creation of the class to minimize the
     * CPU time used to allocate these variables.
     */

    //! Number of LaBr3(Ce) detector 1 words in the event.
    int nL1;

    //! Number of LaBr3(Ce) detector 2 words in the event.
    int nL2;

    //! Number of words from each of the 8 clover detectors.
    int nC[8];

    //! Number of words from the different vert. strips in telescope A.
    int nXA;

    //! Number of words from the different hor. strips in telescope A.
    int nYA;

    //! Number of words from the different back strips in telescope A.
    int nEA;

    //! Number of words from the different vert. strips in telescope B.
    int nXB;

    //! Number of words from the different hor. strips in telescope B.
    int nYB;

    //! Number of words from the different back strips in telescope B.
    int nEB;

    //! Array containing the data words from LaBr3(Ce) detector 1.
    word_t labr1[256];

    //! Array containing the data words from LaBr3(Ce) detector 1.
    word_t labr2[256];

    //! 2-D Array containing all the data words from the Clover detectors.
    word_t clover[8][256];

    //! 2-D Array to populate with all data words from vert. strips, tel. A.
    word_t dXA[256];

    //! 2-D Array to populate with all data words from hor. strips, tel. A.
    word_t dYA[256];

    //! 2-D Array to populate with all data words from back strips, tel A.
    word_t dEA[256];

    //! 2-D Array to populate with all data words from vert. strips, tel. B.
    word_t dXB[256];

    //! 2-D Array to populate with all data words from hor. strips, tel. B.
    word_t dYB[256];

    //! 2-D Array to populate with all data words from back strips, tel B.
    word_t dEB[256];

    //! Excitation energy histogram, telescope A.
    Histogram1Dp h_ex_A;

    //! Excitation energy histogram, telescope B.
    Histogram1Dp h_ex_B;

    //! Excitation energy histogram, both.
    Histogram1Dp h_ex;

    //! Excitation energy histogram, coincidence, by angle.
    Histogram1Dp h_ex_angA[256], h_ex_angB[256], h_ex_ang[256];

    //! Excitation energy histogram, coincidence.
    Histogram1Dp h_ex_coin, h_ex_coinA, h_ex_coinB;

    //! Excitation energy histogram, coincidence, by angle.
    Histogram1Dp h_ex_coin_ang[256];

    //! Multiplicity histograms.
    Histogram1Dp n_Ax, n_Ay, n_AE;
    Histogram1Dp n_Bx, n_By, n_BE;

    //! Energy - time spectra for the particle telescopes.
    Histogram2Dp m_E_t_X_A[16], m_E_t_Y_A[16], m_E_t_A[16];
    Histogram2Dp m_E_t_X_B[16], m_E_t_Y_B[16], m_E_t_B[16];

    //! Energy - time spectra for the gamma detectors.
    Histogram2Dp m_E_t_LaBr_A[2], m_E_t_LaBr_B[2]; // LaBr spectras.
    Histogram2Dp m_E_t_clover_A[8][4], m_E_t_clover_B[8][4]; // Clover spectras.

    //! Particle spectras. Only one word per horizontal, vertical and back strip allowed.
    Histogram2Dp m_e_de_A, m_e_de_B;
    Histogram2Dp m_e_de_A_gate, m_e_de_B_gate;

    //! Particle spectra, by angle.
    Histogram2Dp m_e_de_A_ang[16][16];
    Histogram2Dp m_e_de_B_ang[16][16];

    //! Correlation between two words.
    Histogram2Dp m_corr_AX, m_corr_AY;
    Histogram2Dp m_corr_BX, m_corr_BY;
    Histogram2Dp m_corr_Ab, m_corr_Bb;

    //! Total spectra for detectors (before time gates & particle gates)
    Histogram2Dp m_E_t_clover_tot[8][4];
    Histogram2Dp m_E_t_LaBr_tot[2];

    //! Gamma - excitation matrices.
    Histogram2Dp alfna_clover;
    Histogram2Dp alfna_labr;

    //! Doppler corrected gamma - excitation matrice.
    Histogram2Dp alfna_clover_dop;
    Histogram2Dp alfna_labr_dop;

    //! Spectrum of the subtracted background.
    Histogram1Dp h_Clover_bg;
    Histogram1Dp h_LaBr_bg;

    //! Matrix with energy spectra of the different detectors.
    Histogram2Dp m_detectors_E;

    //! Matrix with Doppler corrected energy spectra of the different detectors.
    Histogram2Dp m_detectors_E_dop;

    //! Angle vs. particle energy, both telescopes.
    Histogram2Dp m_E_ang;

    //! Angle vs. particle energy, telescope A.
    Histogram2Dp m_E_ang_A;

    //! Angle vs. particle energy, telescope B.
    Histogram2Dp m_E_ang_B;

    //! Angle vs. particle energy, coincidence, tel A.
    Histogram2Dp m_E_ang_A_coin;

    //! Angle vs. particle energy, coincidence, tel B.
    Histogram2Dp m_E_ang_B_coin;

    //! Angle vs. particle energy, angles.
    Histogram2Dp m_E_ang_corr;

    //! Max. particle energy for telescope A.
    double maxEA;

    //! Max. particle energy for telescope B.
    double maxEB;

    //! Variables used to show some statistics when the program is finished.
    int nEvents, nWords, ndEw;
    double avrDiff_dE;
    int64_t avrTimediff, avrSq;
    int nClover, nDiscarded;
    int nCoincidence, nCoLaBr, nCoClover;
    int nBg, nBgLaBr, nBgClover;

};

#endif // USERROUTINE2_H
