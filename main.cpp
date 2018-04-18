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
               " AA factor. Note # of samples is factor^2.\n"
               "                                  (default: no antialiasing)\n";
  std::cout << "-i, --input [filename]            set input .obj file\n";
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
  std::string output_filename = "output.png";
  std::string input_filename;
  bool input_mesh = false;
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
        {"input", required_argument, 0, 'i'},
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 'h'},
        {"multithread", no_argument, 0, 'm'},
        {"bvh", no_argument, 0, 'b'},
        {"genspheres", required_argument, 0, 'g'},
        {"radius", required_argument, 0, 'r'},
        {0, 0, 0, 0}};
    int option_index = 0;
    int c =
        getopt_long(argc, argv, "dp:a:o:i:w:h:mbg:r:", long_options, &option_index);
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
  l1.ia = 0.1f * vec3(1, 1, 1);
  r.addLight(&l1);

  DirectionalLight l2;
  l2.direction = normalize(vec3(-1, -1, 1));
  l2.id = 0.9f * vec3(1, 1, 1);
  l2.is = 0.9f * vec3(1, 1, 1);
  r.addLight(&l2);

  DirectionalLight l3;
  l3.direction = normalize(vec3(1, -1, -1));
  l3.id = 0.9f * vec3(1, 1, 1);
  l3.is = 0.9f * vec3(1, 1, 1);
  r.addLight(&l3);
  
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

  if (input_mesh) {
    r.read_file(input_filename);
  }

#if 1
  // set cameras
  float w = 1;
  float h = float(height)/width;
  r.set_persp_cam(vec3(2, 0.05, 0), vec3(-1, 0, 0), vec3(0, 1, 0), float(w) / h,
                   70);
  r.set_ortho_cam(vec3(2, 2, 2), vec3(-1, -1, -1), vec3(0, 1, 0), 2, 2);


  // render and write image to file
  r.render();
  r.outputImage(output_filename);
#else
  // set cameras
  float w = 1;
  float h = float(height)/width;
  std::string filename;
  int num_frames = 360;
  int total_zero_pad = 0;
  int temp_outer = num_frames;
  do {
    ++total_zero_pad;
    temp_outer /= 10;
  } while (temp_outer);

  for (int i = 0; i < num_frames; ++i) {
    int zero_pad = 0;
    int temp = i;
    do {
      ++zero_pad;
      temp /= 10;
    } while (temp);
    std::string num = std::to_string(i);
    for (int j = 0; j < total_zero_pad - zero_pad; ++j) {
      num = "0" + num;
    }
    size_t found = output_filename.find(".png");
    filename = output_filename;
    filename.insert(found, num);

    float x_coord = 2 * cos(i * M_PI / 180);
    float z_coord = 2 * sin(i * M_PI / 180);
    vec3 coords(x_coord, 0.05, z_coord);
    vec3 dir = normalize(vec3(0, 0.5, 0) - coords);
    r.set_persp_cam(coords, dir, vec3(0, 1, 0), float(w) / h, 70);
    r.render();
    r.outputImage(filename);
  }
#endif

  return 0;
}
