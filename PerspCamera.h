#ifndef PERSPCAMERA_H_
#define PERSPCAMERA_H_

#include "Camera.h"

class PerspCamera : public Camera {
 public:
  PerspCamera();
  PerspCamera(vec3 origin, vec3 direction, vec3 vup, float aspect, float hfov);
  void set_camera(vec3 origin, vec3 direction, vec3 vup, float aspect,
                  float hfov);
  virtual Ray get_ray(float u, float v) const;

 private:
  vec3 origin;
  vec3 lower_left;
  vec3 horz;
  vec3 vert;
};
#endif
