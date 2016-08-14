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

#ifndef TDRROUTINE_H
#define TDRROUTINE_H

#include "UserRoutine.h"
#include "Histograms.h"
#include "GainParameters.h"
#include "TelescopeGate.h"
#include "Telescope.h"
#include "TimeGate.h"


#include <string>
#include <cstdint>

class TTree;
class TFile;

/*!
 * \class TDRRoutine
 * \brief Interface for sorting routines for inverse kinematics.
 * \details This interface defines the basic functionality that is needed for all sorting routines for inverse kinematics experiments recorded
 * in the TDR binary file format. The user need to implement a class "Telescope" that describes the experimental setup of the telescopes.
 * \author Vetle W. Ingeberg
 * \version 0.8.0
 * \date 2015-2016
 * \copyright GNU Public License v. 3
 * \todo Implement support for ROOT trees.
 */
class TDRRoutine : public UserRoutine {
public:

    //! Constructor.
	TDRRoutine();

    //! Start of sorting session.
    /*! \return true if successful, false otherwise.
     */
	bool Start();

    //! Pass a command to the sorting routine.
    /*! \return true if successful, false otherwise.
     */
    bool Command(const std::string& cmd /*!< String containing the command to be executed. */);

    //! User class commands.
    /*! \return true if successful, false otherwise.
     */
    virtual bool UserCommand(const std::string& cmd)=0;

    //! Signal to end the sorting session.
    /*! \return true if successful, false otherwise.
     */
	bool End();

protected:

    //! Random number for smoothing of histograms.
    float _rando;

    //! Variable to keep parameters for gain.
    std::unique_ptr<GainParameters> gain_param;

    //! Variable to keep telescope gate A.
    std::unique_ptr<TelescopeGate> telGateA;

    //! Variable to keep telescope gate B.
    std::unique_ptr<TelescopeGate> telGateB;

    //! To calculate the excitation as a function of energy deposited in telescope A.
    std::unique_ptr<Telescope> kinA;

    //! To calculate the excitation as a function of energy deposited in telescope B.
    std::unique_ptr<Telescope> kinB;

    //! Main time gates for events in telescope A.
    std::unique_ptr<TimeGate> tGateA_main;

    //! Secondary time gate for events in telescope A.
    std::unique_ptr<TimeGate> tGateA_sec;

    //! Time gate for background subtraction in telescope A.
    std::unique_ptr<TimeGate> tGateA_bg;

    //! Main time gates for events in telescope B.
    std::unique_ptr<TimeGate> tGateB_main;

    //! Secondary time gate for events in telescope B.
    std::unique_ptr<TimeGate> tGateB_sec;

    //! Time gate for background subtraction in telescope B.
    std::unique_ptr<TimeGate> tGateB_bg;

	//! Create all spectra.
	/*! This method must be implemented in a class deriving from TDRRoutine.
	 */
	virtual void CreateSpectra() = 0;

    //! Performs the gain of the energy of a word.
    /*! \return the correct gain of a detector.
     */
    inline double GainEnergy(const uint16_t &addr, /*!< The ADC address of the detector.      */
                             const float &E     /*!< The ADC measurement of the detector.  */) const
    {
        return gain_param->Find(addr).Eeval(E);
    }

    //! Calls the kinematics class that performs kinematics calculations, spesific for telescope A.
    /*! \return the excitation energy.
     */
    inline double exA(const uint16_t &addrX,    /*!< ADC address of the horizontal strip.   */
                      const uint16_t &addrY,    /*!< ADC address of the vertical strip.     */
                      const double &EdE         /*!< Energy deposited in the telescope.     */) const
    {
        return kinA->CalcEx(addrX, addrY, EdE);
    }

    //! Calls the kinematics class that performs kinematics calculations, spesific for telescope B.
    /*! \return the excitation energy.
     */
    inline double exB(const uint16_t &addrX,        /*!< ADC address of the horizontal strip.   */
                      const uint16_t &addrY,        /*!< ADC address of the vertical strip.     */
                      const double &EdE             /*!< Energy deposited in the telescope.     */) const
    {
        return kinB->CalcEx(addrX, addrY, EdE);
    }
    
    //! Calls the kinematics class that determines the Doppler shift of a observed gamma. (if particle coinsidence with telescope A).
    /*! \return the Doppler corrected gamma energy.
     */
    inline double dopA(const double &theta, /*!< Angle of the detector.                     */
                       const double &e      /*!< Deposited energy in the gamma detector.    */) const
    {
        return kinA->CalcDoppler(theta, e);
    }

    //! Calls the kinematics class that determines the Doppler shift of a observed gamma. (if particle coinsidence with telescope B).
    /*! \return the Doppler corrected gamma energy.
     *  Angles are assumed to be defined such that the beam moves along the y-axis.
     */
    inline double dopB(const double &theta, /*!< Angle of the detector.                     */
                       const double &e      /*!< Deposited energy in the gamma detector.    */) const
    {
        return kinB->CalcDoppler(theta, e);
    }

    //! Check if within the gate in A telescope.
    /*! \return True if within the gate, false otherwise.
     */
    inline bool GateA(const double &de, /*!< Energy deposited in front detector.    */
                      const double &e   /*!< Energy deposited in back detector.     */) const
    {
            return telGateA->Eval(de, e);
    }

