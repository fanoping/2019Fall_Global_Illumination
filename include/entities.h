#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "bbox.h"
#include "material.h"
#include "ray.h"

/// A base class for all entities in the scene.
struct Entity {
    Entity() {};
    Entity(Material* material) : material(material) {}

    /// Check if a ray intersects the object. The arguments intersect and normal will contain the
    /// point of intersection and its normals.
    virtual bool intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const = 0;

    /// Returns an axis-aligned bounding box of the entity.
    // virtual BoundingBox boundingBox() const = 0;

    // glm::dvec3 pos = {0, 0, 0};
    Material* material;
    std::vector<Entity*> _objectlist;
};



struct Sphere: Entity {
    Sphere(const double& r, const glm::dvec3& pos, Material* m):
        Entity(m), radius(r), pos(pos) { _objectlist.push_back(this); }

    glm::dvec3 pos;
    double radius;

    // BoundingBox boundingBox() const;

    bool intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const;
    
    // BoundingBox boundingBox() const;
};

struct Triangle: Entity {
    Triangle(const glm::dvec3& v0, const glm::dvec3& v1, const glm::dvec3& v2, Material* material): Entity(material), _v0(v0), _v1(v1), _v2(v2) { 
        _normal = glm::normalize(glm::cross(_v2-_v0, _v1-_v0));
    }

    bool intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const;
    void invNormal() { _normal = -_normal; }

    // BoundingBox boundingBox() const;
        
    glm::dvec3 _v0;
    glm::dvec3 _v1;
    glm::dvec3 _v2;
    glm::dvec3 _normal;
};

struct Quad: Entity {
    Quad(const glm::dvec3& v0, const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3, Material* material): Entity(material), _v0(v0), _v1(v1), _v2(v2), _v3(v3) {
        Triangle* bot = new Triangle(v0, v1, v3, material);
        Triangle* top = new Triangle(v2, v3, v1, material);
        _normal = glm::normalize(glm::cross(_v3-_v0, _v1-_v0));

        _objectlist.push_back(top);
        _objectlist.push_back(bot);
    }
    
    bool intersect(const Ray& ray, glm::dvec3& intersect, glm::dvec3& normal) const;
    // BoundingBox boundingBox() const;

    glm::dvec3 getnorm() const { return _normal; }

    glm::dvec3 uv2pos(double u, double v) {
        return _v0 + u * (_v3 - _v0) + v * (_v1 - _v0);
    }
    glm::dvec3 _v0;
    glm::dvec3 _v1;
    glm::dvec3 _v2;
    glm::dvec3 _v3;
    glm::dvec3 _normal;

};