#include "Transform.h"

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
    vec3 x = glm::cross(up,zvec);
    vec3 y = glm::cross(zvec,x);
    vec3 ret = glm::normalize(y);
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
    mat4 ret =  mat4(sx,0,0,0,
                     0,sy,0,0,
                     0,0,sz,0,
                     0,0,0,1);
    
    return ret;
    
}

mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    float actualDegrees = -1 * degrees;
    double theta = glm::radians(actualDegrees);
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);
    
    vec3 v = glm::normalize(axis);
    
    mat3 v_vt = mat3(
                     v.x * v.x, v.x * v.y, v.x * v.z,
                     v.x * v.y, v.y * v.y, v.y * v.z,
                     v.x * v.z, v.y * v.z, v.z * v.z
                     );
    
    mat3 m1 = v_vt * (1 - cosTheta);
    
    mat3 identity(1.0);
    
    mat3 m2 = identity * cosTheta;
    
    mat3 dual_matrix = glm::mat3(0, v.z, -v.y,
                                 -v.z, 0, v.x,
                                 v.y, -v.x,0);
    
    return m1 + m2 + dual_matrix * sinTheta;
    
}




Transform::Transform()
{
    
}

Transform::~Transform()
{
    
}
