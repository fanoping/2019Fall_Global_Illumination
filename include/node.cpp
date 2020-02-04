#include "node.h"

#include <iostream>
using namespace std;

bool Node::partition(const int level) {
    // spilt into 8 boxes
    /*
        Here, - means less than 'origin' in that dimension, + means greater than.
        child:	0 1 2 3 4 5 6 7
        x:      - - - - + + + +
        y:      - - + + - - + +
        z:      - + - + - + - +
    */
    // cout<<"nb"<<endl;
    // _bbox.print();
    if ((_bbox.min.x >= _bbox.max.x) || (_bbox.min.y >= _bbox.max.y) || (_bbox.min.z >= _bbox.max.z)) return false;
    // cout<<"nb"<<endl;
    // cout << level<< endl;
    glm::dvec3 mid = (this->_bbox.min + this->_bbox.max) / 2.0;
    setchildren(0, BoundingBox({this->_bbox.min, mid}), level);
    setchildren(1, BoundingBox({{this->_bbox.min[0], this->_bbox.min[1], mid[2]}, {mid[0], mid[1], this->_bbox.max[2]}}), level);
    setchildren(2, BoundingBox({{this->_bbox.min[0], mid[1], this->_bbox.min[2]}, {mid[0], this->_bbox.max[1], mid[2]}}), level);
    setchildren(3, BoundingBox({{this->_bbox.min[0], mid[1], mid[2]}, {mid[0], this->_bbox.max[1], this->_bbox.max[2]}}), level);
    setchildren(4, BoundingBox({{mid[0], this->_bbox.min[1], this->_bbox.min[2]}, {this->_bbox.max[0], mid[1], mid[2]}}), level);
    setchildren(5, BoundingBox({{mid[0], this->_bbox.min[1], mid[2]}, {this->_bbox.max[0], mid[1], this->_bbox.max[2]}}), level);
    setchildren(6, BoundingBox({{mid[0], mid[1], this->_bbox.min[2]}, {this->_bbox.max[0], this->_bbox.max[1], mid[2]}}), level);
    setchildren(7, BoundingBox({{mid[0], mid[1], mid[2]}, {this->_bbox.max[0], this->_bbox.max[1], this->_bbox.max[2]}}), level);
    return true;
    // _children[0] = make_unique<Node>(BoundingBox({_bbox.min, mid}));
    // _children[1] = make_unique<Node>(BoundingBox({{_bbox.min[0], _bbox.min[1], mid[2]}, {mid[0], mid[1], _bbox.max[2]}}));
    // _children[2] = make_unique<Node>(BoundingBox({{_bbox.min[0], mid[1], _bbox.min[2]}, {mid[0], _bbox.max[1], mid[2]}}));
    // _children[3] = make_unique<Node>(BoundingBox({{_bbox.min[0], mid[1], mid[2]}, {mid[0], _bbox.max[1], _bbox.max[2]}}));
    // _children[4] = make_unique<Node>(BoundingBox({{mid[0], _bbox.min[1], _bbox.min[2]}, {_bbox.max[0], mid[1], mid[2]}}));
    // _children[5] = make_unique<Node>(BoundingBox({{mid[0], _bbox.min[1], mid[2]}, {_bbox.max[0], mid[1], _bbox.max[2]}}));
    // _children[6] = make_unique<Node>(BoundingBox({{mid[0], mid[1], _bbox.min[2]}, {_bbox.max[0], _bbox.max[1], mid[2]}}));
    // _children[7] = make_unique<Node>(BoundingBox({{mid[0], mid[1], mid[2]}, {_bbox.max[0], _bbox.max[1], _bbox.max[2]}}));
}

