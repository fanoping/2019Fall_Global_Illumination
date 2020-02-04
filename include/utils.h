
#include <random>

double random_01() {
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0); 
    
    return distribution(generator);
}

double random_minmax(double min, double max) {
    return min + (max - min) * random_01();
}