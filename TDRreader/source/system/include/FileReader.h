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

#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <memory>

class WordBuffer;
class Detectors;

/*!
 * \class FileReader
 * \brief Class for reading TDR buffers from file.
 * \details This class reads the TDR buffers from binary files. It removes all CFD values from the stream. It also decodes the binary format to
 * the WordBuffer type.
 * \author Vetle W. Ingeberg
 * \date 2015-2016
 * \copyright GNU Public License v. 3
 * \todo It should be created an interface class for FileReader and then this class should inherit its functionality. This will make it easier to implement
 * support for other file formats other than the TDR format.
 */
class FileReader {

public:

	//! Initilizer
	FileReader();

	//! Destructor
	~FileReader();

	//! Open a file.
	/*! \return true if opening was successful.
	 */
	bool Open(std::string filename,		/*!< Name of the file to open.	*/
			  unsigned int seekpos=0	/*!< Where to open the file at.	*/);

	//! Read a single buffer from the file.
    /*! \return 1 for new buffer, 0 if EOF is reached or -1 if
     *  an error was encountred.
	 */
    int Read(WordBuffer *buffer	/*!< Buffer to put the data. 	*/);

    //! Retrive the error flag.
    /*! \return The error flag.
     */
    bool IsError() const
    	{ return errorflag; }

    //! Set the detector list to look up.
    void SetDetectors(Detectors *det);

private:
	//! The object for reading files.
	FILE * file;

	//! Close the file.
	void Close();

	//! Structure of the data header for each buffer.
	typedef struct {
    	char     header_id[8];          /*!< Contains the string  EBYEDATA. */
    	uint32_t header_sequence;       /*!< Within the file. */
    	uint16_t  header_stream;        /*!< Data acquisition stream number (in the range 1=>4). */
    	uint16_t  header_tape;          /*!< =1. */
    	uint16_t  header_MyEndian;      /*!< Written as a native 1 by the tape server */
    	uint16_t  header_DataEndian;    /*!< Written as a native 1 in the hardware structure of the data following. */
    	uint32_t header_dataLen;        /*!< Total length of useful data following the header in bytes. */
	} DATA_HEADER_T;

	//! Object to store a new buffer header.
	DATA_HEADER_T header;

	//! Number of buffers that have been read.
	bool errorflag;

	//! Number of buffers read in file.
    int num_buf;

    //! Variable to keep the top 20 bits of the 48-bit timestamp.
	uint64_t topTime;

	//! Variable to keep the detectors list.
    std::unique_ptr<Detectors> detectors;

	//! Function to swap the words such that:
    /*! word 0 -> word 1 and word 1 -> word 0
     *  word 2 -> word 3 and word 3 -> word 2
     *  word 4 -> word 5 and word 5 -> word 4
     *  etc.
     *  \return A buffer where all words are swaped.
     */
    void SwapWords(uint32_t* Old_buf /*!< Buffer containing unswaped words.	*/
     			 , int length 		 /*!< Number of words in buffer. 		*/);

    //! Decodes the words and returning a word-buffer.
    /*! \return A word_t buffer of all the words that are decoded.
     */
    bool DecodeWords(WordBuffer* buffer,
                     uint32_t *buf, /*!< Buffer containing datawords to decode. */
                     int length		/*!< Number of words in the buffer.			*/);

    //! Help function to find the fist sunc signal.
    void FindFirstSync(uint32_t *buf, int length);

};

#endif // FILEREADER_H