    //! Check if within the gate in A telescope.
    /*! \return True if within the gate, false otherwise.
     */
    inline bool GateB(const double &de, /*!< Energy deposited in front detector.    */
                      const double &e   /*!< Energy deposited in back detector.     */) const
    {
        return telGateB->Eval(de, e);
    }

    //! Get the vertical strip, telescope A.
    inline int vertA(const uint16_t &addrX) const
    {
        return kinA->findVert(addrX);
    }

    //! Get the horizontal strip, telescope A.
    inline int horiA(const uint16_t &addrY) const
    {
        return kinA->findHori(addrY);
    }

    //! Get the vertical strip, telescope A.
    inline int vertB(const uint16_t &addrX) const
    {
        return kinB->findVert(addrX);
    }

    //! Get the horizontal strip, telescope A.
    inline int horiB(const uint16_t &addrY) const
    {
        return kinB->findHori(addrY);
    }

    //! Calculates the angle of the pixel, telescope A.
    /*! \return the angle of the pixel.
     */
    inline double angA(const uint16_t &addrX,   /*!< ADC address of the horizontal strip.   */
                       const uint16_t &addrY    /*!< ADC address of the vertical strip.     */) const
    {
        return kinA->CalcAngle(addrX, addrY);
    }

    //! Calculates the angle of the pixel, telescope B.
    /*! \return the angle of the pixel.
     */
    inline double angB(const uint16_t &addrX,   /*!< ADC address of the horizontal strip.   */
                       const uint16_t &addrY    /*!< ADC address of the vertical strip.     */) const
    {
        return kinB->CalcAngle(addrX, addrY);
    }

    //! Main time gate, telescope A.
    /*! \return True if inside the gate, false otherwise.
     */
    inline bool GateTimeA_main(const uint16_t &address, /*!< Address of the detector.                           */
                               const long long &timediff      /*!< Time difference between event time and reading.    */) const
    {
        return tGateA_main->Evaluate(address, timediff);
    }

    //! Secondary time gate, telescope A.
    /*! \return true if inside the gate, false otherwise.
     */
    inline bool GateTimeA_sec(const uint16_t &address, /*!< Address of the detector.                           */
                              const long long &timediff      /*!< Time difference between event time and reading.    */) const
    {
        return tGateA_sec->Evaluate(address, timediff);
    }

    //! Background time gate for subtraction, telescope A.
    /*! \return true if inside the gate, false otherwize.
     */
    inline bool GateTimeA_bg(const uint16_t &address, /*!< Address of the detector.                           */
                             const long long &timediff      /*!< Time difference between event time and reading.    */) const
    {
        return tGateA_bg->Evaluate(address, timediff);
    }

    //! Main time gate, telescope B.
    /*! \return True if inside the gate, false otherwise.
     */
    inline bool GateTimeB_main(const uint16_t &address, /*!< Address of the detector.                           */
                               const long long &timediff      /*!< Time difference between event time and reading.    */) const
    {
        return tGateB_main->Evaluate(address, timediff);
    }

    //! Secondary time gate, telescope B.
    /*! \return true if inside the gate, false otherwise.
     */
    inline bool GateTimeB_sec(const uint16_t &address, /*!< Address of the detector.                           */
                              const long long &timediff      /*!< Time difference between event time and reading.    */) const
    {
        return tGateB_sec->Evaluate(address, timediff);
    }

    //! Background time gate for subtraction, telescope A.
    /*! \return true if inside the gate, false otherwize.
     */
    inline bool GateTimeB_bg(const uint16_t &address, /*!< Address of the detector.                           */
                             const long long &timediff      /*!< Time difference between event time and reading.    */) const
    {
        return tGateB_bg->Evaluate(address, timediff);
    }

	//! Create a 1D histogram.
    /*! \return a pointer to a new 1D histogram.
     */
	Histogram1Dp Spec( const std::string& name,		/*!< The name of the new histogram.		*/
					   const std::string& title,	/*!< The title of the new histogram.	*/
					   int channels,				/*!< The number of regular bins.		*/
					   Axis::bin_t left,			/*!< The lower edge of the lowest bin.	*/
					   Axis::bin_t right,			/*!< The upper edge of the highest bin.	*/
                       const std::string& xtitle	/*!< The title of the x axis.			*/)
    {
        return GetHistograms().Create1D(name, title, channels, left, right, xtitle);
    }

	//! Create a 2D histogram.
    /*! \return a pointer to a new 2D histogram.
     */
	Histogram2Dp Mat( const std::string& name,		/*!< The name of the new histogram.						*/
					  const std::string& title,		/*!< The title of the new histogram.					*/
					  int ch1,						/*!< The number of regular bins of the x axis.			*/
                      Axis::bin_t l1,				/*!< The lower edge of the lowest bin on the x axis.	*/
                      Axis::bin_t r1,				/*!< The upper edge of the highest bin on the x axis.	*/
					  const std::string& xtitle,	/*!< The title of the x axis.							*/
					  int ch2,						/*!< The number of regular bins of the y axis.			*/
                      Axis::bin_t l2,				/*!< The lower edge of the lowest bin on the y axis.	*/
                      Axis::bin_t r2,				/*!< The upper edge of the highest bin on the y axis.	*/
                      const std::string& ytitle		/*!< The title of the y axis.							*/)
    {
        return GetHistograms().Create2D(name, title, ch1, l1, r1, xtitle, ch2, l2, r2, ytitle);
    }
};

#endif // TDRROUTINE_H
