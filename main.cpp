#include <QApplication>

#include <iostream>

#include "camera.h"
#include "gui.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Camera camera({0, 0, (10-1e-4)});
    glm::dvec3 light{0, 10-1e-4, 0};

    RayTracer raytracer(camera, light);

    // Set up scene
    Octree scene({-20, -10, -20}, {20, 10, 20});
    // TODO Add objects to the scene
    // scene.push_back(...);

    Sphere* sphere_1 = new Sphere(3.0, {-3, -7, -5}, Material({0.0215,  0.1745,  0.0215}));
    
    // front wall
    Triangle* tri_1 = new Triangle({-20, -10, -20}, {-20, 10, -20}, {20, -10, -20}, Material({0.75, 0.75, 0.75}));
    Triangle* tri_2 = new Triangle({20, 10, -20}, {20, -10, -20}, {-20, 10, -20}, Material({0.75, 0.75, 0.75}));

    // left wall
    Triangle* tri_3 = new Triangle({-20, -10, 20}, {-20, 10, 20}, {-20, -10, -20}, Material({0.75, 0., 0.}));
    Triangle* tri_4 = new Triangle({-20, 10, -20}, {-20, -10, -20}, {-20, 10, 20}, Material({0.75, 0., 0.}));
    
    // right wall
    Triangle* tri_5 = new Triangle({20, -10, -20}, {20, 10, -20}, {20, -10, 20}, Material({0., 0.75, 0.}));
    Triangle* tri_6 = new Triangle({20, 10, 20}, {20, -10, 20}, {20, 10, -20}, Material({0., 0.75, 0.}));

    // ceiling
    Triangle* tri_7 = new Triangle({-20, 10, -20}, {-20, 10, 20}, {20, 10, -20}, Material({0.75, 0.75, 0.75}));
    Triangle* tri_8 = new Triangle({20, 10, 20}, {20, 10, -20}, {-20, 10, 20}, Material({0.75, 0.75, 0.75}));

    // floor
    Triangle* tri_9 = new Triangle({-20, -10, 20}, {-20, -10, -20}, {20, -10, 20}, Material({0.75, 0.75, 0.75}));
    Triangle* tri_10 = new Triangle({20, -10, -20}, {20, -10, 20}, {-20, -10, -20}, Material({0.75, 0.75, 0.75}));

    // back wall
    Triangle* tri_11 = new Triangle({20, -20, 20}, {20, 20, 20}, {-20, -20, 20}, Material({0.25, 0.20725, 0.20725}));
    Triangle* tri_12 = new Triangle({-20, 20, 20}, {-20, -20, 20}, {20, 20, 20}, Material({0.25, 0.20725, 0.20725}));

    scene.push_back(sphere_1);
    scene.push_back(tri_1);
    scene.push_back(tri_2);
    scene.push_back(tri_3);
    scene.push_back(tri_4);
    scene.push_back(tri_5);
    scene.push_back(tri_6);
    scene.push_back(tri_7);
    scene.push_back(tri_8);
    scene.push_back(tri_9);
    scene.push_back(tri_10);
    scene.push_back(tri_11);
    scene.push_back(tri_12);

    raytracer.setScene(&scene);

    Gui window(500, 500, raytracer);
    window.show();
    return app.exec();
}
