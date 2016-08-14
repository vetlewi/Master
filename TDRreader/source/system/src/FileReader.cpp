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
 * \file FileReader.cpp
 * \brief Implementation of FileReader.
 * \author Vetle W. Ingeberg
 * \date 2015-2016
 * \copyright GNU Public License v. 3
 */

#include "FileReader.h"

#include <cstdio>

#include "TDRWordBuffer.h"
#include "DetectorRead.h"

#define TEST_FILEREAD 0 // Set to 1 for testing, 0 elsewise.


FileReader::FileReader() :
	file( 0 ),
    errorflag( true ),
    num_buf( 0 ),
    topTime( 0 )
{
}

// ########################################################################

FileReader::~FileReader()
{
	Close();
}

// ########################################################################

void FileReader::Close()
{
	if ( file ){
		std::fclose( file );
		file = 0;
        num_buf = 0;
        topTime = 0;
	}
}

// ########################################################################

bool FileReader::Open(std::string filename, unsigned int want)
{
	Close();
	file = std::fopen( filename.c_str(), "rb");
    errorflag = (file == 0)
        || std::fseek(file, want, SEEK_SET) != (int)want;
    return !errorflag;
}

// ########################################################################

void FileReader::SetDetectors(Detectors* det)
{
    detectors = std::unique_ptr<Detectors>(det);
}

int FileReader::Read(WordBuffer* buffer)
{
	if ( errorflag || (!file) )
        return -1;

	if (num_buf != 0){
		char foo[40];
        if (fread(&foo, sizeof(char), 40, file) != 40){
            return 0;
        }
	}
	
    if (fread(&header, sizeof(DATA_HEADER_T), 1, file) != 1)
        return 0;

    unsigned int length = header.header_dataLen/4;
    uint32_t *words = new uint32_t[length];

    unsigned int have =  0;
    int now = -1;
    while ( have < length){
        now = -1;
        if ( file )
            now = fread(words+have, sizeof(uint32_t), length-have, file);
        if (now <= 0) {
            errorflag = (now < 0 || (have != 0 && have != length));
            delete[] words;
            Close();
            return errorflag ? -1 : 0;
        } else {
            have += now;
        }
    }
    num_buf += 1;
    SwapWords(words, length);    
    DecodeWords(buffer, words, length);

    delete[] words;
    return 1;
}

// ########################################################################

void FileReader::SwapWords(uint32_t* buf, int length)
{
    uint32_t tmp1, tmp2;

    for (int i = 0 ; i < length - 1 ; ++i){
        __builtin_prefetch(&buf[i+1]);
        __builtin_prefetch(&buf[i+2]);
        tmp1 = buf[i];
        tmp2 = buf[i+1];
        buf[i] = tmp2;
        buf[i+1] = tmp1;
    }
}

// ########################################################################

bool FileReader::DecodeWords(WordBuffer* buffer, uint32_t *buf, int length){

    if (topTime == 0) FindFirstSync(buf, length);

    uint32_t word;
    uint16_t tmpAddress;
    bool *keep = new bool[length];
    unsigned int found = 0;
    for (int i = 0 ; i < length ; ++i){
        __builtin_prefetch(&buf[i+1]);
        word = buf[i];
        keep[i] = false;
        if (word >> 30 == 3){
        	word = word&0x0fffffff;
            tmpAddress = word >> 16;
            tmpAddress = tmpAddress&16;
            if (tmpAddress == 0){
                keep[i] = true;
                found += 1;
            }
        	//if (detectors->Find(word >> 16).getType() != Dtype::CFD){
        	//	keep[i] = true;
            //	found += 1;
        	//}
        }
    }

    word_t *words = new word_t[found];

    int decoded = 0;
    for (int i = 0 ; i < length ; ++i){
        __builtin_prefetch(&buf[i+1]);
        word = buf[i];
        if ( word >> 30 == 2 ){
            word = word&0x000fffff;
            topTime = word;
            topTime = topTime << 28;
        } else if ( keep[i] ) {
            word = word&0x0fffffff;
            words[decoded].address = word >> 16;
            words[decoded].adcdata = word;
            word = buf[i+1];
            if ( word >> 28 == 0 ){
                words[decoded].timestamp = topTime + word;
                i += 1;
                decoded += 1;
            }
        }
    }
    delete[] keep;
    if (found != buffer->GetSize())
        buffer->Resize(found);
    buffer->SetBuffer(words);
    return true;
}

// ########################################################################
void FileReader::FindFirstSync(uint32_t *buf, int length)
{
    uint32_t word;
    for (int i = 0 ; i < length ; ++i){
        __builtin_prefetch(&buf[i+1]);
        word = buf[i];
        if ( word >> 30 == 2){
            word = word&0x000fffff;
            topTime = word;
            topTime = topTime << 28;
            break;
        }
    }
}

// ########################################################################
// ########################################################################

#if TEST_FILEREAD

#include <iostream>

int main(){

	WordBuffer *testBuffer = new WordBuffer;

	FileReader filereader;

    if (!filereader.Open("R31_0")){
		std::cout << "Managed to open the file." << std::endl;
        int haveRead = 0;
        for (int i = 0 ; i < 400 ; i++){
            haveRead += 1;
            if (filereader.Read(testBuffer) == 0)
                std::cout << "We have read " << haveRead << " buffers!" << std::endl;
            else
               std::cout << "We had some error while reading buffer no.: " << haveRead << std::endl;
        }
	}
}

#endif
