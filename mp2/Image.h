#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include "vec3.h"

class Image {
 public:
  Image();
  Image(int w, int h);
  Image(const Image &other);
  ~Image();
  Image &operator=(const Image &other);

  vec3 &getPixel(int x, int y) const;
  bool setPixel(int x, int y, const vec3 &p);

  int width_;
  int height_;

 private:
  std::vector<std::vector<vec3> > *image;
  void _clear();
};

#endif
