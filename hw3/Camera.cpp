#pragma once
#include "Camera.h"
#include "Film.h"
#include "variables.h"

void Camera::generateRay(Sample &sample, Ray *ray, Film &film){
    
    float beta = tanf(glm::radians(fovy) / 2) * (film.h/2 - sample.y) / (film.h/2);
    
    float tan_fovx_2 = (float) film.w / film.h * tanf(glm::radians(fovy) / 2);
    float alpha = tan_fovx_2 * (sample.x - film.w/2) / (film.w/2);
    
    vec3 dirn = vec3(
                     alpha * u.x + beta * v.x - w.x,
                     alpha * u.y + beta * v.y - w.y,
                     alpha * u.z + beta * v.z - w.z);
    
    *ray = Ray(vec4(lookFrom,1), glm::normalize(vec4(dirn,0)), ray->t, ray->t_min, ray->t_max);
    
}

Camera::Camera(vec3 _lookFrom, vec3 _lookAt, vec3 _up, float _fovy){
    lookFrom = _lookFrom;
    lookAt = _lookAt;
    up = _up;
    fovy = _fovy;
    
    setCoord();
};

void Camera::setCoord(){
    // set coord frame
    // w: eye - center
    w = glm::normalize(lookFrom - lookAt);
    u = glm::normalize(glm::cross(up, w));
    v = glm::cross(w, u);
}

