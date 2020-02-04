#include "../include/entities.h"

#define OFFSET 1e-5

bool Sphere::intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const {
    glm::dvec3 oc = ray.origin - pos;
    
    float a = glm::dot(ray.dir, ray.dir);
    float b = glm::dot(oc, ray.dir);
    float c = glm::dot(oc, oc) - radius * radius;

    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        double t = (-b - sqrt(discriminant)) / a;
        glm::dvec3 tmp = ray.origin + ray.dir * t;

        if (glm::distance(tmp, ray.origin) < glm::distance(intersect, ray.origin) && t > 0) {
            intersect = tmp;
            normal = glm::normalize(tmp - pos);
            return true;
        }

        t = (-b + sqrt(discriminant)) / a;
        tmp = ray.origin + ray.dir * t;
        if (glm::distance(tmp, ray.origin) < glm::distance(intersect, ray.origin) && t > 0) {
            intersect = tmp;
            normal = glm::normalize(tmp - pos);
            return true;
        }      
    }
    return false;
};

// BoundingBox Sphere::boundingBox() const {
//     return BoundingBox(pos - glm::dvec3(radius, radius, radius), pos + glm::dvec3(radius, radius, radius));
// };


bool Triangle::intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const {
    // Möller-Trumbore algorithm

    glm::dvec3 v0v1 = _v1 - _v0;
    glm::dvec3 v0v2 = _v2 - _v0;
    glm::dvec3 pvec = glm::cross(ray.dir, v0v2);

    double det = glm::dot(v0v1, pvec);
    if (fabs(det) < OFFSET) return false;

    double invdet = 1.0 / det;

    glm::dvec3 tvec = ray.origin - _v0;
    double u = glm::dot(tvec, pvec) * invdet;
    if (u < 0 || u > 1) return false;

    glm::dvec3 qvec = glm::cross(tvec, v0v1);
    double v = glm::dot(ray.dir, qvec) * invdet;
    if (v < 0 || u + v > 1) return false;

    double t = glm::dot(v0v2, qvec) * invdet;
    if (t < OFFSET) return false;

    glm::dvec3 tmp = ray.origin + ray.dir * t;
    if (glm::distance(tmp, ray.origin) > glm::distance(intersect, ray.origin)) return false;
    
    intersect = tmp;
    normal = _normal;

    return true;
};

// BoundingBox Triangle::boundingBox() const {
//     glm::dvec3 min, max;
//     min = {std::min(_v0[0], _v1[0]), std::min(_v0[1], _v1[1]), std::min(_v0[2], _v1[2])};
//     min = {std::min(min[0], _v2[0]), std::min(min[1], _v2[1]), std::min(min[2], _v2[2])};

//     max = {std::max(_v0[0], _v1[0]), std::max(_v0[1], _v1[1]), std::max(_v0[2], _v1[2])};
//     max = {std::max(max[0], _v2[0]), std::max(max[1], _v2[1]), std::max(max[2], _v2[2])};
    
//     if (min.x == max.x) { min.x -= OFFSET; max.x += OFFSET; }
//     if (min.y == max.y) { min.y -= OFFSET; max.y += OFFSET; }
//     if (min.z == max.z) { min.z -= OFFSET; max.z += OFFSET; }
    
//     return BoundingBox(min, max);
// };


bool Quad::intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const {
    for (int i = 0; i < _objectlist.size(); ++i) {
        if (_objectlist[i]->intersect(ray, intersect, normal)) {
            return true;
        }
    }
    return false;
};

// BoundingBox Quad::boundingBox() const {
//     glm::dvec3 min, max;
//     min = {std::min(_v0[0], _v1[0]), std::min(_v0[1], _v1[1]), std::min(_v0[2], _v1[2])};
//     min = {std::min(min[0], _v2[0]), std::min(min[1], _v2[1]), std::min(min[2], _v2[2])};
//     min = {std::min(min[0], _v3[0]), std::min(min[1], _v3[1]), std::min(min[2], _v3[2])};

//     max = {std::max(_v0[0], _v1[0]), std::max(_v0[1], _v1[1]), std::max(_v0[2], _v1[2])};
//     max = {std::max(max[0], _v2[0]), std::max(max[1], _v2[1]), std::max(max[2], _v2[2])};
//     max = {std::max(max[0], _v3[0]), std::max(max[1], _v3[1]), std::max(max[2], _v3[2])};

//     if (min.x == max.x) { min.x -= OFFSET; max.x += OFFSET; }
//     if (min.y == max.y) { min.y -= OFFSET; max.y += OFFSET; }
//     if (min.z == max.z) { min.z -= OFFSET; max.z += OFFSET; }
//     return BoundingBox(min, max);
// };