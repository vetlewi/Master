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

#include "UserSingles.h"


#include "TDRWordBuffer.h"
#include "Event.h"
#include "DetectorRead.h"
#include "Histogram1D.h"
#include "Histogram2D.h"
#include "Histograms.h"

#include <string>
#include <cstdlib>
#include <sstream>

#ifdef __linux__
    #include "/usr/local/root/include/TFile.h"
    #include "/usr/local/root/include/TTree.h"
#else
    #include <TTree.h>
    #include <TFile.h>
#endif // __linux__


void UserSingles::CreateSpectra()
{
    for (int i = 0 ; i < 16 ; ++i){
		if (i < 9){
            h_DSSD_A_dX[i] = Spec("h_DSSD_A_dX_0"+std::to_string(i+1), "Calibration DSSD A dX " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_A_dY[i] = Spec("h_DSSD_A_dY_0"+std::to_string(i+1), "Calibration DSSD A dY " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_A_E[i] = Spec("h_DSSD_A_E_0"+std::to_string(i+1), "Calibration DSSD A E " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_B_dX[i] = Spec("h_DSSD_B_dX_0"+std::to_string(i+1), "Calibration DSSD B dX " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_B_dY[i] = Spec("h_DSSD_B_dY_0"+std::to_string(i+1), "Calibration DSSD B dY " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_B_E[i] = Spec("h_DSSD_B_E_0"+std::to_string(i+1), "Calibration DSSD B E " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");

            h_DSSD_A_dX_raw[i] = Spec("h_DSSD_A_dX_0"+std::to_string(i+1)+"_raw", "Raw spectra DSSD A dX " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_A_dY_raw[i] = Spec("h_DSSD_A_dY_0"+std::to_string(i+1)+"_raw", "Raw spectra DSSD A dY " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_A_E_raw[i] = Spec("h_DSSD_A_E_0"+std::to_string(i+1)+"_raw", "Raw spectra DSSD A E " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_B_dX_raw[i] = Spec("h_DSSD_B_dX_0"+std::to_string(i+1)+"_raw", "Raw spectra DSSD B dX " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_B_dY_raw[i] = Spec("h_DSSD_B_dY_0"+std::to_string(i+1)+"_raw", "Raw spectra DSSD B dY " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_B_E_raw[i] = Spec("h_DSSD_B_E_0"+std::to_string(i+1)+"_raw", "Raw spectra DSSD B E " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
		} else {
            h_DSSD_A_dX[i] = Spec("h_DSSD_A_dX_"+std::to_string(i+1), "Calibration DSSD A dX " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_A_dY[i] = Spec("h_DSSD_A_dY_"+std::to_string(i+1), "Calibration DSSD A dY " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_A_E[i] = Spec("h_DSSD_A_E_"+std::to_string(i+1), "Calibration DSSD A E " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_B_dX[i] = Spec("h_DSSD_B_dX_"+std::to_string(i+1), "Calibration DSSD B dX " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_B_dY[i] = Spec("h_DSSD_B_dY_"+std::to_string(i+1), "Calibration DSSD B dY " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");
            h_DSSD_B_E[i] = Spec("h_DSSD_B_E_"+std::to_string(i+1), "Calibration DSSD B E " + std::to_string(i+1), 5000, 0, 10000, "Energy [keV]");

            h_DSSD_A_dX_raw[i] = Spec("h_DSSD_A_dX_"+std::to_string(i+1)+"_raw", "Raw spectra DSSD A dX " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_A_dY_raw[i] = Spec("h_DSSD_A_dY_"+std::to_string(i+1)+"_raw", "Raw spectra DSSD A dY " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_A_E_raw[i] = Spec("h_DSSD_A_E_"+std::to_string(i+1)+"_raw", "Raw spectra DSSD A E " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_B_dX_raw[i] = Spec("h_DSSD_B_dX_"+std::to_string(i+1)+"_raw", "Raw spectra DSSD B dX " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_B_dY_raw[i] = Spec("h_DSSD_B_dY_"+std::to_string(i+1)+"_raw", "Raw spectra DSSD B dY " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
            h_DSSD_B_E_raw[i] = Spec("h_DSSD_B_E_"+std::to_string(i+1)+"_raw", "Raw spectra DSSD B E " + std::to_string(i+1), 16384, 0, 16384, "ADC channel");
		}
    }
    char letter[4] = {'A','B','C','D'};
    for (int i = 0 ; i < 8 ; ++i){
        h_Clover_E[i] = Spec("h_Clover_E_"+std::to_string(i+1), "Calibration spectra Clover " + std::to_string(i+1) + ", with add-back", 10000, 0, 10000, "Energy [keV]");
        for (int j = 0 ; j < 4 ; ++j){
            h_Clover_crystal[i][j] = Spec("h_Clover_crystal_"+std::to_string(i+1)+letter[j],"Calibration spectra Clover crystal " + std::to_string(i+1) + letter[j], 10000, 0, 10000, "Energy [keV]");
            h_Clover_crystal_raw[i][j] = Spec("h_Clover_crystal_raw_"+std::to_string(i+1)+letter[j],"Raw spectra Clover crystal " + std::to_string(i+1) + letter[j], 16384, 0, 16384, "ADC channel");
        }
    }
    


    
    h_LaBr_E[0] = Spec("h_LaBr_E_01", "Calibration LaBr 1", 10000, 0, 10000, "Energy [keV]");
    h_LaBr_E[1] = Spec("h_LaBr_E_02", "Calibration LaBr 2", 10000, 0, 10000, "Energy [keV]");
    
    h_LaBr_E_raw[0] = Spec("h_LaBr_E_raw_01", "Raw LaBr 1", 16384, 0, 16384, "ADC channel");
    h_LaBr_E_raw[1] = Spec("h_LaBr_E_raw_02", "Raw LaBr 2", 16384, 0, 16384, "ADC channel");
    
    m_Gamma_E = Mat("m_Gamma_E", "Gamma energy - channel spectrum", 15000, 0, 15000, "Energy [keV]", 10, 0, 10, "Detector");
    m_Particle_E = Mat("m_Particle_E", "Particle energy - channel spectrum", 5000, 0, 10000, "Energy [keV]", 96, 0, 96, "Detector");
}

bool UserSingles::UserCommand(const std::string &cmd)
{
    std::stringstream icmd(cmd.c_str());

    std::string command;
    icmd >> command;
    if (command == "Tree" || command == "TREE" || command == "tree"){
        std::string fname;
        icmd >> fname;

        file = new TFile(fname.c_str(), "recreate");
        tree = new TTree("calibration", "Calibration");
        for (int i = 0 ; i < 8 ; ++i){
            std::string bName = "Clover_"+std::to_string(i+1);
            tree->Branch(bName.c_str(), &cEv[i].nA, "nA/I:adcdataA[nA]/s:timestampA[nA]/L:e_calA[nA]/D:nB/I:adcdataB[nB]/s:timestampB[nB]/L:e_calB[nB]/D:nC/I:adcdataC[nC]/s:timestampC[nC]/L:e_calC[nC]/D:nD/I:adcdataD[nD]/s:timestampD[nD]/L:e_calD[nD]/D");
        }
        return true;
    } else if (command == "Beta") {
        icmd >> beta;
        doppler = true;
        gamma = 1/sqrt(1-beta*beta);
        return true;
    } else {
        return false;
    }
}


bool UserSingles::SortSingles(const Event& event)
{
    
    
	_rando = drand48() - 0.5;
    
    
    std::string telescope;
    double Energy;
    int64_t start_t=0;
    for (int i = 0 ; i < event.length-1 ; ++i){

        telescope = detectors->Find(event.words[i].address).getTelescope();
        Energy = GainEnergy(event.words[i].address, event.words[i].adcdata+_rando);
        if (telescope[0] == 'C'){
            if (makeTree){
                if (event.words[i].timestamp-start_t >= 150){
                    start_t = event.words[i].timestamp;
                    tree->Fill();
                }

                if (telescope[2] == 'A'){
                    cEv[telescope[1]-'1'].adcdataA[cEv[telescope[1]-'1'].nA] = event.words[i].adcdata;
                    cEv[telescope[1]-'1'].timestampA[cEv[telescope[1]-'1'].nA] = event.words[i].timestamp;
                    cEv[telescope[1]-'1'].e_calA[cEv[telescope[1]-'1'].nA] = Energy;
                    cEv[telescope[1]-'1'].nA += 1;
                } else if (telescope[2] == 'B'){
                    cEv[telescope[1]-'1'].adcdataB[cEv[telescope[1]-'1'].nB] = event.words[i].adcdata;
                    cEv[telescope[1]-'1'].timestampB[cEv[telescope[1]-'1'].nB] = event.words[i].timestamp;
                    cEv[telescope[1]-'1'].e_calB[cEv[telescope[1]-'1'].nB] = Energy;
                    cEv[telescope[1]-'1'].nB += 1;
                } else if (telescope[2] == 'C'){
                    cEv[telescope[1]-'1'].adcdataC[cEv[telescope[1]-'1'].nC] = event.words[i].adcdata;
                    cEv[telescope[1]-'1'].timestampC[cEv[telescope[1]-'1'].nC] = event.words[i].timestamp;
                    cEv[telescope[1]-'1'].e_calC[cEv[telescope[1]-'1'].nC] = Energy;
                    cEv[telescope[1]-'1'].nC += 1;
                } else if (telescope[2] == 'D'){
                    cEv[telescope[1]-'1'].adcdataD[cEv[telescope[1]-'1'].nD] = event.words[i].adcdata;
                    cEv[telescope[1]-'1'].timestampD[cEv[telescope[1]-'1'].nD] = event.words[i].timestamp;
                    cEv[telescope[1]-'1'].e_calD[cEv[telescope[1]-'1'].nD] = Energy;
                    cEv[telescope[1]-'1'].nD += 1;
                }
            }

            h_Clover_crystal[telescope[1]-'1'][telescope[2] - 'A']->Fill(Energy);
            h_Clover_crystal_raw[telescope[1]-'1'][telescope[2] - 'A']->Fill(event.words[i].adcdata);
            if (i + 1 < event.length){
                if (detectors->Find(event.words[i+1].address).getTelescope()[1] == telescope[1]){
                    Energy += GainEnergy(event.words[i+1].address, event.words[i+1].adcdata+_rando);
                    ++i;
                }
            }
            if (doppler)
                Energy = Dop(Energy, detectors->Find(event.words[i].address).getTheta(), detectors->Find(event.words[i].address).getPhi());
            h_Clover_E[telescope[1] - '1']->Fill(Energy);
            m_Gamma_E->Fill(Energy, telescope[1] - '1');
        } else if (telescope[0] == 'L'){
            if (doppler)
                Energy = Dop(Energy, detectors->Find(event.words[i].address).getTheta(), detectors->Find(event.words[i].address).getPhi());
            h_LaBr_E[telescope[1] - '1']->Fill(Energy);
            h_LaBr_E_raw[telescope[1] - '1']->Fill(event.words[i].adcdata);
            m_Gamma_E->Fill(Energy, telescope[1] - '1' + 8);
        } else if (event.words[i].address >= 320 && event.words[i].address <= 335){
            h_DSSD_A_dX[event.words[i].address - 320]->Fill(Energy);
            h_DSSD_A_dX_raw[event.words[i].address - 320]->Fill(event.words[i].adcdata);
            m_Particle_E->Fill(Energy, event.words[i].address - 320);
        } else if (event.words[i].address >= 352 && event.words[i].address <= 367){
            h_DSSD_A_dY[event.words[i].address - 352]->Fill(Energy);
            h_DSSD_A_dY_raw[event.words[i].address - 352]->Fill(event.words[i].adcdata);
            m_Particle_E->Fill(Energy, event.words[i].address - 352 + 16);
        } else if (event.words[i].address >= 384 && event.words[i].address <= 399){
            h_DSSD_A_E[event.words[i].address - 384]->Fill(Energy);
            h_DSSD_A_E_raw[event.words[i].address - 384]->Fill(event.words[i].adcdata);
            m_Particle_E->Fill(Energy, event.words[i].address - 384 + 32);
        } else if (event.words[i].address >= 416 && event.words[i].address <= 431){
            h_DSSD_B_dX[event.words[i].address - 416]->Fill(Energy);
            h_DSSD_B_dX_raw[event.words[i].address - 416]->Fill(event.words[i].adcdata);
            m_Particle_E->Fill(Energy, event.words[i].address - 416 + 48);
        } else if (event.words[i].address >= 448 && event.words[i].address <= 463){
            h_DSSD_B_dY[event.words[i].address - 448]->Fill(Energy);
            h_DSSD_B_dY_raw[event.words[i].address - 448]->Fill(event.words[i].adcdata);
            m_Particle_E->Fill(Energy, event.words[i].address - 448 + 64);
        } else if (event.words[i].address >= 480 && event.words[i].address <= 495){
            h_DSSD_B_E[event.words[i].address - 480]->Fill(Energy);
            h_DSSD_B_E_raw[event.words[i].address - 480]->Fill(event.words[i].adcdata);
            m_Particle_E->Fill(Energy, event.words[i].address - 480 + 80);
        }
        
    }
    return true;
}

bool UserSingles::End()
{
    if (makeTree){
        file->Write();
        file->Close();
    }
    return true;
}
