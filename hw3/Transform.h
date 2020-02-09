#pragma once

#include <glm/glm.hpp>
#include "variables.h"

typedef glm::vec3 vec3 ;
const float pi = 3.14159265 ;

class Transform
{
public:
    Transform();
    virtual ~Transform();
    static vec3 upvector(const vec3 &up, const vec3 &zvec) ;
    static mat4 scale(const float &sx, const float &sy, const float &sz) ;
    static mat3 rotate(const float degrees, const vec3& axis);
    static mat4 translate(const float &tx, const float &ty, const float &tz);
};
