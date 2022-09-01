CXX?=g++
#CXXFLAGS= -o -O1 -ftree-vectorize -std=c++11 -mfpu=neon -pg --coverage
CXXFLAGS= -o -O1 -ftree-vectorize -std=c++11 -pg 
#CXXFLAGS=-O2 -std=c++11 -mfpu=neon -pg

EXEC=hw3

INCLUDEDIR=include
SRCDIR=src
OBJDIR=objs

OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv4`
OPENCV_LIBS = `pkg-config --libs opencv4`

LIBS=-lc $(OPENCV_LIBS)
INCLUDES=-I$(INCLUDEDIR) $(OPENCV_INCLUDE_PATH)

SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(EXEC) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(EXEC)
	rm -f $(OBJDIR)/*.o
	rm -f ./image_outputs/*.tif
