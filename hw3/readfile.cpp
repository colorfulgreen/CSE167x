#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>

using namespace std;

#include "readfile.h"
#include "Transform.h"

bool readvals(stringstream &s, const int numvals, float* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true; 
}

void rightmultiply(const glm::mat4 & M, stack<glm::mat4> &transfstack)
{
    glm::mat4 &T = transfstack.top();
    T = T * M;
}



void readfile(const char* filename, Scene *scene)
{
    string str, cmd;
    ifstream in;
    in.open(filename);
    
    if (in.is_open()) {
        
        stack <glm::mat4> transfstack;
        transfstack.push(glm::mat4(1.0));  // identity
        
        getline (in, str);
        
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines
                
                stringstream s(str);
                s >> cmd;
                
                // initialize variables
                
                float values[10];
                bool validinput;
                float diffuse[3];
                float specular[3];
                float ambient[3];
                float emission[3];
                float shininess;
                int max_vertices;
                
                
                if (cmd == "size") {
                    validinput = readvals(s,2,values);
                    if (validinput) {
                        scene->w = (int) values[0];
                        scene->h = (int) values[1];
                    }
                }else if (cmd == "camera"){
                    validinput = readvals(s, 10, values);
                    if (validinput){
                        vec3 lookFrom = vec3(values[0], values[1], values[2]);
                        vec3 lookAt = vec3(values[3], values[4], values[5]);
                        vec3 up = vec3(values[6], values[7], values[8]);

                        scene->camera->lookFrom = lookFrom;
                        scene->camera->lookAt = lookAt;
                        scene->camera->fovy = values[9];
                        vec3 zvec = lookFrom - lookAt;
                        scene->camera->up = Transform::upvector(up, zvec);
                        
                        scene->camera->setCoord();
                    }
                }else if(cmd == "diffuse"){
                    validinput = readvals(s, 3, values);
                    diffuse[0] = values[0];
                    diffuse[1] = values[1];
                    diffuse[2] = values[2];
                }
                else if (cmd == "shininess"){
                    validinput = readvals(s, 1, values);
                    shininess = values[0];
                }
                else if (cmd == "specular"){
                    validinput = readvals(s, 3, values);
                    specular[0] = values[0];
                    specular[1] = values[1];
                    specular[2] = values[2];
                }
                else if (cmd == "emission"){
                    validinput = readvals(s, 3, values);
                    emission[0] = values[0];
                    emission[1] = values[1];
                    emission[2] = values[2];
                    
                }
                else if (cmd == "sphere" || cmd == "tri"){
                    if (scene->num_objects == max_objects) {
                      cerr << "Reached Maximum Number of Objects " << scene->num_objects << " Will ignore further objects\n";
                    } else {
                      if (cmd == "sphere") {
                        validinput = readvals(s, 4, values);
                        scene->shapes[scene->num_objects] = new Sphere(vec3(values[0], values[1], values[2]), values[3]);
                      } else if (cmd == "tri") {
                        validinput = readvals(s, 3, values);
                        scene->shapes[scene->num_objects] = new Triangle(scene->vertices[(int)values[0]],
                                                                         scene->vertices[(int)values[1]],
                                                                         scene->vertices[(int)values[2]]);
                      }
                        // set material
                        scene->shapes[scene->num_objects]->set_diffuse(diffuse);
                        scene->shapes[scene->num_objects]->set_shininess(shininess);
                        scene->shapes[scene->num_objects]->set_specular(specular);
                        scene->shapes[scene->num_objects]->set_ambient(ambient);
                        scene->shapes[scene->num_objects]->set_emission(emission);
                        // set transform
                        scene->shapes[scene->num_objects]->set_transform(transfstack.top());
                        scene->num_objects += 1;
                    }                    
                }
                else if (cmd == "ambient"){
                    validinput = readvals(s, 3, values);
                    ambient[0] = values[0];
                    ambient[1] = values[1];
                    ambient[2] = values[2];
                }else if (cmd == "point"){
                    if (scene->num_lights < max_lights) {
                        validinput = readvals(s, 6, values);
                        vec3 pos = vec3(values[0], values[1], values[2]);
                        Color color = Color(values[3], values[4], values[5]);
                        scene->lights[scene->num_lights] = new PointLight(color, pos);
                        scene->num_lights += 1;
                    }else{
                        printf("max number of lights reached\n");
                    }
                }
                else if (cmd == "directional"){
                    if (scene->num_lights < max_lights) {
                        validinput = readvals(s, 6, values);
                        vec3 pos = vec3(values[0], values[1], values[2]);
                        Color color = Color(values[3], values[4], values[5]);
                        scene->lights[scene->num_lights] = new DirectionalLight(color, pos);
                        scene->num_lights += 1;
                    }else{
                        printf("max number of lights reached\n");
                    }
                }
                else if (cmd == "maxverts"){
                    validinput = readvals(s, 1, values);
                    max_vertices = (int)values[0];
                    scene->max_vertices = max_vertices;
                }
                else if (cmd == "vertex"){
                    validinput = readvals(s, 3, values);
                    if (scene->current_vertex >= scene->max_vertices){
                        printf("max number of vertices reached\n");
                    }else{
                        scene->vertices[scene->current_vertex] = vec3(values[0], values[1], values[2]);
                        scene->current_vertex += 1;
                    }
                }
                
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        glm::mat4 translate_m = Transform::translate(values[0], values[1], values[2]);
                        rightmultiply(translate_m, transfstack);
                    }
                }
                
                else if (cmd == "scale"){
                    validinput = readvals(s, 3, values);
                    rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
                }
                
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values);
                    if (validinput) {
                        mat3 r = Transform::rotate(values[3], vec3(values[0], values[1], values[2]));
                        rightmultiply(mat4(r), transfstack);
                    }
                }
                
                else if (cmd == "maxdepth"){
                    validinput = readvals(s,1, values);
                    scene->maxdepth = (int)(values[0]);
                }

                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    } else {
                        transfstack.pop();
                    }
                }
                
                else if (cmd == "attenuation"){
                    validinput = readvals(s, 3, values);
                    float _a[3] = {values[0],values[1],values[2]};
                    scene->set_attenuation(_a);
                }

            }
            getline (in, str);
        }
    }
    else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}
