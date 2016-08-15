TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

linux-g++ {
    ROOTFLAGS = $$system( root-config --cflags )
    ROOTLIBS = $$system( root-config --glibs )

    QMAKE_CXXFLAGS = $$ROOTFLAGS -s -Wall -W -std=c++11 -O3 -march=native
    QMAKE_CFLAGS += $$ROOTFLAGS -O3
    LIBS += $$ROOTLIBS
}

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
    ROOTFLAGS = $$system( root-config --cflags )
    ROOTLIBS = $$system( root-config --glibs )
    QMAKE_CFLAGS += $$ROOTFLAGS -g -std=c++11
    QMAKE_CXXFLAGS += $$ROOTFLAGS -g -std=c++11
    LIBS += $$ROOTLIBS
}


INCLUDEPATH +=  source \
                source/export/include \
                source/core/include \
                source/system/include \
                source/userroutine/include \
                source/types/include
SOURCES += source/main.cpp \
        source/export/src/RootWriter.cpp \
        source/export/src/MamaWriter.cpp \
        source/core/src/DetectorRead.cpp \
        source/core/src/OfflineSorting.cpp \
        source/core/src/TDRRoutine.cpp \
        source/core/src/Unpacker.cpp \
        source/system/src/aptr.ipp \
        source/system/src/RateMeter.cpp \
        source/system/src/FileReader.cpp \
        source/system/src/IOPrintf.cpp \
        source/system/src/MTFileBufferFetcher.cpp \
        source/system/src/STFileBufferFetcher.cpp \
        source/types/src/Histograms.cpp \
        source/types/src/Histogram1D.cpp \
        source/types/src/Histogram2D.cpp \
    	source/types/src/Parameters.cpp \
        source/types/src/GainParameters.cpp \
        source/types/src/TelescopeGate.cpp \
        source/types/src/Telescope.cpp \
        source/types/src/TimeGate.cpp \
        source/types/src/Polynomial.cpp \
        source/userroutine/src/UserSort.cpp \
        source/userroutine/src/UserSingles.cpp


HEADERS += source/DefineFile.h \
        source/export/include/RootWriter.h \
        source/export/include/MamaWriter.h \
        source/core/include/DetectorRead.h \
        source/core/include/TDRRoutine.h \
        source/core/include/OfflineSorting.h \
        source/core/include/UserRoutine.h \
        source/core/include/Unpacker.h \
        source/system/include/Logger.h \
        source/system/include/RateMeter.h \
        source/system/include/TDRWordBuffer.h \
        source/system/include/FileReader.h \
        source/system/include/aptr.h \
        source/system/include/IOPrintf.h \
        source/system/include/BufferFetcher.h \
        source/system/include/FileBufferFetcher.h \
        source/system/include/MTFileBufferFetcher.h \
        source/system/include/STFileBufferFetcher.h \
        source/types/include/Event.h \
        source/types/include/Histograms.h \
        source/types/include/Histogram1D.h \
        source/types/include/Histogram2D.h \
        source/types/include/Parameters.h \
        source/types/include/GainParameters.h \
        source/types/include/TelescopeGate.h \
        source/types/include/Telescope.h \
        source/types/include/TimeGate.h \
        source/types/include/Polynomial.h \
        source/userroutine/include/UserSort.h \
        source/userroutine/include/UserSingles.h
