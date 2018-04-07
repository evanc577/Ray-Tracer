#ifndef CAMERA_H_
#define CAMERA_H_

#include "Ray.h"

class Camera {
 public:
   virtual Ray get_ray(float u, float v) const = 0;
};

#endif
