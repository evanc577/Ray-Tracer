CXX = clang++
CXXFLAGS = -O3 -g -std=c++1y -Wall -Wextra -Wpedantic
EXE = ray-tracer
OBJS = main.o RayTracer.o Image.o LightList.o AmbientLight.o \
	   DirectionalLight.o HittableList.o Sphere.o Plane.o CheckerPlane.o \
	   Triangle.o PointLight.o AABB.o
LD = clang++
LDFLAGS = -std=c++1y -lpthread -lpng

all : $(EXE)

$(EXE) : $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXE)

main.o : main.cpp RayTracer.o Image.o Sphere.o Plane.o CheckerPlane.o \
	Triangle.o LightList.o AmbientLight.o DirectionalLight.o
	$(CXX) -c $(CXXFLAGS) main.cpp

RayTracer.o : RayTracer.cpp RayTracer.h Image.o \
	Ray.h HittableList.o LightList.h
	$(CXX) -c $(CXXFLAGS) RayTracer.cpp

Image.o : Image.h Image.cpp
	$(CXX) -c $(CXXFLAGS) Image.cpp

LightList.o : LightList.h LightList.cpp Light.h
	$(CXX) -c $(CXXFLAGS) LightList.cpp

AmbientLight.o : AmbientLight.h AmbientLight.cpp Light.h
	$(CXX) -c $(CXXFLAGS) AmbientLight.cpp

DirectionalLight.o : DirectionalLight.h DirectionalLight.cpp Light.h
	$(CXX) -c $(CXXFLAGS) DirectionalLight.cpp

PointLight.o : PointLight.h PointLight.cpp Light.h
	$(CXX) -c $(CXXFLAGS) PointLight.cpp

HittableList.o : HittableList.h HittableList.cpp Hittable.h Light.h
	$(CXX) -c $(CXXFLAGS) HittableList.cpp

AABB.o : AABB.h AABB.cpp Hittable.h
	$(CXX) -c $(CXXFLAGS) AABB.cpp

Sphere.o : Sphere.h Sphere.cpp Hittable.h
	$(CXX) -c $(CXXFLAGS) Sphere.cpp

Plane.o : Plane.h Plane.cpp Hittable.h
	$(CXX) -c $(CXXFLAGS) Plane.cpp

CheckerPlane.o : CheckerPlane.h CheckerPlane.cpp Plane.o
	$(CXX) -c $(CXXFLAGS) CheckerPlane.cpp

Triangle.o : Triangle.h Triangle.cpp Hittable.h
	$(CXX) -c $(CXXFLAGS) Triangle.cpp

clean :
	rm -rf *.o $(EXE)
