#pragma once

#include "Color.h"
#include "Camera.h"
#include "Shape.h"
#include "variables.h"
#include "Light.h"

class Scene{
public:
    int w,h;
    int num_objects = 0;
    int num_lights = 0;
    // vertices
    int max_vertices = 0;
    vec3 vertices[1000000];
    int num_vertexes = 0;

    int maxdepth = 5;   // The maximum depth (number of bounces) for a ray
    float attenuation[3] = {1,0,0};   // const linear quadratic
    
    Shape** shapes;
    Light** lights;
    Camera* camera;
     
    Scene(int _w, int _h)
    {
        w = _w;
        h = _h;
        shapes = new Shape*[max_objects];
        lights = new Light*[max_lights];
    };
    
    void set_attenuation(float _a[3]){
        attenuation[0] = _a[0];
        attenuation[1] = _a[1];
        attenuation[2] = _a[2];
    }
    
    bool intersect(Ray &ray, float *thit, Intersection *in);
    bool intersectP(Ray &ray, Shape *shape);
    
    Color findColor(Intersection *in);
    
    void rayTrace(Ray &ray, int depth, Color *color);
    
    static Ray createReflectedRay(LocalGeo local, Ray ray);
    
    Color computePointLight(vec3 direction,
                            vec3 lightColor,
                            vec3 normal,
                            vec3 halfvec,
                            Shape* shape);
    
};
