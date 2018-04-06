#ifndef ORTHOCAMERA_H_
#define ORTHOCAMERA_H_

#include "Camera.h"

class OrthoCamera : public Camera {
 public:
   OrthoCamera(float center, float width);
   virtual Ray get_ray(float u, float v) const;

   vec3 center;
   vec3 width;
   vec3 height;
};
#endif
