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
