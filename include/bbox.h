// #pragma once

// #include <array>
// #include <cassert>

// #include <glm/glm.hpp>

// /// Represents an axis-aligned bounding box.
// struct BoundingBox {
//     BoundingBox(glm::dvec3 min, glm::dvec3 max) : min(min), max(max) {
//         assert(min.x < max.x);
//         assert(min.y < max.y);
//         assert(min.z < max.z);
//     }

//     double dx() const { return max.x - min.x; }
//     double dy() const { return max.y - min.y; }
//     double dz() const { return max.z - min.z; }

//     const glm::dvec3 min;
//     const glm::dvec3 max;

//     /// Check if another bounding box intersects the current bounding box.
//     bool intersect(const BoundingBox& other) const {
//         // TODO Implement this
//         return false;
//     }

//     /// Check if a point lies within the bounding box.
//     bool contains(glm::dvec3 point) const {
//         // TODO Implement this
//         return false;
//     }
// };

#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <glm/glm.hpp>

/// Represents an axis-aligned bounding box.
struct BoundingBox {
    BoundingBox(glm::dvec3 min, glm::dvec3 max) : min(min), max(max) {
        assert(min.x < max.x);
        assert(min.y < max.y);
        assert(min.z < max.z);
    }

    double dx() const { return max.x - min.x; }
    double dy() const { return max.y - min.y; }
    double dz() const { return max.z - min.z; }

    const glm::dvec3 min;
    const glm::dvec3 max;

    /// Check if another bounding box intersects the current bounding box.
    bool intersect(const BoundingBox& other) const {
        // TODO Implement this
        return min[0] < other.max[0] &&
	           min[1] < other.max[1] &&
	           min[2] < other.max[2] &&
	           other.min[0] < max[0] &&
	           other.min[1] < max[1] &&
	           other.min[2] < max[2];
    }

    /// Check if a point lies within the bounding box.
    bool contains(glm::dvec3 point) const {
        // TODO Implement this
        return min[0] <= point[0] && point[0] <= max[0] &&
	           min[1] <= point[1] && point[1] <= max[1] &&
	           min[2] <= point[2] && point[2] <= max[2];
    }

    void print(){
        std::ostringstream ss1, ss2;
        ss1 << std::setprecision(4) << "(" << min[0] << ", " << min[1] << ", " << min[2] << ")";
        ss2 << std::setprecision(4) << "(" << max[0] << ", " << max[1] << ", " << max[2] << ")";

        std::cout << "=================================================" << std::endl;
        std::cout << "= bbox min: (" <<  std::left << std::setw(35) << ss1.str() << "=" << std::endl;  
        std::cout << "= bbox max: (" <<  std::left << std::setw(35) << ss2.str() << "=" << std::endl;
        std::cout << "=================================================" << std::endl;  
    }
};
