#pragma once

#include <glm/glm.hpp>

/// Represents the camera with information about the 'sensor' size.
// struct Camera {
//     explicit Camera(glm::dvec3 pos) : Camera(pos, {0, 0, 0}) {}
//     Camera(glm::dvec3 pos, glm::dvec3 lookAt) : pos(pos), up({0, 0, 1.0}), forward(lookAt - pos) {
//         forward = glm::normalize(forward);
//     }

//     glm::dvec3 pos;
//     glm::dvec3 up;
//     glm::dvec3 forward;              // normalized vector of the view direction
//     const double sensorDiag = 0.035; // diagonal of the sensor
//     const double focalDist = 0.04;   // focal distance
// };

struct Camera {
    explicit Camera(glm::dvec3 pos) : Camera(pos, {0, 0, 0}) {}
    Camera(glm::dvec3 pos, glm::dvec3 lookAt) : pos(pos){
        forward = glm::normalize(lookAt - pos);
        w = glm::normalize(pos - lookAt);
        glm::dvec3 tmp_up = {0, 1.0, 0};
        
        if (glm::dot(w, tmp_up) == 1.0) tmp_up = {1.0, 0, 0};
        
        v = glm::normalize(glm::cross(tmp_up, w));
        u = glm::cross(w, v);
    }

    glm::dvec3 w;                       // eye to COI (central of interest)
    glm::dvec3 v;                       // horizontal image direction
    glm::dvec3 u;                       // up vector
    glm::dvec3 pos;
    glm::dvec3 forward;                 // normalized vector of the view direction
    const double sensorDiag = 0.035355; // diagonal of the sensor
    // const double focalDist = 0.04;   // focal distance (distance to the plane)
    const double focalDist = 0.035;
};