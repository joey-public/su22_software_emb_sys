CXX?=g++

#o0
CXXFLAGS=-O0 -std=c++11 
EXEC= ./lab3_fir_o0

#o1
#CXXFLAGS=-O1 -std=c++11 -pg
#EXEC= ./lab3_fir_o1

#o2
#CXXFLAGS=-O2 -std=c++11 -pg
#EXEC= ./lab3_fir_o2

#o3
#CXXFLAGS=-O3 -std=c++11 -pg
#EXEC= ./lab3_fir_o3

#ofast
#CXXFLAGS=-Ofast -std=c++11 -pg
#EXEC= ./lab3_fir_ofast

INCLUDEDIR=include
SRCDIR=src
OBJDIR=objs

OPENCV_VERSION := $(shell pkg-config --modversion opencv4)

ifeq "$(OPENCV_VERSION)" "4.1.1"
	OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv4`
	OPENCV_LIBS = `pkg-config --libs opencv4`
	CXXFLAGS += -DOPENCV4
else
	OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv`
	OPENCV_LIBS = `pkg-config --libs opencv`
endif

#OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv`
#OPENCV_LIBS = `pkg-config --libs opencv`

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
