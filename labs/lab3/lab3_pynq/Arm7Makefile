CXX?=g++

#o0
#CXXFLAGS=-O0 -std=c++11 -mfpu=neon -pg
#EXEC= builds/o0/lab3_fir_o0

#o1
#CXXFLAGS=-O1 -std=c++11 -mfpu=neon -pg
#EXEC=builds/o1/lab3_fir_o1

#o2
#CXXFLAGS=-O2 -std=c++11 -mfpu=neon -pg
#EXEC=builds/o2/lab3_fir_o2

#o3
#CXXFLAGS=-O3 -std=c++11 -mfpu=neon -pg
#EXEC=builds/o3/lab3_fir_o3

#ofast
CXXFLAGS=-Ofast -std=c++11 -mfpu=neon -pg
EXEC=builds/ofast/lab3_fir_ofast


INCLUDEDIR=include
SRCDIR=src
OBJDIR=objs

OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`

LIBS=-lc $(OPENCV_LIBS)

SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/*.cpp, $(OBJDIR)/*.o, $(SRCS))
INCLUDES=-I$(INCLUDEDIR) $(OPENCV_INCLUDE_PATH)

all: $(SRCS) $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(EXEC) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC)
	rm -f $(OBJDIR)/*.o
	rm gmon.out
