#include <iostream>
#include <string>
#include "RayTracer.h"
#include "Sphere.h"
#include "Plane.h"
#include "CheckerPlane.h"
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
    std::cout << "\nExamples:\n";
    std::cout << argv[0] << " -d\n";
    std::cout << argv[0] << " -w 200 -h 200\n";
    std::cout << argv[0] << " -w 3840 -h 2160 -a 16 -p persp "
        "-m -o persp.png\n";
}

int main(int argc, char *argv[]) {
    std::string filename = "output.png";
    bool ortho = true;
    int i = 1;
    int width = 1000;
    int height = 1000;
    bool antialias = false;
    int aa_factor = 1;
    bool multithread = false;

    // parse options
    if (argc == 1) {
        printHelp(argc, argv);
        return 0;
    }
    while (i < argc) {
        std::string arg = argv[i];

        // help flag
        if (arg == "--help") {
            printHelp(argc, argv);
            return 0;
        }

        // default flag
        if (arg == "-d" || arg == "--default") {
            break;
        }

        // projection flag
        else if (arg == "-p" || arg == "--projection") {
            i++;
            if (i == argc) {
                std::cout << argv[0] << ": no projection specified\n";
                return 1;
            }
            else {
                arg = argv[i];
                if (arg == "ortho" || arg == "orthographic") {
                    ortho = true;
                }
                else if (arg == "persp" || arg == "perspective") {
                    ortho = false;
                }
                else {
                    std::cout << argv[0] << ": invalid projection\n";
                    return 1;
                }
            }
        }

        // output filename flag
        else if (arg == "-o") {
            i++;
            if (i == argc) {
                std::cout << argv[0] << ": no output file specified\n";
                return 1;
            }
            else {
                arg = argv[i];
                filename = arg;
            }
        }

        // output width flag
        else if (arg == "-w" || arg == "--width") {
            i++;
            if (i == argc) {
                std::cout << argv[0] << ": no width specified\n";
                return 1;
            }
            else {
                arg = argv[i];
                width = stoi(arg);
            }
        }

        // output height flag
        else if (arg == "-h" || arg == "--height") {
            i++;
            if (i == argc) {
                std::cout << argv[0] << ": no height specified\n";
                return 1;
            }
            else {
                arg = argv[i];
                height = stoi(arg);
            }
        }

        // antialias flag
        else if (arg == "-a" || arg == "--antialias") {
            i++;
            if (i == argc) {
                std::cout << argv[0] << ": no antialias factor specified\n";
                return 1;
            }
            else {
                arg = argv[i];
                aa_factor = stoi(arg);
                antialias = true;
            }
        }

        // multithread flag
        else if (arg == "-m" || arg == "--multithread") {
            multithread = true;
        }

        // invalid flags
        else {
            std::cout << argv[0] << ": invalid option " << arg << "\n";
            return 1;
        }

        i++;
    }

    RayTracer r(width,height);

    // set options
    r.antialias_ = antialias;
    r.aa_factor_ = aa_factor;
    r.ortho = ortho;
    r.multithread = multithread;


    // Lights
    AmbientLight l1;
    l1.ia = vec3(0.2,0.2,0.2);
    r.addLight(&l1);

    DirectionalLight l2;
    l2.direction = unit_vector(vec3(1,-0.4,-1));
    l2.color = vec3(0.6,0.6,0.6);
    r.addLight(&l2);

    DirectionalLight l3;
    l3.direction = unit_vector(vec3(-1,-0.4,-1));
    l3.color = vec3(0.3,0.3,0.3);
    r.addLight(&l3);

    DirectionalLight l4;
    l4.direction = unit_vector(vec3(0,-1,0));
    l3.color = vec3(0.9,0.9,0.9);
    r.addLight(&l4);


    // Spheres
    Sphere s1(vec3(0,0,-1), 0.2);
    s1.ka = vec3(1,0.2,0.3);
    s1.kd = vec3(1,0.2,0.3);
    r.addHittable(&s1);

    Sphere s2(vec3(0.7,0,-2), 0.2);
    s2.ka = vec3(0,1,0);
    s2.kd = vec3(0,1,0);
    r.addHittable(&s2);

    Sphere s3(vec3(-0.5,-0.1,-1), 0.4);
    s3.ka = vec3(0.2,0.1,1);
    s3.kd = vec3(0.2,0.1,1);
    r.addHittable(&s3);

    Sphere s4(vec3(1.4,-0.4,-1.9), 0.3);
    s4.ka = vec3(0.1,0.9,0.9);
    s4.kd = vec3(0.1,0.9,0.9);
    r.addHittable(&s4);

    // Planes
    CheckerPlane p1(vec3(0,-2,0), unit_vector(vec3(0,1,0.8)));
    p1.ka1 = vec3(0.9,0.9,0.9);
    p1.ka2 = vec3(0.1,0.1,0.1);
    p1.kd1 = p1.ka1;
    p1.kd2 = p1.ka2;

    p1.tile_size = 0.5;
    p1.kd = vec3(0.7,1,0.8);
    r.addHittable(&p1);


    // render and write image to file
    r.render();
    r.outputImage(filename);

    return 0;
}
