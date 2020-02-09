#pragma once

#include "Ray.h"
#include "Transform.h"

class Point{
public:
    vec4 p = vec4(0,0,0,1);
    Point(vec4 _p)
    {p = _p;};
    Point(){};
};

class Normal{
public:
    vec3 p = vec3(0,0,0);
    Normal(vec3 _p)
    {p = _p;};
    Normal(){};

};

class LocalGeo{
public:
    Point point = Point();
    Normal normal = Normal();
    LocalGeo(Point _p, Normal _n)
    {point = _p; normal = _n;};
};


class Shape{
public:
    float diffuse[3] = {0,0,0};
    float shininess = 0;
    float specular[3] = {0,0,0};
    float ambient[3] = {0.2,0.2,0.2};
    float emission[3] = {0,0,0};
    
    glm::mat4 transform = glm::mat4(1,0,0,0,
                                    0,1,0,0,
                                    0,0,1,0,
                                    0,0,0,1); // identity
    
    virtual bool intersectP(Ray &ray) = 0;
    virtual bool intersect(Ray &ray, float *thit, LocalGeo *local) = 0;
    virtual ~Shape() {};
    
    void set_diffuse(float _diffuse[3]){
        diffuse[0] = _diffuse[0];
        diffuse[1] = _diffuse[1];
        diffuse[2] = _diffuse[2];
    }
    
    void set_shininess(float _s){
        shininess = _s;
    }
    
    void set_specular(float _specular[3]){
        specular[0] = _specular[0];
        specular[1] = _specular[1];
        specular[2] = _specular[2];
    }
    
    void set_ambient(float _ambient[3]){
        ambient[0] = _ambient[0];
        ambient[1] = _ambient[1];
        ambient[2] = _ambient[2];
    }
    
    void set_emission(float _emission[3]){
        emission[0] = _emission[0];
        emission[1] = _emission[1];
        emission[2] = _emission[2];
    }
    
    void set_transform(glm::mat4 _transform){
        transform = _transform;
    }
};


class Sphere:public Shape{
    void getQuadraticFunction(Ray &ray, float &a, float &b, float &c);
public:
    vec3 center;
    float radius;
    Sphere(vec3, float);
    Sphere(){
        center = vec3(0,0,0);
        radius = 0;
    }
    ~Sphere(){};
    
    bool intersectP(Ray &ray);
    bool intersect(Ray &ray, float *thit, LocalGeo *local);
};

class Triangle:public Shape{
    void setNormal();
public:
    // counter clockwise
    vec3 vert1 = vec3(0,0,0);
    vec3 vert2 = vec3(0,0,0);
    vec3 vert3 = vec3(0,0,0);
    vec3 normal = vec3(0,0,0);
    
    Triangle(){};
    
    Triangle(vec3 _v1, vec3 _v2, vec3 _v3){
        vert1 = _v1;
        vert2 = _v2;
        vert3 = _v3;
    }
    ~Triangle(){};
    
    bool intersectP(Ray &ray);
    bool intersect(Ray &ray, float *thit, LocalGeo *local);
    
    // helper functions
    void baryCentric(vec4 p, float &alpha, float &beta, float &gamma);
};


class Intersection{
public:
    LocalGeo *localGeo = new LocalGeo(Point(), Normal());
    Shape * shape = new Sphere();
    Intersection(){};
};


