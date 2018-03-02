#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include "Pixel.h"

class Image {
    public:
        Image();
        Image(unsigned w, unsigned h);
        Image(const Image &other);
        ~Image();
        Image & operator=(const Image &other);

        Pixel& getPixel(unsigned x, unsigned y) const;
        bool setPixel(unsigned x, unsigned y, const Pixel &p);

        unsigned width_;
        unsigned height_;
    private:
        std::vector<std::vector<Pixel> > * image;
        void _clear();

};

#endif
