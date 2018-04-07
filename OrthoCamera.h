#ifndef ORTHOCAMERA_H_
#define ORTHOCAMERA_H_

#include "Camera.h"

class OrthoCamera : public Camera {
 public:
  OrthoCamera();
  OrthoCamera(vec3 origin, vec3 direction, vec3 vup, float width, float height);
  void set_camera(vec3 origin, vec3 direction, vec3 vup, float width,
                  float height);
  virtual Ray get_ray(float u, float v) const;

  vec3 direction;
  vec3 lower_left;
  vec3 horz;
  vec3 vert;
};
#endif
