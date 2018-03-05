CXX = clang++
CXXFLAGS = -O3 -g -std=c++1y -Wall -Wextra -Wpedantic
EXE= mp1
OBJS = main.o RayTracer.o Image.o lodepng.o LightList.o AmbientLight.o \
	   DirectionalLight.o HittableList.o Sphere.o Plane.o CheckerPlane.o
LD = clang++
LDFLAGS = -std=c++1y -lpthread

all : $(EXE)

$(EXE) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXE)

main.o : main.cpp RayTracer.o Image.o Sphere.o Plane.o CheckerPlane.o \
	LightList.o AmbientLight.o DirectionalLight.o
	$(CXX) -c $(CXXFLAGS) main.cpp

RayTracer.o : RayTracer.cpp RayTracer.h Image.o lodepng.o vec3.h Ray.h \
	HittableList.o LightList.h
	$(CXX) -c $(CXXFLAGS) RayTracer.cpp

Image.o : Image.h Image.cpp vec3.h
	$(CXX) -c $(CXXFLAGS) Image.cpp

lodepng.o : lodepng/lodepng.cpp lodepng/lodepng.h
	$(CXX) -c $(CXXFLAGS) lodepng/lodepng.cpp

LightList.o : LightList.h LightList.cpp Light.h
	$(CXX) -c $(CXXFLAGS) LightList.cpp

AmbientLight.o : AmbientLight.h AmbientLight.cpp Light.h
	$(CXX) -c $(CXXFLAGS) AmbientLight.cpp

DirectionalLight.o : DirectionalLight.h DirectionalLight.cpp Light.h
	$(CXX) -c $(CXXFLAGS) DirectionalLight.cpp

HittableList.o : HittableList.h HittableList.cpp Hittable.h Light.h
	$(CXX) -c $(CXXFLAGS) HittableList.cpp

Sphere.o : Sphere.h Sphere.cpp Hittable.h
	$(CXX) -c $(CXXFLAGS) Sphere.cpp

Plane.o : Plane.h Plane.cpp Hittable.h
	$(CXX) -c $(CXXFLAGS) Plane.cpp

CheckerPlane.o : CheckerPlane.h CheckerPlane.cpp Plane.o
	$(CXX) -c $(CXXFLAGS) CheckerPlane.cpp

clean :
	rm -rf *.o $(EXE)
