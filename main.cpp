#include <getopt.h>
#include <iostream>
#include <string>
#include "AmbientLight.h"
#include "CheckerPlane.h"
#include "DirectionalLight.h"
#include "Plane.h"
#include "PointLight.h"
#include "RayTracer.h"
#include "Sphere.h"
#include "Triangle.h"

void printHelp(int argc, char *argv[]) {
  (void)argc;
  std::cout << "Usage: " << argv[0] << " [OPTION]...\n";
  std::cout << "Generates an image via ray tracing\n\n";
  std::cout << "List of options:\n";
  std::cout << "    --help                        show this help message\n";
  std::cout << "-d, --default                     use default parameters\n";
  std::cout << "-p, --projection [persp|ortho]    set projection"
               " (default: ortho)\n";
  std::cout << "-a, --antialias [factor]          enable antialiasing and set"
               " AA factor"
               " (default: no antialiasing)\n";
  std::cout << "-o, --output [filename]           set output file"
               " (default: output.png)\n";
  std::cout << "-w, --width [pixels]              set output image width"
               " (default: 1000px)\n";
  std::cout << "-h, --height [pixels]             set output image height"
               " (default: 1000px)\n";
  std::cout << "-m, --multithread                 enable multithreading"
               " (default disabled)\n";
  std::cout << "-b, --bvh                         enable bounding volume"
               " hierarchy (default disabled)\n";
  std::cout << "-g, --genspheres [number]         generate a cube of spheres"
               " (default disabled)\n";
  std::cout << "-r, --radius [float]              radius of spheres"
               " (default 0.01) (to be used with --genspheres)\n";
  std::cout << "\nExamples:\n";
  std::cout << argv[0] << " -d\n";
  std::cout << argv[0] << " -w 200 -h 200\n";
  std::cout << argv[0]
            << " -w 3840 -h 2160 -p persp "
               "-o persp.png -ma 16\n";
}

int main(int argc, char *argv[]) {
  bool use_default = false;
  std::string filename = "output.png";
  std::string s;
  bool ortho = true;
  int width = 1000;
  int height = 1000;
  bool antialias = false;
  int aa_factor = 1;
  bool multithread = false;
  bool BVH = false;

  bool genspheres = false;
  unsigned num_spheres = 0;
  float sphere_radius = 0.01f;

  // parse options
  if (argc == 1) {
    printHelp(argc, argv);
    return 0;
  }
  while (true) {
    if (use_default) break;
    static struct option long_options[] = {
        {"help", no_argument, 0, 0},
        {"default", no_argument, 0, 'd'},
        {"projection", required_argument, 0, 'p'},
        {"antialias", required_argument, 0, 'a'},
        {"output", required_argument, 0, 'o'},
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 'h'},
        {"multithread", no_argument, 0, 'm'},
        {"bvh", no_argument, 0, 'b'},
        {"genspheres", required_argument, 0, 'g'},
        {"radius", required_argument, 0, 'r'},
        {0, 0, 0, 0}};
    int option_index = 0;
    int c =
        getopt_long(argc, argv, "dp:a:o:w:h:mbg:r:", long_options, &option_index);
    if (c == -1) break;
    switch (c) {
      case 0:
        printHelp(argc, argv);
        return 0;
      case 'd':
        use_default = true;
        break;
      case 'p':
        s = std::string(optarg);
        if (s == "ortho" || s == "orthographic") {
          ortho = true;
          break;
        } else if (s == "persp" || s == "perspective") {
          ortho = false;
          break;
        }
        std::cout << argv[0] << ": invalid projection\n";
        return 1;
      case 'a':
        antialias = true;
        aa_factor = std::stoi(optarg);
        if (aa_factor < 1) {
          std::cout << argv[0] << ": invalid AA factor\n";
          return 1;
        }
        break;
      case 'o':
        filename = std::string(optarg);
        break;
      case 'w':
        width = std::stoi(optarg);
        if (width > 0) break;
        std::cout << argv[0] << ": invalid width\n";
        return 1;
      case 'h':
        height = std::stoi(optarg);
        if (height > 0) break;
        std::cout << argv[0] << ": invalid height\n";
        return 1;
      case 'm':
        multithread = true;
        break;
      case 'b':
        BVH = true;
        break;
      case 'g':
        genspheres = true;
        num_spheres = std::stoi(optarg);
        break;
      case 'r':
        sphere_radius = std::stof(optarg);
        break;
      case '?':
        return 1;
      default:
        printHelp(argc, argv);
        return 0;
    }
  }

  RayTracer r(width, height);

  // set options
  r.antialias_ = antialias;
  r.aa_factor_ = aa_factor;
  r.ortho = ortho;
  r.multithread = multithread;
  r.BVH = BVH;

  // Lights
  AmbientLight l1;
  l1.ia = 0.4f * vec3(1, 1, 1);
  r.addLight(&l1);

  DirectionalLight l2;
  l2.direction = normalize(vec3(0.5, -1, -1));
  l2.id = 0.9f * vec3(1, 1, 1);
  l2.is = 0.9f * vec3(1, 1, 1);
  r.addLight(&l2);

  std::vector<Sphere> spheres;

  if (genspheres) {
    srand(100);
    for (unsigned i = 0; i < num_spheres; i++) {
      float LO = -1.0f;
      float HI = 1.0f;
      float x = LO + static_cast<float>(rand()) /
        (static_cast<float>(RAND_MAX / (HI - LO)));
      float y = LO + static_cast<float>(rand()) /
        (static_cast<float>(RAND_MAX / (HI - LO)));
      float z = LO + static_cast<float>(rand()) /
        (static_cast<float>(RAND_MAX / (HI - LO)));
      Sphere s(vec3(x, y, z - 3), sphere_radius);
      LO = 0.2f;
      HI = 1.0f;
      float r = LO + static_cast<float>(rand()) /
        (static_cast<float>(RAND_MAX / (HI - LO)));
      float g = LO + static_cast<float>(rand()) /
        (static_cast<float>(RAND_MAX / (HI - LO)));
      float b = LO + static_cast<float>(rand()) /
        (static_cast<float>(RAND_MAX / (HI - LO)));
      s.ka = vec3(r, g, b);
      s.kd = s.ka;
      s.alpha = 3;
      s.ks = 0.5f * vec3(1, 1, 1);
      spheres.push_back(s);
    }

    for (auto &s : spheres) {
      r.addHittable(&s);
    }
  }

  // Planes
  if (false) {
    CheckerPlane p1(vec3(0, -1.2, -2), normalize(vec3(0, 1, 0)));
    p1.ka1 = vec3(0.9, 0.9, 0.9);
    p1.ka2 = vec3(0.1, 0.1, 0.1);
    p1.kd1 = p1.ka1;
    p1.kd2 = p1.ka2;
    p1.ks1 = vec3(0.1, 0.1, 0.1);
    p1.ks2 = p1.ks1;
    p1.alpha1 = 3;
    p1.alpha2 = 3;
    p1.tile_size = 0.2;
    r.addHittable(&p1);
  }

  // render and write image to file
  r.render();
  r.outputImage(filename);

  return 0;
}