bool Node::ray_intersect(const Ray& ray) {
    double txmin, txmax, tymin, tymax, tzmin, tzmax;

    glm::dvec3 invdir = 1.0 / ray.dir;

    txmin = (ray.dir.x < 0) ? (this->_bbox.max.x - ray.origin.x) * invdir.x : (this->_bbox.min.x - ray.origin.x) * invdir.x;
    txmax = (ray.dir.x < 0) ? (this->_bbox.min.x - ray.origin.x) * invdir.x : (this->_bbox.max.x - ray.origin.x) * invdir.x;

    tymin = (ray.dir.y < 0) ? (this->_bbox.max.y - ray.origin.y) * invdir.y : (this->_bbox.min.y - ray.origin.y) * invdir.y;
    tymax = (ray.dir.y < 0) ? (this->_bbox.min.y - ray.origin.y) * invdir.y : (this->_bbox.max.y - ray.origin.y) * invdir.y;

    if ((txmin > tymax) || tymin > txmax) return false;
    if (tymin > txmin) txmin = tymin;
    if (tymax < txmax) txmax = tymax;

    tzmin = (ray.dir.z < 0) ? (this->_bbox.max.z - ray.origin.z) * invdir.z : (this->_bbox.min.z - ray.origin.z) * invdir.z;
    tzmax = (ray.dir.z < 0) ? (this->_bbox.min.z - ray.origin.z) * invdir.z : (this->_bbox.max.z - ray.origin.z) * invdir.z;

    if ((txmin > tzmax) || tzmin > txmax) return false;
    if (tzmin > txmin) txmin = tzmin;
    if (tzmax < txmax) txmax = tzmax;

    return true;
}

void Node::node_intersect(const Ray& ray, std::vector<Entity*>& possible_entities) const{
    if(const_cast<Node*>(this)->ray_intersect(ray)) {
        if (this->is_leaf()) {
            // cout << this->_entities.size() << endl;
            for (std::vector<Entity* const>::iterator iter = this->_entities.begin(); iter != this->_entities.end(); ++iter) {
                // if (!((*iter)->checkFlag)){
                possible_entities.push_back(*iter);
                // }
            }
            // cout << possible_entities.size() << endl;
            return;
        }
        for (int i = 0; i < 8; ++i) {
            const_cast<Node*>(this)->_children[i]->node_intersect(ray, possible_entities);
        }
    }
}

void Node::insert(std::vector<Entity*>& triangles) const {
    if (triangles.size() <= this->_max_elements || this->_level > 8) {
        const_cast<Node*>(this)->_entities.insert(const_cast<Node*>(this)->_entities.end(), triangles.begin(), triangles.end());
        return;
    }
    
    if (is_leaf()) {
        if (!(const_cast<Node*>(this)->partition(this->_level + 1))) {
            const_cast<Node*>(this)->_entities.insert(const_cast<Node*>(this)->_entities.end(), triangles.begin(), triangles.end());
            return;
        }
        if (!(const_cast<Node*>(this)->_entities.empty())) {
            for (std::vector<Entity* const>::iterator iter = this->_entities.begin(); iter != this->_entities.end(); ++iter) {
                // if (!((*iter)->checkFlag)){
                triangles.push_back(*iter);
                // }
            }
            // const_cast<Node*>(this)->_entities.insert(triangles.end(), const_cast<Node*>(this)->_entities.begin(), const_cast<Node*>(this)->_entities.end());
            for (int i = 0; i < const_cast<Node*>(this)->_entities.size(); i++){
                const_cast<Node*>(this)->_entities.pop_back();
            }
        }
    }

    // cout << "eee"  << endl;
    for(int i = 0; i < 8; ++i){
        std::vector<Entity*> child_triangles;
        for (size_t j = 0; j < triangles.size(); ++j) {  
            // triangles[j]->boundingBox().print();
            // (this->_children[i])->_bbox.print();
            if ((this->_children[i])->_bbox.intersect(triangles[j]->boundingBox())){
                // cout << j << endl;
                // (parent._children[i])->_bbox.print();
                child_triangles.push_back(triangles[j]);
            }
        }
       this->_children[i]->insert(child_triangles);
    }
} 
