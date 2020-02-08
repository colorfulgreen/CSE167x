// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.
#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  mat3 ret;
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1. 
  float radians = degrees * pi / 180;
  float cosRad = cos(radians);
  float sinRad = sin(radians);
  // TODO why we need to normalize the axis?
  vec3 normAxis = normalize(axis);
  mat3 I(1.0);
  mat3 m3(0,           -normAxis.z,  normAxis.y,
          normAxis.z,   0,          -normAxis.x,
          -normAxis.y,  normAxis.x,  0);
  ret = cosRad *I + (1-cosRad)*glm::outerProduct(normAxis, normAxis) + sinRad * m3;
  // TODO why not transpose in HW1 ?
  return transpose(ret);
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  
  mat3 rotationMatrix = rotate(degrees, up);
  eye = rotationMatrix * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.  
  vec3 axis = cross(up, eye);

  mat3 rotationMatrix = rotate(-degrees, axis);
  eye = rotationMatrix * eye;
  up = rotationMatrix * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  

	// Create a coordinate frame for the camera
	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, eye));
	vec3 v = normalize(cross(w, u));

	// Define a rotation matrix
	// Apply appropriate translation for camera (eye) location
	mat4 ret(u.x, u.y, u.z, dot(u, -eye),
					 v.x, v.y, v.z, dot(v, -eye),
					 w.x, w.y, w.z, dot(w, -eye),
					 0,   0,   0,   1);

	return transpose(ret);
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  

  // NOTE Fovy, aspect control fov in x, y directions
  // zNear, zFar control viewing frustum
  float d = 1 / tan(fovy * 0.5 * M_PI / 180);
  float zRange = zFar - zNear; 
  mat4 ret(d/aspect, 0,  0,                    0,
           0,        d,  0,                    0,
           0,        0,  -(zFar+zNear)/zRange, -2*zFar*zNear/zRange,
           0,        0,  -1,                   0);
 
  /* TODO ???
  ret[0][0] = scl/aspect;
  ret[1][1] = scl;
  ret[2][2] = -zFar / zRange;
  ret[3][2] = -zFar * zNear / zRange;
  ret[2][3] = -1;
  ret[3][3] = 0;
  */
  
  return transpose(ret);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  // YOUR CODE FOR HW2 HERE
  // Implement scaling 
  mat4 ret = mat4(sx, 0, 0, 0,
                  0, sy, 0, 0,
                  0, 0, sz, 0,
                  0, 0, 0, 1);
  return transpose(ret);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  // YOUR CODE FOR HW2 HERE
  // Implement translation 
  mat4 ret = mat4(1, 0, 0, tx,
                  0, 1, 0, ty,
                  0, 0, 1, tz,
                  0, 0, 0, 1);
  return transpose(ret);
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
