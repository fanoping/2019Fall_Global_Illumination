#pragma once

#include <glm/glm.hpp>

/// Represents the material properties of an entity. For now it only contains color, but it should
/// probably be extended to allow more options.
struct Material {
    Material(): Material({0, 0, 0}, {0, 0, 0}, {0, 0, 0},
                         0.0, 0.0, 0.0, 1.0, 0.0) {}
    Material(const glm::dvec3& ambient, 
             const glm::dvec3& diffuse,
             const glm::dvec3& specular,
             const double& reflectance,
             const double& reflectance_specular,
             const double& transmissivity,
             const double& refraction_index,
             const double& emmisive) :
             ambient_color(ambient), diffuse_color(diffuse), specular_color(specular),
             reflectance(reflectance), reflectance_specular(reflectance_specular),
             transmissivity(transmissivity), refraction_index(refraction_index), emmisive(emmisive) {}
    
    glm::dvec3 ambient_color;
    glm::dvec3 diffuse_color;
    glm::dvec3 specular_color;
    
    // reflectance
    double reflectance;             // reflectance for diffuse + ambient [0, 1]
    double reflectance_specular;         // reflectance for degree of diffuse [0, 1 (perfect diffuse)]

    // transmissivity
    double transmissivity;          // transmissivity materials (contributed to refraction rays)
    double refraction_index;        // material refraction index (air = 1)

    double emmisive;            // emmisive for lights, usually > 8

};