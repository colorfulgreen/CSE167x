#pragma once
#include "Transform.h"
#include "variables.h"

class Ray{
public:
    vec4 pos = vec4(0,0,0,1);
    vec4 dir = vec4(0,0,0,0);
    float t = 0.0;
    float t_min = 0.0;
    float t_max = 0.0;
    
    Ray(vec4 _pos, vec4 _dir, float _t, float _t_min, float _t_max)
    {
        pos = _pos;
        dir = _dir;
        t = _t;
        t_min = _t_min;
        t_max = _t_max;
    }
    
    Ray(vec3 _pos, vec3 _dir, float _t, float _t_min, float _t_max){
        pos = vec4(_pos,1);
        dir = vec4(_dir,0);
        t = _t;
        t_min = _t_min;
        t_max = _t_max;
    }
};