CXX = clang++
CXXFLAGS = -O3 -g -std=c++1y -Wall -Wextra -Wpedantic
EXE= mp1
OBJS = Image.o RayTracer.o lodepng.o main.o vec3.o Ray.o
LD = clang++
LDFLAGS = -std=c++1y -lpthread

all : $(EXE)

$(EXE) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXE)

vec3.o : vec3.h vec3.cpp
	$(CXX) -c $(CXXFLAGS) vec3.cpp

Ray.o : Ray.h Ray.cpp vec3.o
	$(CXX) -c $(CXXFLAGS) Ray.cpp

Image.o : Image.h Image.cpp vec3.o
	$(CXX) -c $(CXXFLAGS) Image.cpp

RayTracer.o : RayTracer.cpp RayTracer.h Image.o lodepng.o
	$(CXX) -c $(CXXFLAGS) RayTracer.cpp

main.o : main.cpp RayTracer.o Image.o vec3.o
	$(CXX) -c $(CXXFLAGS) main.cpp

lodepng.o : lodepng/lodepng.cpp lodepng/lodepng.h
	$(CXX) -c $(CXXFLAGS) lodepng/lodepng.cpp

clean :
	rm -rf *.o *.png mp1
