#pragma once

#include <glm/glm.hpp>

struct Ray {
    Ray(): origin({0, 0, 0}), dir({0, 0, 0}) {}
    Ray(glm::dvec3 origin, glm::dvec3 dir) : origin(std::move(origin)), dir(glm::normalize(dir)) {}

    glm::dvec3 origin;
    glm::dvec3 dir; // normalized directional vector
};
