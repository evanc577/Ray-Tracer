#include "OrthoCamera.h"

OrthoCamera::OrthoCamera() {
  lower_left = vec3(-1, -1, 0);
  direction = vec3(0, 0, -1);
  horz = vec3(2, 0, 0);
  vert = vec3(0, 2, 0);
}

OrthoCamera::OrthoCamera(vec3 o, vec3 d, vec3 vup, float width, float height) {
  set_camera(o, d, vup, width, height);
}

void OrthoCamera::set_camera(vec3 o, vec3 d, vec3 vup, float width,
                             float height) {
  vec3 u, v;
  direction = normalize(d);
  u = normalize(cross(vup, direction));
  v = cross(direction, u);

  lower_left = o - width / 2 * u - height / 2 * v + direction;
  horz = width * u;
  vert = height * v;
}

Ray OrthoCamera::get_ray(float u, float v) const {
  return Ray(lower_left + u * horz + v * vert, direction);
}
