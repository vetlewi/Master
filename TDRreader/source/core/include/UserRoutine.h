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


#ifndef USERROUTINE_H
#define USERROUTINE_H

#include <string>

#include "Histograms.h"
#include "DefineFile.h"
#include "DetectorRead.h"

struct Event;

/*!
 * \class UserRoutine
 * \brief Interface for sorting routines.
 * \details This is the basic interface for all sorting routines.
 * \author Vetle W. Ingeberg
 * \version 0.8.0
 * \date 2015-2016
 * \copyright GNU Public License v. 3
 * \todo Implement functionality to build ROOT trees.
 */
class UserRoutine {
public:
	//! Virtual no-op destructor.
	virtual ~UserRoutine() { }

	//! Called before starting any sorting.
    virtual bool Start() = 0;

	//! Called for a command.
    virtual bool Command(const std::string& command /*!< Command text to process. */) = 0;
#ifndef SINGLES
	//! Called to sort an event.
    virtual bool Sort(const Event& event /*!< The event structure filled with data. */) = 0;
#endif // SINGLES
#ifdef SINGLES
    //! Called to sort an event as singles.
    virtual bool SortSingles(const Event& event /*!< The event structure filled with data.  */) = 0;
#endif // SINGLES

	//! Called after all sorting is finished.
    virtual bool End() = 0;

    //! Get list of created histograms.
    /*! \return a list of histograms created by the sorting routine.
     */
    Histograms& GetHistograms()
    {
        return histograms;
    }

    //! Pass a map of all detectors in the system.
    void SetDetectors(Detectors *det    /*!< Pointer to the map of all the detectors in the system. */)
    {
        detectors = std::unique_ptr<Detectors>(det);
    }

protected:
    //! List of detectors.
    std::unique_ptr<Detectors> detectors;

private:
    //! The list of histograms.
    Histograms histograms;
};

#endif //USERROUTINE_H
