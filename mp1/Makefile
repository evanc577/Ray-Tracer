CXX = clang++
CXXFLAGS = -O -g -std=c++1y -Wall -Wextra -Wpedantic
EXE= mp1
OBJS = Pixel.o Image.o RayTracer.o lodepng.o main.o

all : $(EXE)

$(EXE) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXE)

Pixel.o : Pixel.h Pixel.cpp
	$(CXX) -c $(CXXFLAGS) Pixel.cpp

Image.o : Image.h Image.cpp Pixel.o
	$(CXX) -c $(CXXFLAGS) Image.cpp

RayTracer.o : RayTracer.cpp RayTracer.h Image.o lodepng.o
	$(CXX) -c $(CXXFLAGS) RayTracer.cpp

main.o : main.cpp RayTracer.o Image.o
	$(CXX) -c $(CXXFLAGS) main.cpp

lodepng.o : lodepng/lodepng.cpp lodepng/lodepng.h
	$(CXX) -c $(CXXFLAGS) lodepng/lodepng.cpp

clean : 
	rm -rf *.o *.png mp1
