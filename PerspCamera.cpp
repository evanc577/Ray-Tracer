#include "PerspCamera.h"

PerspCamera::PerspCamera() {
  origin = vec3(0,0,0);
  lower_left = vec3(-1, -1, -1);
  horz = vec3(2,0,0);
  vert = vec3(0,2,0);
}

PerspCamera::PerspCamera(vec3 o, vec3 d, vec3 vup, float aspect, float hfov) {
  set_camera(o, d, vup, aspect, hfov);
}

void PerspCamera::set_camera(vec3 o, vec3 d, vec3 vup, float aspect,
                             float hfov) {
  vec3 u, v, w;
  float theta = hfov * M_PI / 180;
  float half_width = tan(theta / 2);
  float half_height = half_width / aspect;
  origin = o;
  w = normalize(d);
  u = normalize(cross(vup, w));
  v = cross(w, u);

  lower_left = origin - half_width * u - half_height * v + w;
  horz = 2 * half_width * u;
  vert = 2 * half_height * v;
}

Ray PerspCamera::get_ray(float u, float v) const {
  return Ray(origin, lower_left + u * horz + v * vert - origin);
}
