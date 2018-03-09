#include <getopt.h>
#include <iostream>
#include <string>
#include "RayTracer.h"
#include "Sphere.h"
#include "Plane.h"
#include "CheckerPlane.h"
#include "Triangle.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"

void printHelp(int argc, char *argv[]) {
    (void)argc;
    std::cout << "Usage: " << argv[0] << " [OPTION]...\n";
    std::cout << "Generates an image via ray tracing\n\n";
    std::cout << "List of options:\n";
    std::cout << "    --help           show this help message\n";
    std::cout << "-d, --default        use default parameters\n";
    std::cout << "-p, --projection     set projection {persp|ortho}"
        " (default: ortho)\n";
    std::cout << "-a, --antialias      enable antialiasing and set"
        " AA factor"
        " (default: no antialiasing)\n";
    std::cout << "-o, --output         set output file"
        " (default: output.png)\n";
    std::cout << "-w, --width          set output image width in px"
        " (default: 1000px)\n";
    std::cout << "-h, --height         set output image height in px"
        " (default: 1000px)\n";
    std::cout << "-m, --multithread    enable multithreading"
        " (default disabled)\n";
    std::cout << "-s, --sRGB           transform to sRGB color space"
        " (experimental) (default disabled)\n";
    std::cout << "\nExamples:\n";
    std::cout << argv[0] << " -d\n";
    std::cout << argv[0] << " -w 200 -h 200\n";
    std::cout << argv[0] << " -w 3840 -h 2160 -p persp "
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
    bool sRGB = false;

    // parse options
    if (argc == 1) {
        printHelp(argc,argv);
        return 0;
    }
    while (true) {
        if (use_default) break;
        static struct option long_options[] =
        {
            {"help",        no_argument,       0, 0},
            {"default",     no_argument,       0, 'd'},
            {"projection",  required_argument, 0, 'p'},
            {"antialias",   required_argument, 0, 'a'},
            {"output",      required_argument, 0, 'o'},
            {"width",       required_argument, 0, 'w'},
            {"height",      required_argument, 0, 'h'},
            {"multithread", no_argument,       0, 'm'},
            {"sRGB",        no_argument,       0, 's'},
            {0, 0, 0, 0}
        };
        int option_index = 0;
        int c = getopt_long(argc, argv, "dp:a:o:w:h:ms",
                long_options, &option_index);
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
                }
                else if (s == "persp" || s == "perspective") {
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
            case 's':
                sRGB = true;
                break;
            case '?':
                return 1;
            default:
                printHelp(argc, argv);
                return 0;

        }
    }

    RayTracer r(width,height);

    // set options
    r.antialias_ = antialias;
    r.aa_factor_ = aa_factor;
    r.ortho = ortho;
    r.multithread = multithread;
    r.sRGB = sRGB;


    // Lights
    AmbientLight l1;
    l1.ia = glm::vec3(0.1,0.1,0.1);
    r.addLight(&l1);

    DirectionalLight l2;
    l2.direction = glm::normalize(glm::vec3(0.5,-1,0));
    l2.id = glm::vec3(0.9,0.9,0.9);
    l2.is = glm::vec3(0.9,0.9,0.9);
    r.addLight(&l2);

    DirectionalLight l3;
    l3.direction = glm::normalize(glm::vec3(-0.5,-1,-0.5));
    l3.id = glm::vec3(0.9,0.9,0.9);
    l3.is = glm::vec3(0.9,0.9,0.9);
    r.addLight(&l3);

    // DirectionalLight l4;
    // l4.direction = glm::normalize(glm::vec3(0,-1,0));
    // l3.id = glm::vec3(0.9,0.9,0.9);
    // l3.is = glm::vec3(0.5,0.5,0.5);
    // r.addLight(&l4);


    // Spheres
    Sphere s1(glm::vec3(0,0,-1.1), 0.2);
    s1.ka = glm::vec3(1,0.1,0.1);
    s1.kd = glm::vec3(1,0.1,0.1);
    s1.ks = 0.5f*glm::vec3(1,1,1);
    s1.alpha = 10;
    r.addHittable(&s1);

    Sphere s2(glm::vec3(0.7,0,-2), 0.2);
    s2.ka = glm::vec3(0,1,0);
    s2.kd = glm::vec3(0,1,0);
    s2.ks = 0.5f*glm::vec3(1,1,1);
    s2.alpha = 10;
    r.addHittable(&s2);

    Sphere s3(glm::vec3(-0.4,-0.1,-1), 0.15);
    s3.ka = glm::vec3(1,0.1,1);
    s3.kd = glm::vec3(1,0.1,1);
    s3.ks = 0.5f*glm::vec3(1,1,1);
    s3.alpha = 10;
    r.addHittable(&s3);

    Sphere s4(glm::vec3(0.8,-0.4,-1.9), 0.3);
    s4.ka = glm::vec3(0.1,0.9,0.9);
    s4.kd = glm::vec3(0.1,0.9,0.9);
    s4.ks = 0.05f*glm::vec3(1,1,1);
    s4.alpha = 30;
    r.addHittable(&s4);

    // Planes
    CheckerPlane p1(glm::vec3(0,-0.5,-2), glm::normalize(glm::vec3(0,1,0.4)));
    p1.ka1 = glm::vec3(0.9,0.9,0.9);
    p1.ka2 = glm::vec3(0.1,0.1,0.1);
    p1.kd1 = p1.ka1;
    p1.kd2 = p1.ka2;
    p1.ks1 = glm::vec3(0.1,0.1,0.1);
    p1.ks2 = p1.ks1;
    p1.alpha1 = 3;
    p1.alpha2 = 3;
    p1.tile_size = 0.2;
    r.addHittable(&p1);

    // Triangles
    Triangle t1(glm::vec3(-0.25,-0.25,-1.2), glm::vec3(0.2,-0.25,-0.8),
            glm::vec3(0,0.35,-1.3));
    t1.ka = glm::vec3(0.9,0.9,0);
    t1.kd = glm::vec3(0.9,0.9,0);
    t1.ks = glm::vec3(0.5,0.5,0.5);
    r.addHittable(&t1);

    // render and write image to file
    r.render();
    r.outputImage(filename);

    return 0;
}
