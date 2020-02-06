// Transform.cpp: implementation of the Transform class.

#include <stdio.h>

#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	// YOUR CODE FOR HW1 HERE

    float radians = degrees * pi / 180;
    float cosRad = cos(radians);
    float sinRad = sin(radians);
    vec3 normAxis = normalize(axis);
    mat3 R, I(1.0);
    mat3 m3(0,           -normAxis.z,  normAxis.y,
            normAxis.z,   0,          -normAxis.x,
            -normAxis.y,  normAxis.x,  0);

    /* TODO
    error: invalid operands to binary expression ('double' and 'mat3'
    R = cos(radians) * m1;
    */
    R = cosRad *I + (1-cosRad)*glm::outerProduct(normAxis, normAxis) + sinRad * m3;
	return R;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE

    mat3 rotationMatrix = rotate(-degrees, up);
    eye = rotationMatrix * eye;
    up = rotationMatrix * up;

    printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));

}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE 
    vec3 axis = glm::cross(up, eye);

    mat3 rotationMatrix = rotate(degrees, axis);
    eye = rotationMatrix * eye;
    up = rotationMatrix * up;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
	// YOUR CODE FOR HW1 HERE

    // Create a coordinate frame for the camera
    vec3 w = normalize(eye);
    vec3 u = normalize(cross(up, eye));
    vec3 v = normalize(cross(w, u));
   
    // Define a rotation matrix
    mat4 m1(u.x, u.y, u.z, dot(u, -eye),
            v.x, v.y, v.z, dot(v, -eye),
            w.x, w.y, w.z, dot(w, -eye),
            0,   0,   0,   1);
   
    // Apply appropriate translation for camera (eye) location
	return transpose(m1);
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
