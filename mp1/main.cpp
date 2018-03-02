#include <iostream>
#include <string>
#include "lodepng/lodepng.h"
#include "RayTracer.h"

int main(int argc, char *argv[]) {
    std::string filename = argc > 1 ? argv[1] : "test.png";
    RayTracer r;
    r.setImageSize(1000,1000);
    r.render();
    r.outputImage(filename);
    return 0;
}
