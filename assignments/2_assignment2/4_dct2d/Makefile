CXX?=g++
CXXFLAGS=-O1

BUILD_DIR := build/
BIN := dct2d 
CXXSRCS := $(wildcard src/*.cxx)

OBJS := $(CXXSRCS:src/%.cxx=$(BUILD_DIR)%.o)

CXXFLAGS += -Iinclude -I/usr/include/opencv4/

LDFLAGS += -llapack -lblas -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio
.PHONY: all
all:	$(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN) $(LDFLAGS)

$(BUILD_DIR)%.o: src/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf build/*
	rm -f $(BIN)
