#include <iostream>
#include <string>
#include "RayTracer.h"
#include "Sphere.h"

int main(int argc, char *argv[]) {
    std::string filename = "output.png";
    bool ortho = true;
    int i = 1;
    int width = 1000;
    int height = 1000;
    bool antialias = false;
    int aa_factor = 1;

    // parse options
    while (i < argc) {
        std::string arg = argv[i];

        // help flag
        if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [OPTION]...\n";
            std::cout << "Generates an image via ray tracing\n\n";
            std::cout << "List of options:\n";
            std::cout << "    --help           show this help message\n";
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
            return 0;
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

        // invalid flags
        else {
            std::cout << argv[0] << ": invalid option " << arg << "\n";
            return 1;
        }

        i++;
    }

    RayTracer r(width,height);

    Sphere s1(vec3(0,0,-1), 0.2);
    Sphere s2(vec3(0.7,0,-2), 0.2);
    Sphere s3(vec3(-0.5,-0.1,-1), 0.2);
    Sphere s4(vec3(0,-100.2,-1), 100);

    r.addHittable(&s1);
    r.addHittable(&s2);
    r.addHittable(&s3);
    r.addHittable(&s4);

    r.antialias_ = antialias;
    r.aa_factor_ = aa_factor;

    std::cout << "rendering image...\n";
    r.render(ortho);
    std::cout << "done\n";

    std::cout << "writing to " << filename << "...\n";
    r.outputImage(filename);
    std::cout << "done\n";

    return 0;
}
