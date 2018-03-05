CXX = clang++
CXXFLAGS = -O3 -g -std=c++1y -Wall -Wextra -Wpedantic
EXE= mp1
OBJS = Image.o RayTracer.o lodepng.o main.o LightList.o
LD = clang++
LDFLAGS = -std=c++1y -lpthread

all : $(EXE)

$(EXE) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXE)

Image.o : Image.h Image.cpp vec3.h
	$(CXX) -c $(CXXFLAGS) Image.cpp

RayTracer.o : RayTracer.cpp RayTracer.h Image.o lodepng.o vec3.h Ray.h \
	HittableList.h LightList.h
	$(CXX) -c $(CXXFLAGS) RayTracer.cpp

main.o : main.cpp RayTracer.o Image.o Sphere.h Plane.h CheckerPlane.h \
	LightList.o AmbientLight.h DirectionalLight.h
	$(CXX) -c $(CXXFLAGS) main.cpp

lodepng.o : lodepng/lodepng.cpp lodepng/lodepng.h
	$(CXX) -c $(CXXFLAGS) lodepng/lodepng.cpp

LightList.o : LightList.h LightList.cpp Light.h
	$(CXX) -c $(CXXFLAGS) LightList.cpp

clean :
	rm -rf *.o *.png mp1
