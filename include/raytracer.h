#pragma once

#include <algorithm>
#include <memory>
//#include <future>

#include <glm/glm.hpp>

#include "camera.h"
#include "entities.h"
#include "image.h"
#include "octree.h"
#include "material.h"
#include <iostream>
#include "utils.h"

#define N_SAMPLES 1000
#define OFFSET 1e-4
#define MAX_DEPTH 20 // 0: no bounce
#define N_SAMPLES_DIRECT 1 // 0: no bounce
#define N_SAMPLES_INDIRECT 1 // 0: no bounce

// std::default_random_engine generator; 
// std::uniform_real_distribution<double> distribution_hf(-0.5, 0.5); 
// std::uniform_real_distribution<double> distribution_01(0.0, 1.0); 

class RayTracer {
  public:
    RayTracer() = delete;
    RayTracer(const Camera& camera, Quad* light)
        : _camera(camera), _light(light), _image(std::make_shared<Image>(0,0)){};

    void setScene(const Octree* scene) { _scene = scene; }

    void run(int w, int h) {
        // TODO Implement this
        _image = std::make_shared<Image>(w, h);
        double zoom = _camera.sensorDiag / sqrt(w * w + h * h);
        // The structure of the for loop should remain for incremental rendering.

        
        #pragma omp parallel for num_threads(32)
        for (int y = (-h/2); y < (h/2); y++) {
            for (int x = (-w/2); x < (w/2); x++) {
                if (_running) {
                // TODO Implement this
                    glm::dvec3 final_color = {0, 0, 0};

                    for (int n = 0; n < N_SAMPLES; ++n) {

                        double jitter_x = random_minmax(-0.5, 0.5);
                        double jitter_y = random_minmax(-0.5, 0.5);

                        glm::dvec3 point_on_image = _camera.pos + _camera.focalDist * _camera.forward + ((double)y + jitter_y) * zoom * _camera.u + ((double)x + jitter_x) * zoom * _camera.v;
                        Ray ray = Ray(_camera.pos, point_on_image - _camera.pos);
                        std::vector<Entity*> candidates = _scene->intersect(ray);
                        
                        // final_color += getColor(ray, 0);
                        final_color += getColor(ray);
                    }
                    final_color /= (double)N_SAMPLES;

                    // if (y > (int)h/4) {
                    //    std::cout << final_color[0] << " " << final_color[1] << " " << final_color[2] << std::endl; 
                    // }
                    
                    final_color[0] = glm::clamp(std::pow(final_color[0], 1 / 2.2), 0.0, 1.0);
                    final_color[1] = glm::clamp(std::pow(final_color[1], 1 / 2.2), 0.0, 1.0);
                    final_color[2] = glm::clamp(std::pow(final_color[2], 1 / 2.2), 0.0, 1.0);
                    
                    // final_color[0] = std::min(1.0, final_color[0]);
                    // final_color[1] = std::min(1.0, final_color[1]);
                    // final_color[2] = std::min(1.0, final_color[2]);

                    // final_color[0] = std::max(0.0, final_color[0]);
                    // final_color[1] = std::max(0.0, final_color[1]);
                    // final_color[2] = std::max(0.0, final_color[2]);

                    #pragma omp critical
                    _image->setPixel(x + (w/2), (h/2) - y - 1, final_color);
                }
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
    Quad* _light;
    std::shared_ptr<Image> _image;

    

    // bool hitLight(Ray& checkray) {
    //     glm::dvec3 check_intersection = glm::dvec3(MAXFLOAT, MAXFLOAT, MAXFLOAT);
    //     glm::dvec3 check_normal = {0, 0, 0};

    //     if (!_light->intersect(checkray, check_intersection, check_normal)) {
    //         return false;
    //     }

        
    //     std::vector<Entity*> check_possible_intersect = _scene->intersect(checkray);
    //     if (!(check_possible_intersect.empty())) {
    //         glm::dvec3 ob_intersection = glm::dvec3(MAXFLOAT, MAXFLOAT, MAXFLOAT);
    //         glm::dvec3 ob_normal = {0, 0, 0};
            
    //         for (std::vector<Entity*>::iterator iter = check_possible_intersect.begin(); iter != check_possible_intersect.end(); ++iter) {
    //             if ((*iter)->intersect(checkray, ob_intersection, ob_normal)){
    //                 if (glm::distance(check_intersection, checkray.origin) > glm::distance(ob_intersection, checkray.origin)){
    //                     return false;   
    //                 }       
    //             }
    //         }
    //     }
    //     return true;
    // }

    glm::dvec3 createHemishpereSampleRay(glm::dvec3& normal, const double& r1, const double& r2) {
        // create Hemisphere coordinates
        glm::dvec3 tmp_vec = {0, 1.0, 0};
        if (glm::dot(normal, tmp_vec) == 1.0) tmp_vec = {1.0, 0, 0};
        glm::dvec3 nt = glm::normalize(glm::cross(tmp_vec, normal));
        glm::dvec3 nb = glm::cross(normal, nt);


        // double sinTheta = std::sqrt(1 - r1 * r1); 
        double cosTheta_sqrt = std::sqrt(r1); 
        double phi = 2 * M_PI * r2; 
        // glm::dvec3 sample = {sinTheta * std::cos(phi), r1, sinTheta * std::sin(phi)};
        glm::dvec3 sample = {cosTheta_sqrt * std::cos(phi), sqrt(1 - cosTheta_sqrt), cosTheta_sqrt * std::sin(phi)};
        sample = {sample.x * nt.x + sample.y * normal.x + sample.z * nb.x,
                  sample.x * nt.y + sample.y * normal.y + sample.z * nb.y,
                  sample.x * nt.z + sample.y * normal.z + sample.z * nb.z};

        return sample;
    }


    // glm::dvec3 getColor(Ray& ray, int depth);
    
    // first attempt: buggy for ceiling

    /*glm::dvec3 getColor(Ray& ray, int depth) {
        // std::cout << depth<<std::endl;
        glm::dvec3 intersection = glm::dvec3(MAXFLOAT, MAXFLOAT, MAXFLOAT);
        glm::dvec3 normal = {0, 0, 0};
        bool hit = false;

        std::vector<Entity*> possible_intersect = _scene->intersect(ray);
        
        Material* m;
        for (int i = 0; i < possible_intersect.size(); ++i) {
            if (possible_intersect[i]->intersect(ray, intersection, normal)){
                hit = true;
                m = possible_intersect[i]->material;      
            }
        }

        glm::dvec3 color = {0, 0, 0};        
        
        if (!hit) return color;
        if (depth > MAX_DEPTH) return color;

        color += m->emmisive;
        

        // creat the angle theta, phi follow uniform distribution
        double r1 = random_01(); 
        double r2 = random_01();
            
        Ray sample_ray = Ray(intersection + normal * OFFSET, createHemishpereSampleRay(normal, r1, r2));
        color += getColor(sample_ray, depth + 1) * m->reflectance * (1 - m->reflectance_specular) * m->diffuse_color;
        
    
        color[0] = std::min(1.0, color[0]);
        color[1] = std::min(1.0, color[1]);
        color[2] = std::min(1.0, color[2]);

        color[0] = std::max(0.0, color[0]);
        color[1] = std::max(0.0, color[1]);
        color[2] = std::max(0.0, color[2]);
        
        
        return color;
    } 
    */

    /* 2nd attempt
    glm::dvec3 getColor(Ray& ray, int depth) {
        glm::dvec3 intersection = {MAXFLOAT, MAXFLOAT, MAXFLOAT};
        glm::dvec3 normal = {0, 0, 0};
        bool hit = false;
        
        // check if hits an object
        if (hitLight(ray)){
            return _light->material->emmisive * _light->material->specular_color;
        }


        std::vector<Entity*> possible_intersect = _scene->intersect(ray);
        
        Material* m;
        for (int i = 0; i < possible_intersect.size(); ++i) {
            if (possible_intersect[i]->intersect(ray, intersection, normal)){
                hit = true;
                m = possible_intersect[i]->material;      
            }
        }

        if (!hit) return {0, 0, 0};
        if (depth > MAX_DEPTH) return {0, 0, 0};

        glm::dvec3 total = {0, 0, 0};
        glm::dvec3 diffuse_part = {0, 0, 0};
        glm::dvec3 specular_part = {0, 0, 0};

        
        if (1 - m->transmissivity) {  
            // speculer direct 
            glm::dvec3 recursive_ray_dir = ray.dir - normal * glm::dot(ray.dir, normal) * 2.0;
            Ray recursive_ray = Ray(intersection + OFFSET * normal, recursive_ray_dir); 
            
            specular_part += getColor(recursive_ray, depth + 1) * m->specular_color * m->reflectance * m->reflectance_specular;
        
            // diffuse with lambertian brdf
            if (1 - m->reflectance_specular) {
                int n_samples_light = 1;
                for (int n_sample = 0; n_sample < n_samples_light; ++n_sample){
                    double u = random_01();
                    double v = random_01();

                    Ray shadow_ray = Ray(intersection + OFFSET * normal, _light->uv2pos(u, v) - (intersection + OFFSET * normal));
                    if (hitLight(shadow_ray)) {
                        diffuse_part += m->diffuse_color * m->reflectance * (1 - m->reflectance_specular) / M_PI *  // albedo / M_PI
                                        _light->material->emmisive * _light->material->specular_color *
                                        std::max(0.0, glm::dot(shadow_ray.dir, normal)) *   // cosine angle
                                        std::max(0.0, glm::dot(_light->_normal, -shadow_ray.dir)) /  // cosine angle of light
                                        (double)(n_samples_light); 
                    }
                                        
                }
                
                // indirect light source
                int n_samples_indirect_light = 1;
                glm::dvec3 indirect_diffuse_part = {0, 0, 0};

                for (int n_sample = 0; n_sample < n_samples_light; ++n_sample){
                    double r1 = random_01(); 
                    double r2 = random_01();
                    Ray sample_ray = Ray(intersection + normal * OFFSET, createHemishpereSampleRay(normal, r1, r2));   
                    indirect_diffuse_part += getColor(sample_ray, depth + 1) * m->diffuse_color / (double)n_samples_indirect_light; 
                }
            }
            // std::cout << diffuse_part[0] << " " << diffuse_part[1] << " " << diffuse_part[2] << std::endl;  
            total += (specular_part + diffuse_part) / (1 - m->transmissivity);
        }

        return total;
    } 
    */

    glm::dvec3 getColor(Ray& ray) {
        glm::dvec3 intersection = {MAXFLOAT, MAXFLOAT, MAXFLOAT};
        glm::dvec3 normal = {0, 0, 0};
        Material m;
        
        if(find_nearest(ray, intersection, normal, m)) {
            return m.emmisive + getRecursiveColor(ray, intersection, normal, m, 0);
        }
        return {0, 0, 0};
    }

    glm::dvec3 getRecursiveColor(Ray& ray, glm::dvec3& intersection, glm::dvec3& normal, Material& m, int depth) {
        // main recursive function from the starting point
        intersection += normal * OFFSET;
        return getDirectColor(ray, intersection, normal, m) + getIndirectColor(ray, intersection, normal, m, depth);
    }

    glm::dvec3 getDirectColor(Ray& ray, glm::dvec3& intersection, glm::dvec3& normal, Material& m) {
        glm::dvec3 total = {0, 0, 0};
        Quad* sample_light = _light;
        double u, v; //, gen;
        // int index = 0;
        
        for (int i = 0; i < N_SAMPLES_DIRECT; ++i) {
            u = random_01();
            v = random_01();

            // gen = random_01();
            // if (gen <= 11050./15350) { index = 0; }
            // else if (11050./15350 < gen <= 11900./15350) {index = 1;}
            // else if (11900./15350 < gen <= 12750./15350) {index = 2;}
            // else if (12750./15350 < gen <= 14050.5/15350) {index = 3;}
            // else if (14050.5/15350 < gen <= 15350./15350) {index = 4;}

            // if (gen <= 1./5) { index = 0; }
            // else if (1./5 < gen <= 2./5) {index = 1;}
            // else if (2./5 < gen <= 3./5) {index = 2;}
            // else if (3./5 < gen <= 4./5) {index = 3;}
            // else if (4./5 < gen <= 1.0) {index = 4;}

            // if (gen <= 0.6) { index = 0; }
            // else if (0.6 < gen <= 0.7) {index = 1;}
            // else if (0.7 < gen <= 0.8) {index = 2;}
            // else if (0.8 < gen <= 0.9) {index = 3;}
            // else if (0.9 < gen <= 1.0) {index = 4;}


            // sample_light = _light[index];
            // glm::dvec3 light_sample_pos = sample_light->uv2pos(u, v);
            glm::dvec3 light_sample_pos = _light->uv2pos(u, v);
            glm::dvec3 light_sample_norm = _light->getnorm(); 
            if (visability_term(intersection, light_sample_pos)) {
                if (1 - m.reflectance) {
                    // diffuse
                    // std::cout<< "AAA" << std::endl;
                    total += geometry_term(intersection, normal, light_sample_pos, light_sample_norm) *
                                           sample_light->material->diffuse_color * sample_light->material->emmisive *
                                           m.diffuse_color * (1 - m.reflectance); // /
                                        //    glm::distance(intersection, light_sample_pos);
                
                    // specular
                    if (m.reflectance_specular) {
                        total += geometry_term(intersection, normal, light_sample_pos, light_sample_norm) *
                                 glm::pow(glm::dot(glm::normalize(-ray.dir + glm::normalize(light_sample_pos - intersection)), normal), 8) *
                                 sample_light->material->diffuse_color * sample_light->material->emmisive *
                                 m.specular_color * (1 - m.reflectance);// / 
                                //  glm::distance(intersection, light_sample_pos);;
                    }
                }
                // std::cout << a.x << " "<<  a.y<< " " <<a.z << std::endl;
                // std::cout <<  glm::dot(-ray.dir + glm::normalize(light_sample_pos - intersection), normal) <<std::endl;
            }   
        }
        // std::cout << total.x << " "<<  total.y<< " " <<total.z << std::endl; 
        return total / ((double)N_SAMPLES_DIRECT * M_PI);
    }

    glm::dvec3 getIndirectColor(Ray& ray, glm::dvec3& intersection, glm::dvec3& normal, Material& m, int depth) { 
        double russian_roulette = random_01();
        double non_terminate_prob = (MAX_DEPTH) ? 0.8 : 1.0;

        if (russian_roulette > non_terminate_prob || depth > MAX_DEPTH) return {0, 0, 0};

        glm::dvec3 total = {0, 0, 0};
        
        Ray reflection_ray = Ray(intersection, ray.dir - normal * 2.0 * glm::dot(ray.dir, normal));
        glm::dvec3 reflection_intersection = {MAXFLOAT, MAXFLOAT, MAXFLOAT};
        glm::dvec3 reflection_normal = {0, 0, 0};
        Material reflection_m;
        
        if (m.reflectance) {
            if(find_nearest(reflection_ray, reflection_intersection, reflection_normal, reflection_m)){
                total += getRecursiveColor(reflection_ray, reflection_intersection, reflection_normal, reflection_m, depth + 1) *
                         m.diffuse_color * m.reflectance; 
            }
        }

        for (int n = 0; n < N_SAMPLES_INDIRECT; ++n) {
            double r1 = random_01();
            double r2 = random_01();

            Ray sample_ray = Ray(intersection, createHemishpereSampleRay(normal, r1, r2));
            glm::dvec3 sample_intersection = {MAXFLOAT, MAXFLOAT, MAXFLOAT};
            glm::dvec3 sample_normal = {0, 0, 0};
            Material sample_m;
            
            if(find_nearest(sample_ray, sample_intersection, sample_normal, sample_m)){
                total += r1 * getRecursiveColor(sample_ray, sample_intersection, sample_normal, sample_m, depth + 1) *
                         m.diffuse_color / (double)(N_SAMPLES_INDIRECT); 
            }

            // Ray reflection_ray = Ray(intersection, ray.dir - normal * 2.0 * glm::dot(ray.dir, normal));
            // glm::dvec3 reflection_intersection = {MAXFLOAT, MAXFLOAT, MAXFLOAT};
            // glm::dvec3 reflection_normal = {0, 0, 0};
            // Material reflection_m;
            
            
        }

        // std::cout << total.x << " "<<  total.y<< " " <<total.z << std::endl; 
        return total / (non_terminate_prob);
    }

    double geometry_term(const glm::dvec3& self, const glm::dvec3& self_normal, const glm::dvec3& other, const glm::dvec3& other_normal){
        // std::cout << (self-other).x << " "<<  (self-other).y<< " " <<(self-other).z << std::endl; 
        
        double cos_at_self = std::max(0.0, glm::dot(glm::normalize(other - self), glm::normalize(self_normal)));
        double cos_at_other = std::max(0.0, glm::dot(glm::normalize(self - other), glm::normalize(other_normal)));
        
        // std::cout<< cos_at_self * cos_at_other << std::endl;
        return cos_at_self * cos_at_other;
    }

    bool visability_term(const glm::dvec3& self, const glm::dvec3& other) {
        Ray r = Ray(self, other - self);
        glm::dvec3 obstacle_intersection = {MAXFLOAT, MAXFLOAT, MAXFLOAT};
        glm::dvec3 obstacle_normal = {0, 0, 0};
        Material obstacle_m;
        
        if (find_nearest(r, obstacle_intersection, obstacle_normal, obstacle_m)){
            if( glm::distance(obstacle_intersection, self) < glm::distance(other, self) ){
                return false;
            }
        }
        return true;
    }

    bool find_nearest(Ray& ray, glm::dvec3& intersection, glm::dvec3& normal, Material& m) {

        // find nearest intersection -> intersection
        bool hit = false;
        std::vector<Entity*> possible_intersect = _scene->intersect(ray);
        
        for (int i = 0; i < possible_intersect.size(); ++i) {
            if (possible_intersect[i]->intersect(ray, intersection, normal)){
                hit = true;
                m = *(possible_intersect[i]->material); 
            }
        }

        return hit;
    }
    
};


