#include <QApplication>

#include <iostream>

#include "camera.h"
#include "gui.h"
// #include "raytracer.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Camera camera({278, 273, -800}, {278, 273, 0});

    Material* DIFFUSE_LIGHT = new Material({0.05, 0.05, 0.05}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},
                                          0.22, 0, 0, 1.0, 2.0);
    
    // std::vector<Quad*> area_light;

    // Quad* area_light_back = new Quad({213.0, 528.8, 332.0}, {213.0, 548.8, 332.0}, {343.0, 548.8, 332.0}, {343.0, 528.8, 332.0}, DIFFUSE_LIGHT);
    // Quad* area_light_left = new Quad({343.0, 528.8, 332.0}, {343.0, 548.8, 332.0}, {343.0, 548.8, 227.0}, {343.0, 528.8, 227.0}, DIFFUSE_LIGHT);
    // Quad* area_light_front = new Quad({343.0, 528.8, 227.0}, {343.0, 548.8, 227.0}, {213.0, 548.8, 227.0}, {213.0, 528.8, 227.0}, DIFFUSE_LIGHT);
    // Quad* area_light_right = new Quad({213.0, 528.8, 227.0}, {213.0, 548.8, 227.0}, {213.0, 548.8, 332.0}, {213.0, 528.8, 332.0}, DIFFUSE_LIGHT);
    Quad* area_light_bottom = new Quad({343.0, 548.8, 332.0}, {343.0, 548.8, 227.0}, {213.0, 548.8, 227.0}, {213.0, 548.8, 332.0}, DIFFUSE_LIGHT);
    
    // area_light.push_back(area_light_back);
    // area_light.push_back(area_light_left);
    // area_light.push_back(area_light_right);
    // area_light.push_back(area_light_back);
    // area_light.push_back(area_light_front);

    
    RayTracer raytracer(camera, area_light_bottom);

    // Set up scene
    Octree scene({-600, -600, -600}, {600, 600, 600});
    // TODO Add objects to the scene
    // scene.push_back(...);

    Material* DIFFUSE_WHITE = new Material({0.05, 0.05, 0.05}, {0.9, 0.7, 0.4}, {0.9, 0.7, 0.4},
                                          0.0, 0.0, 0, 1.0, 0.0);
    Material* DIFFUSE_RED = new Material({0.05, 0, 0}, {0.76, 0.0, 0.0}, {1.0, 0.0, 0.0},
                                          0.0, 0.0, 0, 1.0, 0.0);
    Material* DIFFUSE_GREEN = new Material({0, 0.05, 0}, {0.11, 0.38, 0.04}, {0.0, 1.0, 0.0},
                                            0.0, 0.0, 0, 1.0, 0.0);
    Material* REFLECTED = new Material({0.2125, 0.1275, 0.054}, {0.714, 0.4284, 0.18144}, {0.393548, 0.271906, 0.166721},
                                        0.99, 0.0, 0, 1.0, 0.0);
    Material* REFLECTED_CEIL = new Material({0.05, 0.05, 0.05}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2},
                                            1.0, 0.0, 0, 1.0, 0.0);
                                    

    Sphere* sphere_1 = new Sphere(83.0, {425.0, 83.0, 100.0}, REFLECTED);
    
    // front wall
    Quad* front_wall = new Quad({549.6, 0.0, 559.2}, {556.0, 548.8, 559.2}, {0, 548.8, 559.2}, {0, 0, 559.2}, DIFFUSE_WHITE);

    // left wall
    Quad* left_wall = new Quad({552.8, 0, 0}, {556.0, 548.8, 0}, {556.0, 548.8, 559.2}, {549.6, 0.0, 559.2}, DIFFUSE_RED);
    
    // right wall
    Quad* right_wall = new Quad({0, 0, 559.2}, {0, 548.8, 559.2}, {0, 548.8, 0}, {0, 0, 0}, DIFFUSE_GREEN);

    // ceiling
    Quad* ceiling = new Quad({556.0, 548.8, 559.2}, {556.0, 548.8, 0}, {0, 548.8, 0}, {0, 548.8, 559.2}, REFLECTED_CEIL);

    // floor
    Quad* floor = new Quad({552.8, 0, 0}, {549.6, 0, 559.2}, {0, 0, 559.2}, {0, 0, 0}, DIFFUSE_WHITE);

    
    // tall block
    Quad* tall_block_left = new Quad({472.0, 0.0, 406.0}, {472.0, 330.0, 406.0}, {423.0, 330.0, 247.0}, {423.0, 0.0, 247.0}, DIFFUSE_WHITE);
    Quad* tall_block_back = new Quad({314.0, 0.0, 456.0}, {314.0, 330.0, 456.0}, {472.0, 330.0, 406.0}, {472.0, 0.0, 406.0}, DIFFUSE_WHITE);
    Quad* tall_block_right = new Quad({265.0, 0.0, 296.0}, {265.0, 330.0, 296.0}, {314.0, 330.0, 456.0}, {314.0, 0, 456.0}, DIFFUSE_WHITE);
    Quad* tall_block_front = new Quad({423.0, 0.0, 247.0}, {423.0, 330.0, 247.0}, {265.0, 330.0, 296.0}, {265.0, 0.0, 296.0}, DIFFUSE_WHITE);
    Quad* tall_block_top = new Quad({423.0, 330.0, 247.0}, {472.0, 330.0, 406.0}, {314.0, 330.0, 456.0}, {265.0, 330.0, 296.0}, DIFFUSE_WHITE);


    Quad* short_block_left = new Quad({240.0, 0.0, 272.0}, {240.0, 165.0, 272.0}, {290.0, 165.0, 114.0}, {290.0, 0.0, 114.0}, DIFFUSE_WHITE);
    Quad* short_block_back = new Quad({82.0, 0.0, 225.0}, {82.0, 165.0, 225.0}, {240.0, 165.0, 272.0}, {240.0, 0.0, 272.0}, DIFFUSE_WHITE);
    Quad* short_block_right = new Quad({130.0, 0.0, 65.0}, {130.0, 165.0, 65.0}, {82.0, 165.0, 225.0}, {82.0, 0, 225.0}, DIFFUSE_WHITE);
    Quad* short_block_front = new Quad({290.0, 0.0, 114.0}, {290.0, 165.0, 114.0}, {130.0, 165.0, 65.0}, {130.0, 0.0, 65.0}, DIFFUSE_WHITE);
    Quad* short_block_top = new Quad({290.0, 165.0, 114.0}, {240.0, 165.0, 272.0}, {82.0, 165.0, 225.0}, {130.0, 165.0, 65.0}, DIFFUSE_WHITE);
    
    
    // scene.push_back(sphere_1);
    scene.push_back(front_wall);
    scene.push_back(left_wall);
    scene.push_back(right_wall);
    scene.push_back(ceiling);
    scene.push_back(floor);
    
    // scene.push_back(area_light_left);
    // scene.push_back(area_light_right);
    scene.push_back(area_light_bottom);
    // scene.push_back(area_light_back);
    // scene.push_back(area_light_front);
    
    scene.push_back(tall_block_left);
    scene.push_back(tall_block_back);
    scene.push_back(tall_block_right);
    scene.push_back(tall_block_front);
    scene.push_back(tall_block_top);

    scene.push_back(short_block_left);
    scene.push_back(short_block_back);
    scene.push_back(short_block_right);
    scene.push_back(short_block_front);
    scene.push_back(short_block_top);

    scene.push_back(sphere_1);



    raytracer.setScene(&scene);

    Gui window(500, 500, raytracer);
    // Gui window(300, 300, raytracer);
    window.show();
    return app.exec();
}
