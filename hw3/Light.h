#pragma once
#include "Color.h"
#include <string>


class Light{
public:
    std::string name;
    virtual ~Light(){};
};

class AmbientLight:public Light{
public:
    std::string name = "ambient";
    Color color = Color(0,0,0);
    
    AmbientLight(Color _color){
        color = _color;
    }
    
    ~AmbientLight(){};
    
};

class PointLight:public Light{
public:
    std::string name = "point";
    Color color = Color(0,0,0);
    vec3 position = vec3(0,0,0);
    
    PointLight(Color _color, vec3 _pos){
        color = _color;
        position = _pos;
    }
    
    ~PointLight(){};
};

class DirectionalLight:public Light{
public:
    std::string name = "directional";
    Color color = Color(0,0,0);
    vec3 position = vec3(0,0,0);
    
    DirectionalLight(Color _color, vec3 _pos){
        color = _color;
        position = _pos;
    }
    
    ~DirectionalLight(){};
};