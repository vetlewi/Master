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
 * \file Unpacker.cpp
 * \brief Implementation of Unpacker.
 * \author Vetle W. Ingeberg
 * \version 0.8.0
 * \date 2015-2016
 * \copyright GNU Public License v. 3
 */

#include "Unpacker.h"

#include "TDRWordBuffer.h"
#include "Event.h"

#include "DefineFile.h"

#include <string>
#include <cstdlib>

Unpacker::Unpacker()
	: buffer( 0 )
	, buffer_idx( 0 )
	, eventlength_sum( 0 )
	, event_count ( 0 )
{
}

void Unpacker::SetBuffer(const WordBuffer* buffr)
{
    buffer = buffr; // Setting internal variable with the new buffer.

    buffer_idx = 0; // Reset buffer position.
    curr_Buf = 0; // Reseting the position in the buffer.

    // Resting the length of the buffer.
	event_count = eventlength_sum = 0;
}

Unpacker::Status Unpacker::Next(Event &event)
{
    if ( curr_Buf >= buffer->GetSize() )
        return END; // End of buffer reached.

	int n_data = 0;
    if ( !UnpackOneEvent(event, n_data) ){
        buffer_idx = n_data + 1;
        return END; // If no event found, end of buffer.
	}

    eventlength_sum += 1;
	event_count += 1;
    buffer_idx = n_data + 1;

	return OKAY;
}

bool Unpacker::ExtractEvent(Event &event, int &n_data, int i, int64_t &timeOfEvent)
{
    int stop = i;
    int start = i;
    int end = -1;

    // Time of the event.
    int64_t ClockStart = timeOfEvent;

    // Finding all words with timestamp >= -35 from the start time.
    for (int j = i ; j >= 1 ; --j){
        __builtin_prefetch(&buffer[j-1]);
        __builtin_prefetch(&buffer[j-2]);
        start = j;
        int64_t timediff = ClockStart - (*buffer)[j-1].timestamp;
        if (std::abs(timediff) > 35){ // If the "next" (j-1) word is to early, stop the loop.
            start = j;
            break;
        }
    }

    for (unsigned int j = i ; j < buffer->GetSize()-1 ; ++j){
        __builtin_prefetch(&buffer[j+1]);
        __builtin_prefetch(&buffer[j+2]);
        int64_t timediff = (*buffer)[j+1].timestamp - ClockStart;
        if (std::abs(timediff) > 150){ // if the "next" word (j+1) is to late, stop the loop.
            stop = j;
            break;
        } else if (std::abs(timediff) >= 5 && end < i){
                end = j;
            }
    }

    if (end > i){
        n_data = end;
        curr_Buf = end+1;
    } else {
        n_data = stop;
        curr_Buf = stop+1; // Where to continue the search for E strip words.
    }
    event.length = stop - start + 1;
    event.words.reset(new word_t[stop-start+1]);
    event.timestamp = timeOfEvent;

    // Setting the data words to the event structure.
    for (int j = start ; j < stop + 1 ; ++j){
        __builtin_prefetch(&event.words[j-start+1],1);
        __builtin_prefetch(&buffer[j+1]);
        event.words[j-start] = (*buffer)[j];
    }

    return true;
}

bool Unpacker::UnpackOneEvent(Event &event, int &n_data)
{
    for (unsigned int i = curr_Buf ; i < buffer->GetSize() ; ++i){
        __builtin_prefetch(&buffer[i+1]);
        // Hard coding of the addresses of E strips for telescope A for improved speed.
        if ((*buffer)[i].address >= 384 && (*buffer)[i].address <= 399){
            // "Trigger" time.
            int64_t tEvent = (*buffer)[i].timestamp;
            event.Reset();
            event.trigTel = 'A'; // To notify what telescope was triggered.
            return ExtractEvent(event, n_data, i, tEvent);
        // Hard coding of the addresses of E strips for telescope B for improved speed.
        } else if ((*buffer)[i].address >= 480 && (*buffer)[i].address <= 495){
            // "Trigger" time.
            int64_t tEvent = (*buffer)[i].timestamp;
            event.Reset();
            event.trigTel = 'B';
            return ExtractEvent(event, n_data, i, tEvent);
        }
    }
    n_data = buffer->GetSize() + 1;
    return false; // End of buffer is reached.
}
