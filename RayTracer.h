#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <chrono>
#include <iostream>
#include <limits>
#include <png++/png.hpp>
#include <random>
#include <string>
#include <thread>
#include "AABB.h"
#include "OrthoCamera.h"
#include "PerspCamera.h"
#include "HittableList.h"
#include "Image.h"
#include "LightList.h"
#include "Ray.h"
#include "vec3.h"

class RayTracer {
 public:
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

  Image *image_;
  void render();

  bool ortho;

  bool antialias_;
  int aa_factor_;

  bool multithread;

  bool BVH;

  PerspCamera cam;

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

  std::default_random_engine generator;
};

#endif
