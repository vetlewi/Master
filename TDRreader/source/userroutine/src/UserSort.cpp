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

#include "UserSort.h"

#include "DefineFile.h"

#include "TDRWordBuffer.h"
#include "DetectorRead.h"
#include "Histogram1D.h"
#include "Histogram2D.h"
#include "Histograms.h"
#include "Event.h"

#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>

bool UserSort::UserCommand(const std::string &cmd){

    std::stringstream icmd(cmd.c_str());

    std::string command;
    icmd >> command;

    if (command == "MaxEA"){
        icmd >> maxEA;
        return true;
    } else if (command == "MaxEB"){
        icmd >> maxEB;
        return true;
    } else {
        return false;
    }
}

void UserSort::CreateSpectra()
{
    for (int i = 0 ; i < 16 ; ++i){
        if (i < 9){
            m_E_t_X_A[i] = Mat("m_E_t_X_A_0"+std::to_string(i+1), "Energy - time, vertical strips, telescope A", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_Y_A[i] = Mat("m_E_t_Y_A_0"+std::to_string(i+1), "Energy - time, horizontal strips, telescope A", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_A[i] = Mat("m_E_t_A_0"+std::to_string(i+1), "Energy - time, back strips, telescope A", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");

            m_E_t_X_B[i] = Mat("m_E_t_X_B_0"+std::to_string(i+1), "Energy - time, vertical strips, telescope B", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_Y_B[i] = Mat("m_E_t_Y_B_0"+std::to_string(i+1), "Energy - time, horizontal strips, telescope B", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_B[i] = Mat("m_E_t_B_0"+std::to_string(i+1), "Energy - time, back strips, telescope B", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
        } else {
            m_E_t_X_A[i] = Mat("m_E_t_X_A_"+std::to_string(i+1), "Energy - time, vertical strips, telescope A", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_Y_A[i] = Mat("m_E_t_Y_A_"+std::to_string(i+1), "Energy - time, horizontal strips, telescope A", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_A[i] = Mat("m_E_t_A_"+std::to_string(i+1), "Energy - time, back strips, telescope A", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");

            m_E_t_X_B[i] = Mat("m_E_t_X_B_"+std::to_string(i+1), "Energy - time, vertical strips, telescope B", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_Y_B[i] = Mat("m_E_t_Y_B_"+std::to_string(i+1), "Energy - time, horizontal strips, telescope B", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_B[i] = Mat("m_E_t_B_"+std::to_string(i+1), "Energy - time, back strips, telescope B", 4096, 0, 16384, "Si energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
        }
    }
    char tmp[] = {'A','B','C','D'};
    for (int i = 0 ; i < 8 ; ++i){
        for (int j = 0 ; j < 4 ; ++j){
            std::string tmp2(1, tmp[j]);
            m_E_t_clover_A[i][j] = Mat("m_E_t_clover_A_"+std::to_string(i+1)+tmp2, "Energy - time, Crystal "+std::to_string(i+1)+tmp2+", telescope A", 4096, 0, 16384, "Clover energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
            m_E_t_clover_B[i][j] = Mat("m_E_t_clover_B_"+std::to_string(i+1)+tmp2, "Energy - time, Crystal "+std::to_string(i+1)+tmp2+", telescope B", 4096, 0, 16384, "Clover energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
        }
    }

    m_E_t_LaBr_A[0] = Mat("m_E_t_A_LaBr1", "Energy - time, LaBr 1, telescope A", 4096, 0, 16384, "LaBr energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
    m_E_t_LaBr_A[1] = Mat("m_E_t_A_LaBr2", "Energy - time, LaBr 2, telescope A", 4096, 0, 16384, "LaBr energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
    m_E_t_LaBr_B[0] = Mat("m_E_t_B_LaBr1", "Energy - time, LaBr 1, telescope B", 4096, 0, 16384, "LaBr energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");
    m_E_t_LaBr_B[1] = Mat("m_E_t_B_LaBr2", "Energy - time, LaBr 2, telescope B", 4096, 0, 16384, "LaBr energy [keV]", MAX_T+MIN_T, -MIN_T*10, MAX_T*10, "Time difference [ns]");


    m_e_de_A = Mat("m_e_de_A", "E energy - dE energy", 4000, 0, 24000, "E detector energy [keV]", 4000, 0, 24000, "#Delta E detector energy [keV]");
    m_e_de_B = Mat("m_e_de_B", "E energy - dE energy", 4000, 0, 24000, "E detector energy [keV]", 4000, 0, 32000, "#Delta E detector energy [keV]");
    m_e_de_A_gate = Mat("m_e_de_A_gate", "E energy - dE energy", 4000, 0, 24000, "E detector energy [keV]", 4000, 0, 24000, "#Delta E detector energy [keV]");
    m_e_de_B_gate = Mat("m_e_de_B_gate", "E energy - dE energy", 4000, 0, 24000, "E detector energy [keV]", 4000, 0, 32000, "#Delta E detector energy [keV]");


    m_corr_AX = Mat("m_corr_AX", "Vertical strips - 2 words correlation", 16, 1, 17, "Vertical strip 1", 16, 1, 17, "Vertical strip 2");
    m_corr_AY = Mat("m_corr_AY", "Horizontal strips - 2 words correlation", 16, 1, 17, "Horizontal strip 1", 16, 1, 17, "Horizontal strip 2");
    m_corr_BX = Mat("m_corr_BX", "Vertical strips - 2 words correlation", 16, 1, 17, "Vertical strip 1", 16, 1, 17, "Vertical strip 2");
    m_corr_BY = Mat("m_corr_BY", "Horizontal strips - 2 words correlation", 16, 1, 17, "Horizontal strip 1", 16, 1, 17, "Horizontal strip 2");

    m_corr_Ab = Mat("m_corr_Ab", "Back strips - 2 words correlation", 16, 1, 17, "Back strip 1", 16, 1, 17, "Back strip 2");
    m_corr_Bb = Mat("m_corr_Bb", "Back strips - 2 words correlation", 16, 1, 17, "Back strip 1", 16, 1, 17, "Back strip 2");

    h_ex_A = Spec("h_ex_A", "Excitation energy", 3000, -2000, 10000, "Calculated excitation energy [keV]");
    h_ex_B = Spec("h_ex_B", "Excitation energy", 3000, -2000, 10000, "Calculated excitation energy [keV]");
    h_ex = Spec("h_ex", "Excitation energy", 3000, -2000, 10000, "Calculated excitation energy [keV]");
    h_ex_coin = Spec("h_ex_coin", "Excitation energy, coinsidence", 3000, -2000, 10000, "Excitation energy [keV]");
    h_ex_coinA = Spec("h_ex_coinA", "Excitation energy, coinsidence", 3000, -2000, 10000, "Excitation energy [keV]");
    h_ex_coinB = Spec("h_ex_coinB", "Excitation energy, coinsidence", 3000, -2000, 10000, "Excitation energy [keV]");

    for (int i = 0 ; i < 16 ; ++i){
        for (int j = 0 ; j < 16 ; ++j){
            if (i < 9){
                if (j < 9){
                    h_ex_angA[16*i + j] = Spec("h_ex_angA_0"+std::to_string(i+1)+"_0"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+")",3000, -2000, 10000, "Excitation energy [keV]");
                    h_ex_angB[16*i + j] = Spec("h_ex_angB_0"+std::to_string(i+1)+"_0"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+")",3000, -2000, 10000, "Excitation energy [keV]");
                    h_ex_coin_ang[16*i + j] = Spec("h_ex_coin_ang_0"+std::to_string(i+1)+"_0"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+"), #gamma-coincidence",3000, -2000, 10000, "Excitation energy [keV]");
#if ALL_EDE_ANGLE
                    m_e_de_A_ang[i][j] = Mat("m_e_de_A_0"+std::to_string(i+1)+"_0"+std::to_string(j+1), "E energy - dE energy ("+std::to_string(i+320)+","+std::to_string(j+352)+")", 500, 0, 20000, "E energy [keV]", 500, 0, 15000, "#Delta E energy [keV]");
                    m_e_de_B_ang[i][j] = Mat("m_e_de_B_0"+std::to_string(i+1)+"_0"+std::to_string(j+1), "E energy - dE energy ("+std::to_string(i+416)+","+std::to_string(j+448)+")", 500, 0, 20000, "E energy [keV]", 500, 0, 15000, "#Delta E energy [keV]");
#endif // ALL_EDE_ANGLE
                } else {
                    h_ex_angA[16*i + j] = Spec("h_ex_angA_0"+std::to_string(i+1)+"_"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+")",3000, -2000, 10000, "Excitation energy [keV]");
                    h_ex_angB[16*i + j] = Spec("h_ex_angB_0"+std::to_string(i+1)+"_"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+")",3000, -2000, 10000, "Excitation energy [keV]");
                    h_ex_coin_ang[16*i + j] = Spec("h_ex_coin_ang_0"+std::to_string(i+1)+"_"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+"), #gamma-coincidence",3000, -2000, 10000, "Excitation energy [keV]");
#if ALL_EDE_ANGLE
                    m_e_de_A_ang[i][j] = Mat("m_e_de_A_0"+std::to_string(i+1)+"_"+std::to_string(j+1), "E energy - dE energy ("+std::to_string(i+320)+","+std::to_string(j+352)+")", 500, 0, 20000, "E energy [keV]", 500, 0, 15000, "#Delta E energy [keV]");
                    m_e_de_B_ang[i][j] = Mat("m_e_de_B_0"+std::to_string(i+1)+"_"+std::to_string(j+1), "E energy - dE energy ("+std::to_string(i+416)+","+std::to_string(j+448)+")", 500, 0, 20000, "E energy [keV]", 500, 0, 15000, "#Delta E energy [keV]");
#endif // ALL_EDE_ANGLE
                }
            } else {
                if (j < 9){
                    h_ex_angA[16*i + j] = Spec("h_ex_angA_"+std::to_string(i+1)+"_0"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+")",3000, -2000, 10000, "Excitation energy [keV]");
                    h_ex_angB[16*i + j] = Spec("h_ex_angB_"+std::to_string(i+1)+"_0"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+")",3000, -2000, 10000, "Excitation energy [keV]");
                    h_ex_coin_ang[16*i + j] = Spec("h_ex_coin_ang_"+std::to_string(i+1)+"_0"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+"), #gamma-coincidence",3000, -2000, 10000, "Excitation energy [keV]");
#if ALL_EDE_ANGLE
                    m_e_de_A_ang[i][j] = Mat("m_e_de_A_"+std::to_string(i+1)+"_0"+std::to_string(j+1), "E energy - dE energy ("+std::to_string(i+320)+","+std::to_string(j+352)+")", 500, 0, 20000, "E energy [keV]", 500, 0, 15000, "#Delta E energy [keV]");
                    m_e_de_B_ang[i][j] = Mat("m_e_de_B_"+std::to_string(i+1)+"_0"+std::to_string(j+1), "E energy - dE energy ("+std::to_string(i+416)+","+std::to_string(j+448)+")", 500, 0, 20000, "E energy [keV]", 500, 0, 15000, "#Delta E energy [keV]");
#endif // ALL_EDE_ANGLE
                } else {
                    h_ex_angA[16*i + j] = Spec("h_ex_angA_"+std::to_string(i+1)+"_"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+")",3000, -2000, 10000, "Excitation energy [keV]");
                    h_ex_angB[16*i + j] = Spec("h_ex_angB_"+std::to_string(i+1)+"_"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+")",3000, -2000, 10000, "Excitation energy [keV]");
                    h_ex_coin_ang[16*i + j] = Spec("h_ex_coin_ang_"+std::to_string(i+1)+"_"+std::to_string(j+1), "Excitation energy ("+std::to_string(i+1)+","+std::to_string(j+1)+"), #gamma-coincidence",3000, -2000, 10000, "Excitation energy [keV]");
#if ALL_EDE_ANGLE
                    m_e_de_A_ang[i][j] = Mat("m_e_de_A_"+std::to_string(i+1)+"_"+std::to_string(j+1), "E energy - dE energy ("+std::to_string(i+320)+","+std::to_string(j+352)+")", 500, 0, 20000, "E energy [keV]", 500, 0, 15000, "#Delta E energy [keV]");
                    m_e_de_B_ang[i][j] = Mat("m_e_de_B_"+std::to_string(i+1)+"_"+std::to_string(j+1), "E energy - dE energy ("+std::to_string(i+416)+","+std::to_string(j+448)+")", 500, 0, 20000, "E energy [keV]", 500, 0, 15000, "#Delta E energy [keV]");
#endif // ALL_EDE_ANGLE
                }
            }
        }
    }

    n_Ax = Spec("n_Ax", "Words within 20 ns of the trigger", 256, 0, 256, "Number of vertical strip words");
    n_Ay = Spec("n_Ay", "Words within 20 ns of the trigger", 256, 0, 256, "Number of horizontal strip words");
    n_AE = Spec("n_AE", "Words within 20 ns of the trigger", 256, 0, 256, "Number of back strip words");

    n_Bx = Spec("n_Bx", "Words within 20 ns of the trigger", 256, 0, 256, "Number of vertical strip words");
    n_By = Spec("n_By", "Words within 20 ns of the trigger", 256, 0, 256, "Number of horizontal strip words");
    n_BE = Spec("n_BE", "Words within 20 ns of the trigger", 256, 0, 256, "Number of back strip words");

    alfna_clover = Mat("alfna_clover", "Excitation - gamma conicidence matrix", 1000, 0, 8000, "Clover energy [keV]", 1000, 0, 8000, "Excitation energy [keV]");
    alfna_labr = Mat("alfna_labr", "Excitation - gamma conicidence matrix", 1000, 0, 8000, "LaBr energy [keV]", 1000, 0, 8000, "Excitation energy [keV]");

    alfna_clover_dop = Mat("alfna_clover_dop", "Excitation - gamma conicidence matrix", 1000, 0, 8000, "Clover energy [keV]", 1000, 0, 8000, "Excitation energy [keV]");
    alfna_labr_dop = Mat("alfna_labr_dop", "Excitation - gamma conicidence matrix", 1000, 0, 8000, "LaBr energy [keV]", 1000, 0, 8000, "Excitation energy [keV]");

    m_detectors_E = Mat("m_detectors_E", "Prompt #gamma ray spectra of all detector", 16384, 0, 16384, "#gamma detector energy [keV]", 10, 1, 11, "Detector id.");
    m_detectors_E_dop = Mat("m_detectors_E_dop", "Prompt Doppler corrected #gamma ray spectra of all detector", 16384, 0, 16384, "#gamma detector energy [keV]", 10, 1, 11, "Detector id.");

    h_Clover_bg = Spec("h_Clover_bg", "Background spectrum - Clovers", 16384, 0, 16384, "Energy [keV]");
    h_LaBr_bg = Spec("h_LaBr_bg", "Background spectrum - LaBr", 16384, 0, 16384, "Energy [keV]");

    m_E_ang = Mat("m_E_ang", "Angle - energy matrix, both", 16*16, 0, 16*16, "Angle (pixel id.)", 5000, 0, 25000, "Particle energy [keV]");
    m_E_ang_A = Mat("m_E_ang_A", "Angle - energy matrix, A", 16*16, 0, 16*16, "Angle (pixel id.)", 5000, 0, 25000, "Particle energy [keV]");
    m_E_ang_B = Mat("m_E_ang_B", "Angle - energy matrix, B", 16*16, 0, 16*16, "Angle (pixel id.)", 5000, 0, 25000, "Particle energy [keV]");
    m_E_ang_A_coin = Mat("m_E_ang_A_coin", "Angle - energy matrix, B, gamma coincidence", 16*16, 0, 16*16, "Angle (pixel id.)", 5000, 0, 25000, "Particle energy [keV]");
    m_E_ang_B_coin = Mat("m_E_ang_B_coin", "Angle - energy matrix, B, gamma coincidence", 16*16, 0, 16*16, "Angle (pixel id.)", 5000, 0, 25000, "Particle energy [keV]");
    m_E_ang_corr = Mat("m_E_ang_corr", "dlsaf", 50, 20, 70, "Angle [deg]", 5000, 0, 25000, "Particle energy [keV]");


    avrDiff_dE=0;
    ndEw=0;
    nEvents = 0;
    nWords = 0;
    avrTimediff = 0;
    avrSq = 0;
    nClover = 0;
    nDiscarded = 0;
    nCoincidence=0,nCoLaBr=0, nCoClover=0;
    nBg=0, nBgLaBr=0, nBgClover=0;
}


bool UserSort::Sort(const Event &event)
{
    nEvents += 1;
    nWords += event.length;
    _rando = drand48() - 0.5;

    // Reseting variables.
    int ndEx=0, ndEy=0, ndE=0;
    nL1=0, nL2=0;
    nXA=0; nYA=0; nEA=0;
    nXB=0; nYB=0; nEB=0;
    for (int i = 0 ; i < 8 ; ++i)
        nC[i] = 0;

    std::string telescope;
    word_t dX[16], dY[16], dE[16];

    // Defining some variables used in loops. May
    // give a slight increase in performance.
    int64_t timediff;
    double eSi, e_addback, e_bg, e_thisWord, eDop, eLaBr;

    bool g_in_coin = false;

    // Sorting out the data words of the different detectors.
    for (int i = 0 ; i < event.length ; ++i){
        __builtin_prefetch(&event.words[i+1]);
        telescope = detectors->Find(event.words[i].address).getTelescope();
        uint16_t address = event.words[i].address;
        if (telescope[0] == 'C'){
            int cnum = telescope[1] - '1';
            if (nC[cnum] < 256)
                clover[cnum][nC[cnum]] = event.words[i];
            nC[cnum] += 1;
        } else if (address == 520){ // Hardcode for speed.
            if (nL1 < 256)
                labr1[nL1] = event.words[i];
            ++nL1;
        } else if (address == 521){ // Hardcode for speed.
            if (nL2 < 256)
                labr2[nL2] = event.words[i];
            ++nL2;
        } else if (320 <= address && address <= 335){
            if (nXA < 256)
                dXA[nXA] = event.words[i];
            ++nXA;
        } else if (352 <= address && address <= 367){
            if (nYA < 256)
                dYA[nYA] = event.words[i];
            ++nYA;
        } else if (384 <= address && address <= 399){
            if (nEA < 256)
                dEA[nEA] = event.words[i];
            ++nEA;
        } else if (416 <= address && address <= 431){
            if (nXB < 256)
                dXB[nXB] = event.words[i];
            nXB += 1;
        } else if (448 <= address && address <= 463){
            if (nYB < 256)
                dYB[nYB] = event.words[i];
            ++nYB;
        } else if (480 <= address && address <= 495){
            if (nEB < 256)
                dEB[nEB] = event.words[i];
            ++nEB;
        }
    }

    if (event.trigTel == 'A'){

        // Check if the rules for a "trigger" is not broken.

        for (int i = 0 ; i < nEB ; ++i){

            timediff = dEB[i].timestamp - event.timestamp;

            if (std::abs(timediff) < 5){
                nDiscarded += 1;
                return true;
            }
        }
        for (int i = 0 ; i < nXA ; ++i){

            timediff = dXA[i].timestamp - event.timestamp;
            eSi = GainEnergy(dXA[i].address, dXA[i].adcdata+_rando);

            m_E_t_X_A[dXA[i].address-320]->Fill(eSi, timediff*10);
            if (std::abs(timediff) <= 6){
                if (ndEx < 16)
                    dX[ndEx] = dXA[i];
                ++ndEx;
            }
        }
        for (int i = 0 ; i < nYA ; ++i){

            timediff = dYA[i].timestamp - event.timestamp;
            eSi = GainEnergy(dYA[i].address, dYA[i].adcdata+_rando);

            m_E_t_Y_A[dYA[i].address-352]->Fill(eSi, timediff*10);
            if (std::abs(timediff) <= 6){
                if (ndEy < 16)
                    dY[ndEy] = dYA[i];
                ++ndEy;
            }
        }
        for (int i = 0 ; i < nEA ; ++i){

            timediff = dEA[i].timestamp - event.timestamp;
            eSi = GainEnergy(dEA[i].address, dEA[i].adcdata+_rando);

            m_E_t_A[dEA[i].address-384]->Fill(eSi, timediff*10);
            if (std::abs(timediff) <= 6){
                if (ndE < 16)
                    dE[ndE] = dEA[i];
                ++ndE;
            }
        }

        // If trigger ok, then run analysis on the event.
        if (ndEx == 1 && ndEy == 1 && ndE == 1){

            // Energy of in the telescopes.
            double eEx = GainEnergy(dX[0].address, dX[0].adcdata+_rando);
            double eEy = GainEnergy(dY[0].address, dY[0].adcdata+_rando);
            double eE = GainEnergy(dE[0].address, dE[0].adcdata+_rando);

            // Fill the particle matrices using the average of the x and y strip.
            m_e_de_A->Fill(eE, (eEx+eEy)*0.5);
#if ALL_EDE_ANGLE
            m_e_de_A_ang[vertA(dX[0].address) - 1][horiA(dY[0].address) - 1]->Fill(eE, (eEx+eEy)*0.5);
#endif // ALL_EDE_ANGLE

            // Gate on protons/deutrons, whatever.
            if (GateA(0.5*(eEx+eEy), eE) && 0.5*(eEx + eEy) + eE < maxEA){

                // Particle matrix after gating.
                m_e_de_A_gate->Fill(eE, (eEx+eEy)*0.5);

                // Energy - Angle correlation spectrum.
                m_E_ang->Fill(16*(vertA(dX[0].address)-1)+horiA(dY[0].address)-1, eE+0.5*(eEx+eEy));
                m_E_ang_A->Fill(16*(vertA(dX[0].address)-1)+horiA(dY[0].address)-1, eE+0.5*(eEx+eEy));

                // Calculating the excitation of the residual nucleus.
                double excitation = exA(dX[0].address, dY[0].address, (0.5*(eEx+eEy)+eE)*1e-3)*1e3;

                // The excitation spectra.
                h_ex_A->Fill(excitation);
                h_ex->Fill(excitation);
                h_ex_angA[16*(vertA(dX[0].address)-1) + horiA(dY[0].address) - 1]->Fill(excitation);

                // Now we loop and look for interesting Clover data.
                for (int i = 0 ; i < 8 ; ++i){
                    e_addback = 0; // If there is events where we have to use add-back to get the full energy.
                    e_bg = 0;

                    bool Co=false;
                    bool Bg=false;

                    // Looping over all words from the particular Clover detector.
                    for (int j = 0 ; j < nC[i] ; ++j){

                        // To identify the current telescope.
                        telescope = detectors->Find(clover[i][j].address).getTelescope();

                        // Calculate the energy of the current word.
                        e_thisWord = GainEnergy(clover[i][j].address, clover[i][j].adcdata+_rando);

                        // Time difference between trigger and clover detector.
                        timediff = clover[i][j].timestamp - event.timestamp;

                        // Energy time spectra for each of the crystals. For concidence gating.
                        // Do this for each of the crystals in each of the telescopes.
                        m_E_t_clover_A[i][telescope[2]-'A']->Fill(e_thisWord, timediff*10);

                        // Now gate on time. We assume that all words within the time gate are from the same gamma.
                        if (GateTimeA_main(clover[i][j].address, timediff) || GateTimeA_sec(clover[i][j].address, timediff)){
                            e_addback += e_thisWord;
                            Co=true;
                        // Everything arriving 500 ns before the prompt peak is random background, use data from this as a estimate of the
                        // background of the events.
                        } else if (GateTimeA_main(clover[i][j].address, timediff-50) || GateTimeA_sec(clover[i][j].address, timediff-50)){
                            e_bg += e_thisWord;
                            Bg=true;
                        }
                    }
                    if (Co){ // If there was any conicidences within the time gate, then we keep the total amount of energy deposited.

                        // Get the propper Doppler correction for the particular telescope.
                        // We could use the mean position of the crystal and not the entire detector,
                        // but currently I don't know which crystal is which.
                        eDop = dopA(detectors->Find(clover[i][0].address).getScatAngle(), e_addback);
                        m_detectors_E->Fill(e_addback, i);
                        alfna_clover->Fill(e_addback, excitation); // No doppler correction.
                        alfna_clover_dop->Fill(eDop, excitation); // Doppler corrected.

                        m_detectors_E->Fill(e_addback, i+1);
                        m_detectors_E_dop->Fill(eDop, i+1);

                        nCoincidence += 1;
                        nCoClover += 1;
                        g_in_coin = true;

                    }
                    if (Bg){
                        eDop = dopA(detectors->Find(clover[i][0].address).getScatAngle(), e_bg);
                        alfna_clover->Fill(e_bg, excitation, -1);
                        alfna_clover_dop->Fill(eDop, excitation, -1);
                        h_Clover_bg->Fill(e_bg);
                        nBg += 1;
                        nBgClover += 1;
                    }
                }

                // Looping through the LaBr words.
                for (int i = 0 ; i < nL1 ; ++i){
                    timediff = labr1[i].timestamp - event.timestamp;
                    eLaBr = GainEnergy(labr1[i].address, labr1[i].adcdata+_rando);

                    m_E_t_LaBr_A[0]->Fill(eLaBr, timediff*10);

                    eDop = dopA(detectors->Find(labr1[i].address).getScatAngle(), eLaBr);

                    // Is the gamma prompt? Then we save it!
                    if (GateTimeA_main(labr1[i].address, timediff) || GateTimeA_sec(labr1[i].address, timediff)){
                        alfna_labr->Fill(eLaBr, excitation);
                        alfna_labr_dop->Fill(eDop, excitation);
                        m_detectors_E->Fill(eLaBr, 9);
                        m_detectors_E_dop->Fill(eDop, 9);
                        nCoincidence += 1;
                        nCoLaBr += 1;
                        g_in_coin = true;
                    } else if (timediff >= 60 && timediff <= 79){ // Background subtraction.
                        alfna_labr->Fill(eLaBr, excitation, -1);
                        alfna_labr_dop->Fill(eDop, excitation, -1);
                        h_LaBr_bg->Fill(eLaBr);
                        nBg += 1;
                        nBgLaBr += 1;
                    }
                }

                for (int i = 0 ; i < nL2 ; ++i){
                    timediff = labr2[i].timestamp - event.timestamp;
                    eLaBr = GainEnergy(labr2[i].address, labr2[i].adcdata+_rando);

                    m_E_t_LaBr_A[1]->Fill(eLaBr, timediff*10);

                    eDop = dopA(detectors->Find(labr2[i].address).getScatAngle(), eLaBr);

                    // Is the gamma prompt? Then we save it!
                    if (GateTimeA_main(labr2[i].address, timediff) || GateTimeA_sec(labr2[i].address, timediff)){
                        alfna_labr->Fill(eLaBr, excitation);
                        alfna_labr_dop->Fill(eDop, excitation);
                        m_detectors_E->Fill(eLaBr, 10);
                        m_detectors_E_dop->Fill(eDop, 10);
                        nCoincidence += 1;
                        nCoLaBr += 1;
                        g_in_coin = true;
                    } else if (timediff >= 60 && timediff <= 77){ // Background subtraction.
                        alfna_labr->Fill(eLaBr, excitation, -1);
                        alfna_labr_dop->Fill(eDop, excitation, -1);
                        h_LaBr_bg->Fill(eLaBr);
                        nBg += 1;
                        nBgLaBr += 1;
                    }
                }

                if (g_in_coin){
                    h_ex_coin->Fill(excitation);
                    h_ex_coinA->Fill(excitation);
                    h_ex_coin_ang[16*(vertA(dX[0].address)-1) + horiA(dY[0].address) - 1]->Fill(excitation);
                    m_E_ang_A_coin->Fill(16*(vertA(dX[0].address)-1)+horiA(dY[0].address)-1, eE+0.5*(eEx+eEy));
                }
            }
        }
    } else if (event.trigTel == 'B'){

        // Check if the rules for a "trigger" is not broken.

        for (int i = 0 ; i < nEA ; ++i){

            timediff = dEA[i].timestamp - event.timestamp;

            if (std::abs(timediff) < 5){
                nDiscarded += 1;
                return true;
            }
        }

        for (int i = 0 ; i < nXB ; ++i){

            timediff = dXB[i].timestamp - event.timestamp;
            eSi = GainEnergy(dXB[i].address, dXB[i].adcdata+_rando);
            m_E_t_X_B[dXB[i].address-416]->Fill(eSi, timediff*10);

            if (std::abs(timediff) <= 6){
                if (ndEx < 16)
                    dX[ndEx] = dXB[i];
                ++ndEx;
            }
        }
        for (int i = 0 ; i < nYB ; ++i){

            timediff = dYB[i].timestamp - event.timestamp;
            eSi = GainEnergy(dYB[i].address, dYB[i].adcdata+_rando);
            m_E_t_Y_B[dYB[i].address-448]->Fill(eSi, timediff*10);

            if (std::abs(timediff) <= 6){
                if (ndEy < 16)
                    dY[ndEy] = dYB[i];
                ++ndEy;
            }
        }
        for (int i = 0 ; i < nEB ; ++i){

            timediff = dEB[i].timestamp - event.timestamp;
            eSi = GainEnergy(dEB[i].address, dEB[i].adcdata+_rando);
            m_E_t_B[dEB[i].address-480]->Fill(eSi, timediff*10);

            if (std::abs(timediff) <= 6){
                if (ndE < 16)
                    dE[ndE] = dEB[i];
                ++ndE;
            }
        }

        // If trigger ok, then run analysis on the event.
        if (ndEx == 1 && ndEy == 1 && ndE == 1){

            double eEx = GainEnergy(dX[0].address, dX[0].adcdata+_rando);
            double eEy = GainEnergy(dY[0].address, dY[0].adcdata+_rando);
            double eE = GainEnergy(dE[0].address, dE[0].adcdata+_rando);

            m_e_de_B->Fill(eE, (eEx+eEy)*0.5);

#if ALL_EDE_ANGLE
            m_e_de_B_ang[vertB(dX[0].address)-1][horiB(dY[0].address)-1]->Fill(eE, (eEx+eEy)*0.5);
#endif // ALL_EDE_ANGLE

            if (GateB(0.5*(eEx+eEy), eE) && 0.5*(eEx + eEy) + eE < maxEB){

                m_e_de_B_gate->Fill(eE, (eEx+eEy)*0.5);

                // Particle energy - angle correlation.
                m_E_ang->Fill(16*(vertB(dX[0].address)-1)+horiB(dY[0].address)-1, eE+0.5*(eEx+eEy));
                m_E_ang_B->Fill(16*(vertB(dX[0].address)-1)+horiB(dY[0].address)-1, eE+0.5*(eEx+eEy));

                // Calculating the excitation of the residual nucleus.
                double excitation = exB(dX[0].address, dY[0].address, (0.5*(eEx+eEy)+eE)*1e-3)*1e3;

                // The excitation spectra.
                h_ex_B->Fill(excitation);
                h_ex->Fill(excitation);

                h_ex_angB[16*(vertB(dX[0].address)-1) + horiB(dY[0].address) - 1]->Fill(excitation);

                // Now we loop and look for interesting Clover data.
                for (int i = 0 ; i < 8 ; ++i){
                    e_addback = 0; // If there is events where we have to use add-back to get the full energy.
                    e_bg = 0;
                    bool Co = false;
                    bool Bg = false;

                    for (int j = 0 ; j < nC[i] ; ++j){

                        // To identify the current telescope.
                        telescope = detectors->Find(clover[i][j].address).getTelescope();

                        // Calculate the energy of the current word.
                        e_thisWord = GainEnergy(clover[i][j].address, clover[i][j].adcdata+_rando);

                        // Time difference between the clover word and the trigger.
                        timediff = clover[i][j].timestamp - event.timestamp;


                        avrTimediff += timediff;
                        avrSq += timediff*timediff;
                        nClover += 1;
                        // Raw energy time spectra for each of the crystals. For concidence gating.
                        // Do this for each of the telescopes, since this may be different.
                        m_E_t_clover_B[i][telescope[2]-'A']->Fill(e_thisWord, timediff*10);

                        // Now gate on time. We assume that all words within the time gate are from the same gamma.
                        if (GateTimeB_main(clover[i][j].address, timediff) || GateTimeB_sec(clover[i][j].address, timediff)){
                            Co = true;
                            e_addback += e_thisWord;
                        // Everything arriving 500 ns before the prompt peak is random background, use data from this as a estimate of the
                        // background of the events.
                        } else if (GateTimeB_main(clover[i][j].address, timediff-50) || GateTimeB_sec(clover[i][j].address, timediff-50)){
                            e_bg += e_thisWord;
                            Bg = true;
                        }
                    }
                    if (Co){ // If there was any conicidences within the time limit.

                        // Get the propper Doppler correction for the particular telescope.
                        // We could use the mean position of the crystal and not the entire detector,
                        // but currently I don't know which crystal is which.
                        eDop = dopB(detectors->Find(clover[i][0].address).getScatAngle(), e_addback);

                        alfna_clover->Fill(e_addback, excitation); // No doppler correction.
                        alfna_clover_dop->Fill(eDop, excitation); // Doppler corrected.

                        m_detectors_E->Fill(e_addback, i+1);
                        m_detectors_E_dop->Fill(eDop, i+1);

                        nCoincidence += 1;
                        nCoClover += 1;
                        g_in_coin = true;
                    }

                    if (Bg){
                        eDop = dopA(detectors->Find(clover[i][0].address).getScatAngle(), e_bg);
                        alfna_clover->Fill(e_bg, excitation, -1);
                        alfna_clover_dop->Fill(eDop, excitation, -1);
                        h_Clover_bg->Fill(e_bg);
                        nBg += 1;
                        nBgClover += 1;
                    }
                }

                // Looping through the LaBr words.
                for (int i = 0 ; i < nL1 ; ++i){
                    timediff = labr1[i].timestamp - event.timestamp;

                    // Retriving the correct energy using the supplied gain and shift values.
                    eLaBr = GainEnergy(labr1[i].address, labr1[i].adcdata+_rando);

                    m_E_t_LaBr_B[0]->Fill(eLaBr, timediff*10); // Filling time spectrum.

                    // Calculating the Doppler corrected energy.
                    eDop = dopB(detectors->Find(labr1[i].address).getScatAngle(), eLaBr);

                    // Is the gamma prompt? Then we save it!
                    if (GateTimeB_main(labr1[i].address, timediff) || GateTimeB_sec(labr1[i].address, timediff)){
                        alfna_labr->Fill(eLaBr, excitation);
                        alfna_labr_dop->Fill(eDop, excitation);
                        m_detectors_E->Fill(eLaBr, 9);
                        m_detectors_E_dop->Fill(eDop, 9);
                        nCoincidence += 1;
                        nCoLaBr += 1;
                        g_in_coin = true;
                    } else if (timediff >= 60 && timediff <= 76){ // Background subtraction.
                        alfna_labr->Fill(eLaBr, excitation, -1);
                        alfna_labr_dop->Fill(eDop, excitation, -1);
                        h_LaBr_bg->Fill(eLaBr);
                        nBg += 1;
                        nBgLaBr += 1;
                    }
                }

                for (int i = 0 ; i < nL2 ; ++i){
                    timediff = labr2[i].timestamp - event.timestamp;
                    eLaBr = GainEnergy(labr2[i].address, labr2[i].adcdata+_rando);

                    m_E_t_LaBr_B[1]->Fill(eLaBr, timediff*10); // Filling time spectrum.

                    eDop = dopB(detectors->Find(labr2[i].address).getScatAngle(), eLaBr);

                    // Saving the prompt gammas!
                    if (GateTimeB_main(labr2[i].address, timediff) || GateTimeB_sec(labr2[i].address, timediff)){
                        alfna_labr->Fill(eLaBr, excitation);
                        alfna_labr_dop->Fill(eDop, excitation);
                        m_detectors_E->Fill(eLaBr, 10);
                        m_detectors_E_dop->Fill(eDop, 10);
                        nCoincidence += 1;
                        nCoLaBr += 1;
                        g_in_coin = true;
                    } else if (timediff >= 60 && timediff <= 76){ // And subtracting the background.
                        alfna_labr->Fill(eLaBr, excitation, -1);
                        alfna_labr_dop->Fill(eDop, excitation, -1);
                        h_LaBr_bg->Fill(eLaBr);
                        nBg += 1;
                        nBgLaBr += 1;
                    }
                }

                if (g_in_coin){
                    h_ex_coin->Fill(excitation);
                    h_ex_coinB->Fill(excitation);
                    h_ex_coin_ang[16*(vertB(dX[0].address)-1) + horiB(dY[0].address) - 1]->Fill(excitation);
                    m_E_ang_B_coin->Fill(16*(vertB(dX[0].address)-1)+horiB(dY[0].address)-1, eE+0.5*(eEx+eEy));
                }
            }
        }
    }

    return true;
}

bool UserSort::End()
{
    std::cout << "Total number of events: " << nEvents << " Average event length (words): " << double(nWords)/double(nEvents) << std::endl;
    std::cout << "Average clover timediff: " << double(avrTimediff)/nClover << " ± " << sqrt(double(avrSq)/nClover - double(avrTimediff)*double(avrTimediff)/(nClover*nClover)) << std::endl;
    std::cout << "Number of events discarded: " << nDiscarded << std::endl;
    std::cout << "Number of coinsidences identified: Total=" << nCoincidence << ", Clover=" << nCoClover << ", LaBr=" << nCoLaBr << std::endl;
    std::cout << "Background subtracted: Total=" << nBg << ", Clover=" << nBgClover << ", LaBr=" << nBgLaBr << std::endl;
    return true;
}
