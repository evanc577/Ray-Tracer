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
               " (default: persp)\n";
  std::cout << "-a, --antialias [factor]          enable antialiasing and set"
               " AA factor. Note # of samples is factor^2.\n"
               "                                  (default: no antialiasing)\n";
  std::cout << "-i, --input [filename]            .obj file to import\n";
  std::cout << "-s, --smooth                      enable smooth shading for "
               "imported meshes\n";
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
}

int main(int argc, char *argv[]) {
  bool use_default = false;
  std::string output_filename = "output.png";
  std::string input_filename;
  bool input_mesh = false;
  std::string s;
  bool ortho = false;
  int width = 1000;
  int height = 1000;
  bool antialias = false;
  int aa_factor = 1;
  bool multithread = false;
  bool BVH = false;
  bool smooth = false;

  bool genspheres = false;
  unsigned num_spheres = 0;
  float sphere_radius = 0.01f;

  vec3 camera_location(0, 0.12, 0.22);
  vec3 camera_direction(0, 0, -1);
  vec3 camera_up(0, 1, 0);
  float camera_fov = 70.0f;
  float camera_width = 5;
  float camera_height = 5;

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
        {"input", required_argument, 0, 'i'},
        {"smooth", no_argument, 0, 's'},
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 'h'},
        {"multithread", no_argument, 0, 'm'},
        {"bvh", no_argument, 0, 'b'},
        {"genspheres", required_argument, 0, 'g'},
        {"radius", required_argument, 0, 'r'},
        {0, 0, 0, 0}};
    int option_index = 0;
    int c = getopt_long(argc, argv, "dp:a:o:i:sw:h:mbg:r:", long_options,
                        &option_index);
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
        output_filename = std::string(optarg);
        break;
      case 'i':
        input_filename = std::string(optarg);
        input_mesh = true;
        break;
      case 's':
        smooth = true;
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
  r.smooth = smooth;

  if (ortho) camera_height = camera_width * height / width;

  // Lights
  AmbientLight l1;
  l1.ia = 0.1f * vec3(1, 1, 1);
  r.addLight(&l1);

  DirectionalLight l2;
  l2.direction = normalize(vec3(-1, -1, 1));
  l2.id = 0.9f * vec3(1, 1, 1);
  l2.is = 0.9f * vec3(1, 1, 1);
  r.addLight(&l2);

  DirectionalLight l3;
  l3.direction = normalize(vec3(-1, -1, -1));
  l3.id = 0.4f * vec3(1, 1, 1);
  l3.is = 0.4f * vec3(1, 1, 1);
  r.addLight(&l3);

  DirectionalLight l4;
  l4.direction = normalize(vec3(1, -1, -1));
  l4.id = 0.3f * vec3(1, 1, 1);
  l4.is = 0.3f * vec3(1, 1, 1);
  r.addLight(&l4);

  DirectionalLight l5;
  l5.direction = normalize(vec3(1, -1, 1));
  l5.id = 0.2f * vec3(1, 1, 1);
  l5.is = 0.2f * vec3(1, 1, 1);
  r.addLight(&l5);

  // PointLight l4;
  // l4.point = vec3(0,0,0);
  // l4.id = 1.0f * vec3(1, 1, 1);
  // l4.is = l4.id;
  // r.addLight(&l4);

  std::vector<Sphere> spheres;

  if (genspheres) {
    srand(100);
    for (unsigned i = 0; i < num_spheres; ++i) {
      float LO = -1.0f + sphere_radius;
      float HI = 1.0f + sphere_radius;
      float x = LO + static_cast<float>(rand()) /
                         (static_cast<float>(RAND_MAX / (HI - LO)));
      float y = LO + static_cast<float>(rand()) /
                         (static_cast<float>(RAND_MAX / (HI - LO)));
      float z = LO + static_cast<float>(rand()) /
                         (static_cast<float>(RAND_MAX / (HI - LO)));
      Sphere s(vec3(x, y, z), sphere_radius);
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

  // Triangle t1(vec3(1,0,0.5), vec3(0,1,0), vec3(1,1,0));
  // t1.ka = vec3(0,1,0);
  // t1.kd = t1.ka;
  // t1.ks = t1.ka;
  // r.addHittable(&t1);

#if 0
  // Planes
  CheckerPlane p1(vec3(0, -.55, 0), normalize(vec3(0, 1, 0)));
  p1.ka1 = vec3(0.9, 0.9, 0.9);
  p1.ka2 = vec3(0.1, 0.1, 0.1);
  p1.kd1 = p1.ka1;
  p1.kd2 = p1.ka2;
  p1.ks1 = vec3(0.1, 0.1, 0.1);
  p1.ks2 = p1.ks1;
  p1.alpha1 = 3;
  p1.alpha2 = 3;
  p1.tile_size = 5;
  r.addHittable(&p1);
#endif

  r.mesh_kd = vec3(0.6, 0.6, 0.6);
  r.mesh_ka = vec3(0.05, 0.05, 0.05);
  r.mesh_ks = vec3(0.6, 0.6, 0.6);

  if (input_mesh) {
    r.read_file(input_filename);
  }

  // set cameras
  float w = 1;
  float h = float(height) / width;
  r.set_persp_cam(camera_location, camera_direction, camera_up, float(w) / h,
                  camera_fov);
  r.set_ortho_cam(camera_location, camera_direction, camera_up, camera_width,
                  camera_height);

  r.gamma = 0.7;

  // render and write image to file
  r.render();
  r.outputImage(output_filename);

  return 0;
}
