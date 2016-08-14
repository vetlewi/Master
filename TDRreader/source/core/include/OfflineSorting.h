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

#ifndef OFFLINESORTING_H
#define OFFLINESORTING_H

#include <string>

#include "RateMeter.h"
#include "Unpacker.h"

class Detectors;
class FileBufferFetcher;
class WordBuffer;

class UserRoutine;


/*!
 *  \class     OfflineSorting
 *  \brief     A class for handling offline sorting.
 *  \details   The class handles the flow of data from reading the batch script defining the files to read and passing data that are read
 *             to the correct classes.
 *  \author    Vetle W. Ingeberg
 *  \version   0.8.0
 *  \date      2015-2016
 *  \copyright GNU Public License v. 3
 */
class OfflineSorting {
public:

	//! Initialize sorting session.
	OfflineSorting(UserRoutine& us	/*!< The sorting routine to use. */);

    //! Overloaded.
    OfflineSorting(UserRoutine &us,         /*!< The sorting routine to use.    */
                   FileBufferFetcher *bf    /*!< The BufferFetcher to use.      */);

	//! Destructor.
	~OfflineSorting();

	//! Read commands in the batch file.
	void Read_batchfile(const std::string& input_file /*!< Name of the input file to process. */);

    // Convinient helper for a short main() routine.
	/*! Can be use as following:
     *  \code
	 *  int main(int argc, char* argv[])
	 *  {
	 *		return OfflineSorting::Run(new UserRoutine, argc, argv);
	 	}
     * \endcode
	 */
    static int Run(UserRoutine* ur, /*!< User implementation of the event sorting routine.  */
                   int argc,        /*!< No. of command line arguments.                     */
                   char* argv[]     /*!< The command line arguments.                        */);

    //! Overload of Run.
    /*! Can be used to give another file buffer reader class than the default. Is used as following:
     *  \code
     *  int main(int argc, char* argv[])
     *  {
     *      return OfflineSorting::Run(new UserRoutine, new FileBufferFetcher, argc, argv);
     *  }
     *  \endcode
     */
    static int Run(UserRoutine *ur,         /*!< User implementation of the event sorting routine.          */
                   FileBufferFetcher *bf,   /*!< The BufferFetcher class to be used for fetching buffers.   */
                   int argc,                /*!< No. of command line arguments.                             */
                   char *argv[]             /*!< The command line arguments.                                */);

	//! Sort one file.
	/*! \return true if everything is okey; else false.
	 */
    bool SortFile(const std::string filename,   /*!< The name of the file to read.  */
                  int buf_start=0,              /*!< Where to begin.                */
                  int buf_end=-1                /*!< Where to end.                  */);

protected:
	//! Sort one buffer.
	/*! \return true if everything is okey; else false.
	 */
    bool SortBuffer(const WordBuffer* buffer /*!< The buffer to sort. */);

private:
    //! Variable to contain user routine.
    UserRoutine& userSort;

    //! Write to command line.
    bool is_tty;

    //! Filereader object.
    std::unique_ptr<FileBufferFetcher> bufferFetcher;

	//! Pointer to detectors list.
    std::unique_ptr<Detectors> detectors;

	//! Ratemeter for buffer rate messurements.
    RateMeter rateMeter;

    //! Object performing unpacking of the data.
    Unpacker unpacker;

	//! Handles 'export' commands.
	/*! 
	 *  \return true if everything is okey; else false.
	 */
	bool export_command(std::istream& icmd);

	//! Handles 'data' commands.
	/*! Reads the parameters and performs sorting of the file.
	 *  \return true if everything is okey; else false.
	 */
	bool data_command(std::istream& icmd);

	//! Create list of detectors.
	/*! Reads the detectors file name and initiates a new
	 *  detectors listing.
	 *  \return true if everything is okey; else false.
	 */
	bool detectors_command(std::istream& icmd);

	//! Handles a command.
	/*! \return true if everything is okey; else false.
	 */
	bool next_command(const std::string& cmd /*!< The command. */);

	//! Get next command from the input stream.
	/*! Input will be the batch file.
     *  \return true if successful.
	 */
	bool next_commandline(std::istream& in, std::string& cmd_line);

    //! Variable to count the number of events extracted in each buffer.
    int nEvents;

    //! Variable to count the total number of words extracted. For calculating the average event length.
    int nWords;
};

#endif // OFFLINESORTING_H
