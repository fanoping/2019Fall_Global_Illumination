#pragma once

#include <glm/glm.hpp>

#include "bbox.h"
#include "material.h"
#include "ray.h"

/// A base class for all entities in the scene.
struct Entity {

    constexpr Entity() : material(Material(glm::dvec3(1, 0, 0))) {}
    constexpr Entity(const Material& material) : material(material) {}

    /// Check if a ray intersects the object. The arguments intersect and normal will contain the
    /// point of intersection and its normals.
    virtual bool intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const = 0;

    /// Returns an axis-aligned bounding box of the entity.
    // virtual BoundingBox boundingBox() const = 0;

    // glm::dvec3 pos = {0, 0, 0};
    Material material;
};



struct Sphere: Entity {
    Sphere(const double& r, const glm::dvec3& pos, const Material& m):
        Entity(m), radius(r), pos(pos) {}

    glm::dvec3 pos;
    double radius;

    bool intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const;
    
    // BoundingBox boundingBox() const;
};

struct Triangle: Entity {
    Triangle(): Triangle({0, 0, 6}, {0, 1, 6}, {0, 0, 0}, Material()) {}
    Triangle(const glm::dvec3& v0, const glm::dvec3& v1, const glm::dvec3& v2, const Material& material): Entity(material), _v0(v0), _v1(v1), _v2(v2) { 
        _normal = glm::normalize(glm::cross(_v2-_v0, _v1-_v0));
    }

    bool intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const;
    void invNormal() { _normal = -_normal; }
        
    glm::dvec3 _v0;
    glm::dvec3 _v1;
    glm::dvec3 _v2;
    glm::dvec3 _normal;
};

// struct Quad: Entity {
//     public:
//         Quad(): Quad({0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {0, 0, 0}, Material()) {}
//         Quad(const glm::dvec3& v0, const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3, const Material& material): Entity(material), _v0(v0), _v1(v1), _v2(v2), _v3(v3) {
//             Triangle* top = new Triangle(_v0, _v1, _v2, material);
//             Triangle* bot = new Triangle(_v2, _v3, _v0, material);
//             _normal = glm::normalize(glm::cross(_v1-_v0, _v2-_v0));
//         }

//         bool intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const;

//     private:
//         Triangle

//         // Triangle top, bot;
    
// };