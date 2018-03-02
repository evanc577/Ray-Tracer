#ifndef PIXEL_H
#define PIXEL_H

class Pixel {
    public:
        Pixel();
        Pixel(float red, float green, float blue, float alpha);

        float r;
        float g;
        float b;
        float a;
};

#endif
