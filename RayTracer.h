#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <png++/png.hpp>
#include <random>
#include <string>
#include <thread>
#include "AABB.h"
#include "HittableList.h"
#include "Image.h"
#include "LightList.h"
#include "OrthoCamera.h"
#include "PerspCamera.h"
#include "Ray.h"
#include "Triangle.h"
#include "Sphere.h"
#include "vec3.h"

class RayTracer {
 public:
  struct vertex {
    vec3 point;
    vec3 normal;
  };
  RayTracer();
  RayTracer(unsigned w, unsigned h);
  RayTracer(const RayTracer &other);
  ~RayTracer();
  RayTracer &operator=(const RayTracer &other);

  void outputImage(std::string filename) const;

  void test() const;
  void setImage(const Image &im);
  void setImageSize(unsigned w, unsigned h);

  void addHittable(Hittable *h);
  void clearHittables();

  void addLight(Light *l);
  void clearLights();

  void set_ortho_cam(vec3 origin, vec3 direction, vec3 vup, float width,
                     float height);
  void set_persp_cam(vec3 origin, vec3 direction, vec3 vup, float aspect,
                     float hfov);

  void read_file(const std::string& filename);

  void generate_vertex_normals();

  Image *image_;
  void render();

  bool ortho;

  bool antialias_;
  int aa_factor_;

  bool multithread;

  bool BVH;

  PerspCamera p_cam;
  OrthoCamera o_cam;

 private:
  void _copy(const RayTracer &other);
  void _clear();

  void renderSection(int thread, int num_threads);

  void linearToSRGB();
  float sRGB_max;

  vec3 color(const Ray &r);
  vec3 randomInUnitSphere();

  HittableList hittables;
  AABB hittables_BVH;

  LightList lights;


  std::vector<vertex> vertices;
  std::vector<Triangle>* triangles;

  std::default_random_engine generator;
};

#endif
