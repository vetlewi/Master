##############################################################
#
# Makefile for building: TDRreader.
# Modify the compiler, falgs, etc. to fit you computer setup.
# The software requires ROOT 5/6 to compile.
##############################################################



#### Compiler, flags, etc.
#### These may have to be configured, depending on the system.

## Compilers
CC			=	gcc
CXX			=	g++
LINK		=	g++
RM_FILE		=	rm -f

## ROOT flags/libs.
RLIBS		=	$(shell root-config --ldflags --libs)
RFLAGS		=	$(shell root-config --cflags)

CFLAGS		=	-O3
CXXFLAGS	=	-Wall -W -pipe -pthread -stdlib=libc++ -m64 -std=c++11 -O3


#### Files

### Edit this to where you want the binary files.
TARGETDIR	=	./build

TARGET		=	$(TARGETDIR)/TDRreader

SOURCES		=	source/main.cpp \
				source/export/src/RootWriter.cpp \
				source/export/src/MamaWriter.cpp \
				source/core/src/DetectorRead.cpp \
				source/core/src/OfflineSorting.cpp \
				source/core/src/TDRRoutine.cpp \
				source/core/src/Unpacker.cpp \
				source/system/src/RateMeter.cpp \
				source/system/src/FileReader.cpp \
				source/system/src/IOPrintf.cpp \
				source/system/src/MTFileBufferFetcher.cpp \
				source/system/src/STFileBufferFetcher.cpp \
				source/types/src/Histograms.cpp \
				source/types/src/Histogram1D.cpp \
				source/types/src/Histogram2D.cpp \
				source/types/src/GainParameters.cpp \
				source/types/src/TelescopeGate.cpp \
				source/types/src/Telescope.cpp \
				source/types/src/TimeGate.cpp \
				source/types/src/Polynomial.cpp \
				source/userroutine/src/UserSort.cpp \
				source/userroutine/src/UserSingles.cpp

OBJECTS		=	$(TARGETDIR)/main.o \
				$(TARGETDIR)/RootWriter.o \
				$(TARGETDIR)/MamaWriter.o \
				$(TARGETDIR)/DetectorRead.o \
				$(TARGETDIR)/OfflineSorting.o \
				$(TARGETDIR)/TDRRoutine.o \
				$(TARGETDIR)/Unpacker.o \
				$(TARGETDIR)/RateMeter.o \
				$(TARGETDIR)/FileReader.o \
				$(TARGETDIR)/IOPrintf.o \
				$(TARGETDIR)/MTFileBufferFetcher.o \
				$(TARGETDIR)/STFileBufferFetcher.o \
				$(TARGETDIR)/Histograms.o \
				$(TARGETDIR)/Histogram1D.o \
				$(TARGETDIR)/Histogram2D.o \
				$(TARGETDIR)/Parameters.o \
				$(TARGETDIR)/GainParameters.o \
				$(TARGETDIR)/TelescopeGate.o \
				$(TARGETDIR)/Telescope.o \
				$(TARGETDIR)/TimeGate.o \
				$(TARGETDIR)/Polynomial.o \
				$(TARGETDIR)/UserSort.o \
				$(TARGETDIR)/UserSingles.o

INCPATH		=	-I. \
				-I./source \
				-I./source/export/include \
				-I./source/core/include \
				-I./source/system/include \
				-I./source/types/include \
				-I./source/userroutine/include

all: $(TARGET) $(DUMMY)

DUMMY:
	mkdir $(TARGETDIR)

$(TARGET): $(OBJECTS)
	$(LINK) $(LFLAGS) $(RLIBS) -o $(TARGET) $(OBJECTS)

$(TARGETDIR)/main.o: ./source/main.cpp
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/main.o ./source/main.cpp

$(TARGETDIR)/RootWriter.o: ./source/export/src/RootWriter.cpp ./source/export/include/RootWriter.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/RootWriter.o ./source/export/src/RootWriter.cpp

$(TARGETDIR)/MamaWriter.o: ./source/export/src/MamaWriter.cpp ./source/export/include/MamaWriter.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/MamaWriter.o ./source/export/src/MamaWriter.cpp

$(TARGETDIR)/DetectorRead.o: ./source/core/src/DetectorRead.cpp ./source/core/include/DetectorRead.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/DetectorRead.o ./source/core/src/DetectorRead.cpp

