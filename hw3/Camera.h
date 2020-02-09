#pragma once

#include "Transform.h"
#include "Ray.h"
#include "Sampler.h"
#include "Film.h"

class Camera{
public:
    vec3 lookFrom = vec3();
    vec3 lookAt = vec3();
    vec3 up = vec3();
    
    // for coord frame
    vec3 u = vec3();
    vec3 v = vec3();
    vec3 w = vec3();
    
    float fovy;
    Camera(vec3 _lookFrom, vec3 _lookAt, vec3 _up, float _fovy);
    
    void setCoord();
    
    void generateRay(Sample &sample, Ray *ray, Film &film);
    
};