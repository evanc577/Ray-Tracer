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
  ~RayTracer();

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

  // read .obj file
  void read_file(const std::string& filename);
  vec3 mesh_ka, mesh_kd, mesh_ks;
  float mesh_alpha;

  void generate_vertex_normals();

  void render();

  Image *image_;

  bool ortho;

  bool antialias_;
  int aa_factor_;

  bool multithread;

  bool BVH;

  PerspCamera p_cam;
  OrthoCamera o_cam;

  // enable/disable smooth (Phong) shading
  bool smooth;

  // for tone mapping
  float gamma;

 private:
  void _clear();

  // for use with multithreading
  void renderSection(int thread, int num_threads);

  vec3 color(const Ray &r);

  HittableList hittables;
  AABB hittables_BVH;

  LightList lights;

  std::vector<vertex> vertices;
  std::vector<Triangle>* triangles;

  std::default_random_engine generator;
};

#endif
