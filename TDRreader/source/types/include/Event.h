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

#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include "TDRWordBuffer.h"

//! Event structure.
struct Event {
    char trigTel;                       //! A if telescope A triggered the event, B if telescope B.
    int length;                         //! Number of words in event.
    int64_t timestamp;                  //! Timestamp of the trigger word.
    std::unique_ptr<word_t[]> words;    //! Array containing the datawords of the event.

    //! Constructor.
    Event() : length( 0 ){ }

    //! Destructor.
    ~Event(){ Reset(); }

    //! Clear memory when not needed.
    void Reset()
    {
        if (words) words.reset();
        length = 0;
    }
};

#endif // EVENT_H
