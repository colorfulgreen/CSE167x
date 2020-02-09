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
  mat3 ret, I(1.0);
  float radians = degrees * pi / 180;
  float cosRad = cos(radians);
  float sinRad = sin(radians);
  vec3 normAxis = normalize(axis);
  mat3 m3(0,           -normAxis.z,  normAxis.y,
          normAxis.z,   0,          -normAxis.x,
          -normAxis.y,  normAxis.x,  0);
  ret = cosRad * I + (1-cosRad)*glm::outerProduct(normAxis, normAxis) + sinRad * m3;
  return transpose(ret);
}


mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
  mat4 ret = mat4(1, 0, 0, tx,
                  0, 1, 0, ty,
                  0, 0, 1, tz,
                  0, 0, 0, 1);
  return transpose(ret);
}


Transform::Transform()
{
    
}

Transform::~Transform()
{
    
}
