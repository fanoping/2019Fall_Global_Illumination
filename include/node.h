#include "ray.h"
#include "bbox.h"
#include "entities.h"


struct Node {
    explicit Node(const BoundingBox& bbox, const int level) : _bbox(bbox), _max_elements(10), _level(level) {}

    /// Subdivides the current node into 8 children.
    bool partition(const int level);
    bool ray_intersect(const Ray& ray);
    void node_intersect(const Ray& ray, std::vector<Entity*>& possible_entities) const;
    void insert(std::vector<Entity*>& triangles) const;
     
    void setchildren(const size_t idx, const BoundingBox& box, const int level) {_children[idx] = std::make_unique<Node>(box, level);}
    void print() {
        for (int i = 0; i < 8; ++i){
            _children[i]->_bbox.print();
        }
    }
    bool is_leaf() const { return _children[0] == nullptr; }

    BoundingBox _bbox;
    std::vector<Entity*> _entities;
    std::array<std::unique_ptr<Node>, 8> _children;
    int  _level;
    int  _max_elements;
};