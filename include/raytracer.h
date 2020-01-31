#pragma once

#include <algorithm>
#include <memory>
//#include <future>

#include <glm/glm.hpp>

#include "camera.h"
#include "entities.h"
#include "image.h"
#include "octree.h"

#define OFFSET 1e-5

class RayTracer {
  public:
    RayTracer() = delete;
    RayTracer(const Camera& camera, glm::dvec3 light)
        : _camera(camera), _light(light), _image(std::make_shared<Image>(0,0)){};

    void setScene(const Octree* scene) { _scene = scene; }

    void run(int w, int h) {
        // TODO Implement this
        _image = std::make_shared<Image>(w, h);
        double zoom = _camera.sensorDiag / sqrt(w * w + h * h);
        // The structure of the for loop should remain for incremental rendering.
        
        for (double y = (-h/2); y < (h/2) && _running; y++) {
            for (double x = (-w/2); x < (w/2) && _running; x++) {
                // TODO Implement this
                glm::dvec3 point_on_image = _camera.pos + _camera.focalDist * _camera.forward + y * zoom * _camera.u + x * zoom * _camera.v;
                
                Ray ray = Ray(_camera.pos, point_on_image - _camera.pos);

                std::vector<Entity*> candidates = _scene->intersect(ray);
                
                
                glm::dvec3 final_color = getColor(ray, 0);
                _image->setPixel(x + (w/2), (h/2) - y - 1, final_color);
            }
        }
    }

    bool running() const { return _running; }
    void stop() { _running = false; }
    void start() { _running = true; }

    std::shared_ptr<Image> getImage() const { return _image; }

  private:
    bool _running = false;
    const Octree* _scene;
    Camera _camera;
    glm::dvec3 _light;
    std::shared_ptr<Image> _image;

    bool occluded(Ray& checkray, glm::dvec3 intersection){
        std::vector<Entity*> check_possible_intersect = _scene->intersect(checkray);
        if (!(check_possible_intersect.empty())) {
            glm::dvec3 check_intersection = glm::dvec3(MAXFLOAT, MAXFLOAT, MAXFLOAT);
            glm::dvec3 check_normal = _camera.w;
            bool check_hit = false;
        
            #pragma omp critical
            for (std::vector<Entity*>::iterator iter = check_possible_intersect.begin(); iter != check_possible_intersect.end(); ++iter) {
                #pragma omp critical
                if ((*iter)->intersect(checkray, check_intersection, check_normal)){
                    check_hit = true;     
                }
            }
            if (check_hit) {
                if (glm::distance(check_intersection, intersection) < glm::distance(_light, intersection)){
                    return true;
                }
            }
        }
        return false;
    }

    glm::dvec3 getColor(Ray& ray, int depth) {
        glm::dvec3 intersection = glm::dvec3(MAXFLOAT, MAXFLOAT, MAXFLOAT);
        glm::dvec3 normal = {0, 0, 0};
        bool hit = false;

        std::vector<Entity*> possible_intersect = _scene->intersect(ray);
        
        Material m;
        for (int i = 0; i < possible_intersect.size(); ++i) {
            #pragma omp critical
            if (possible_intersect[i]->intersect(ray, intersection, normal)){
                hit = true;
                m = possible_intersect[i]->material;      
            }
        }

        glm::dvec3 final_color = {0, 0, 0};
        // move the occluded function outside
        if (hit){
            Ray checkray = Ray((intersection + OFFSET * normal), _light - (intersection + OFFSET * normal));
            bool occ = occluded(checkray, intersection + OFFSET * normal);

            glm::dvec3 light_color = {1.0, 1.0, 1.0};
            bool ambient = true;
            bool diffuse = true;

            if (ambient) {
                final_color += m.color * light_color * 0.33;
            }
            if (diffuse && !occ) {
                double factor = std::max(0.0, glm::dot(glm::normalize(_light - (intersection + OFFSET * normal)), normal));
                final_color += m.color * light_color * factor * 0.67;
            }
        }

        final_color[0] = (final_color[0] > 1) ? 1.0 : final_color[0];
        final_color[1] = (final_color[1] > 1) ? 1.0 : final_color[1];
        final_color[2] = (final_color[2] > 1) ? 1.0 : final_color[2];

        final_color[0] = (final_color[0] < 0) ? 0.0 : final_color[0];
        final_color[1] = (final_color[1] < 0) ? 0.0 : final_color[1];
        final_color[2] = (final_color[2] < 0) ? 0.0 : final_color[2];

        return final_color;
    }  
};