$(TARGETDIR)/OfflineSorting.o: ./source/core/src/OfflineSorting.cpp ./source/core/include/OfflineSorting.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/OfflineSorting.o ./source/core/src/OfflineSorting.cpp

$(TARGETDIR)/TDRRoutine.o: ./source/core/src/TDRRoutine.cpp ./source/core/include/TDRRoutine.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/TDRRoutine.o ./source/core/src/TDRRoutine.cpp

$(TARGETDIR)/Unpacker.o: ./source/core/src/Unpacker.cpp ./source/core/include/Unpacker.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/Unpacker.o ./source/core/src/Unpacker.cpp

$(TARGETDIR)/RateMeter.o: ./source/system/src/RateMeter.cpp ./source/system/include/RateMeter.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/RateMeter.o ./source/system/src/RateMeter.cpp

$(TARGETDIR)/FileReader.o: ./source/system/src/FileReader.cpp ./source/system/include/FileReader.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/FileReader.o ./source/system/src/FileReader.cpp

$(TARGETDIR)/IOPrintf.o: ./source/system/src/IOPrintf.cpp ./source/system/include/IOPrintf.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/IOPrintf.o ./source/system/src/IOPrintf.cpp

$(TARGETDIR)/MTFileBufferFetcher.o: ./source/system/src/MTFileBufferFetcher.cpp ./source/system/include/MTFileBufferFetcher.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/MTFileBufferFetcher.o ./source/system/src/MTFileBufferFetcher.cpp

$(TARGETDIR)/STFileBufferFetcher.o: ./source/system/src/STFileBufferFetcher.cpp ./source/system/include/STFileBufferFetcher.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/STFileBufferFetcher.o ./source/system/src/STFileBufferFetcher.cpp

$(TARGETDIR)/Histograms.o: ./source/types/src/Histograms.cpp ./source/types/include/Histograms.h 
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/Histograms.o ./source/types/src/Histograms.cpp

$(TARGETDIR)/Histogram1D.o: ./source/types/src/Histogram1D.cpp ./source/types/include/Histogram1D.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/Histogram1D.o ./source/types/src/Histogram1D.cpp

$(TARGETDIR)/Histogram2D.o: ./source/types/src/Histogram2D.cpp ./source/types/include/Histogram2D.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/Histogram2D.o ./source/types/src/Histogram2D.cpp

$(TARGETDIR)/Parameters.o: ./source/types/src/Parameters.cpp ./source/types/include/Parameters.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/Parameters.o ./source/types/src/Parameters.cpp

$(TARGETDIR)/GainParameters.o: ./source/types/src/GainParameters.cpp ./source/types/include/GainParameters.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/GainParameters.o ./source/types/src/GainParameters.cpp

$(TARGETDIR)/TelescopeGate.o: ./source/types/src/TelescopeGate.cpp ./source/types/include/TelescopeGate.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/TelescopeGate.o ./source/types/src/TelescopeGate.cpp

$(TARGETDIR)/Telescope.o: ./source/types/src/Telescope.cpp ./source/types/include/Telescope.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/Telescope.o ./source/types/src/Telescope.cpp

$(TARGETDIR)/TimeGate.o: ./source/types/src/TimeGate.cpp ./source/types/include/TimeGate.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/TimeGate.o ./source/types/src/TimeGate.cpp

$(TARGETDIR)/Polynomial.o: ./source/types/src/Polynomial.cpp ./source/types/include/Polynomial.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/Polynomial.o ./source/types/src/Polynomial.cpp

$(TARGETDIR)/UserSort.o:	./source/userroutine/src/UserSort.cpp ./source/userroutine/include/UserSort.h
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/UserSort.o ./source/userroutine/src/UserSort.cpp

$(TARGETDIR)/UserSingles.o: ./source/userroutine/src/UserSingles.cpp ./source/userroutine/include/UserSingles.h 
	$(CXX) -c $(CXXFLAGS) $(RFLAGS) $(INCPATH) -o $(TARGETDIR)/UserSingles.o ./source/userroutine/src/UserSingles.cpp


clean:
	-$(RM_FILE) $(TARGET)
	-$(RM_FILE) $(OBJECTS)