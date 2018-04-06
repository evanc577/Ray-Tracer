# Basic Ray Tracer

Objects:

* Spheres

* Planes (and checkered planes)

* Triangles

Light Types:

* Ambient lights

* Directional lights

* Point lights

Lighting (via Phong Reflection Model):

* Ambient

* Diffuse

* Specular

* HDR tone-mapping via gamma-compression

Projections:

* Orthographic

* Perspective

Other Goodies:

* Acceleration of large number of objects via a bounded volume hierarchy

* Arbitrary image size support

* Anti-aliasing support (arbitrary positive integers)

* Multithreading support

* Specify output file

## Sample Images

Perspective image

![Perspective Image](images/perspective.png)

Orthographic image

![Orthographic Image](images/orthographic.png)

## Bounding Volume Hierarchy

Scenes with many objects can be accelerated with a BVH

![BVH Scalability](images/bvh_chart.png)

Test with 2^18 spheres

![BVH Image](images/bvh.png)
