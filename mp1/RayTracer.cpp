#include <chrono>
#include <random>
#include "RayTracer.h"

RayTracer::RayTracer() : image(NULL) {
    setImageSize(100,100);
    antialias = false;
    aa_factor = 1;
}

RayTracer::RayTracer(unsigned w, unsigned h) :image(NULL) {
    setImageSize(w,h);
    antialias = false;
    aa_factor = 1;
}

RayTracer::RayTracer(const RayTracer &other) {
    _copy(other);
}

RayTracer::~RayTracer() {
    _clear();
}

RayTracer & RayTracer::operator=(const RayTracer &other) {
    if (this != &other) {
        _clear();
        _copy(other);
    }
    return *this;
}

void RayTracer::outputImage(std::string filename) const {
    if (!image) {
        std::cout << __FUNCTION__ << "() error, no image\n";
        return;
    }

    std::vector<unsigned char> temp;
    temp.resize(image->width_ * image->height_ * 4);
    for (int i = 0; i < image->width_; i++) {
        for (int j = 0; j < image->height_; j++) {
            vec3 &p = image->getPixel(i,j);
            temp[4*(image->height_-j-1)*image->width_ + 4*i + 0] =
                (unsigned char)(255*p.r());
            temp[4*(image->height_-j-1)*image->width_ + 4*i + 1] =
                (unsigned char)(255*p.g());
            temp[4*(image->height_-j-1)*image->width_ + 4*i + 2] =
                (unsigned char)(255*p.b());
            temp[4*(image->height_-j-1)*image->width_ + 4*i + 3] =
                (unsigned char)(255);
        }
    }

    unsigned error = lodepng::encode(filename, temp, image->width_,
            image->height_);
    if (error) {
        std::cout << "encoder error " << error << ": "<<
            lodepng_error_text(error) << std::endl;
    }
}

void RayTracer::setImageSize(unsigned w, unsigned h) {
    _clear();
    image = new Image(w, h);
}

void RayTracer::render(bool ortho) {
    //TODO implement render()
    if (!image) {
        std::cout << __FUNCTION__ << "() error, image size not set\n";
        return;
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    float lower = -0.5;
    float left = -0.5*float(image->width_)/float(image->height_);

    vec3 corner(left, lower, -0.0);
    vec3 corner_persp(left, lower, -1.0);
    vec3 horz(-2.0*left, 0.0, 0.0);
    vec3 vert(0.0, 1.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    for (int j = 0; j < image->height_; j++) {
        for (int i = 0; i < image->width_; i++) {
            float u = float(i)/float(image->width_);
            float v = float(j)/float(image->height_);

            if (!antialias) {
                Ray r;
                if (ortho) {
                    r.setOrigin(corner + u*horz + v*vert);
                    r.setDirection(corner_persp + u*horz + v*vert);
                }
                else {
                    r.setOrigin(origin);
                    r.setDirection(corner_persp + u*horz + v*vert);
                }
                vec3 col = color(r);
                std::cout << col.e[0] << " " << col.e[1] << " " << col.e[2] << "\n";
                vec3 &pix = image->getPixel(i,j);
                pix = col;
            }

            else {
                vec3 col(0,0,0);
                for (int k = 0; k < aa_factor; k++) {
                    float range_begin = float(k)/float(aa_factor);
                    float range_end = float(k+1)/float(aa_factor);
                    std::uniform_real_distribution<float>
                        dist(range_begin, range_end);
                    float temp_u = u + dist(generator)/float(image->width_);
                    float temp_v = v + dist(generator)/float(image->height_);

                    // std::cout << range_begin << " " << range_end << " ";
                    // std::cout << temp_u << " " << temp_v << "\n";

                    Ray r;
                    if (ortho) {
                        r.setOrigin(corner + temp_u*horz + temp_v*vert);
                        r.setDirection(corner_persp + temp_u*horz + temp_v*vert);
                    }
                    else {
                        r.setOrigin(origin);
                        r.setDirection(corner_persp + temp_u*horz + temp_v*vert);
                    }

                    // vec3 temp = color(r);
                    // std::cout << temp.e[0] << " " << temp.e[1] << " " <<
                        // temp.e[2] << "\n";

                    col += color(r);
                }
                float aa_float = float(aa_factor);
                col /= vec3(aa_float, aa_float, aa_float);
                // std::cout << col.e[0] << " " << col.e[1] << " " << col.e[2] << "\n";
                vec3 &pix = image->getPixel(i,j);
                pix = col;
            }
        }
    }
}

vec3 RayTracer::color(const Ray &r) {
    float t = hitSphere(vec3(0,0,-1), 0.2, r);
    if (t > 0.0) {
        // vec3 N = unit_vector(r.location(t) - vec3(0,0,-1));
        // return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1);
        return vec3(1,0,0);
    }
    return vec3(0,0,0);
}

float RayTracer::hitSphere(const vec3 &center, float radius, const Ray &r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) return -1.0;
    else return (-b - sqrt(discriminant)) / (2.0*a);
}

void RayTracer::_clear() {
    if (image) {
        delete image;
        image = NULL;
    }
}

void RayTracer::_copy(const RayTracer &other) {
    image = other.image;
}
