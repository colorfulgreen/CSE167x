#pragma once
#include "Scene.h"
#include "variables.h"

bool Scene::intersect(Ray &ray, float *thit, Intersection *in){
    // go through every object in scene, return closest
    float _thit = INFINITY;
    LocalGeo _local = LocalGeo(Point(vec4(0,0,0,1)), Normal(vec3(0,0,0)));
    Shape *_shape = new Sphere();
    
    bool hit = false;
    
    for (int i = 0; i < num_objects; i++) {
        if (shapes[i]->intersect(ray, thit, in->localGeo)){
            if (*thit <= _thit) {
                _thit = *thit;
                _local = *(in->localGeo);
                _shape = shapes[i];
            }
            hit = true;
        }
    }
    
    *thit = _thit;
    *(in->localGeo) = _local;
    in->shape = _shape;
    
    return hit;
    
}

bool Scene::intersectP(Ray &ray, Shape *shape){
    
    // set min t
    float threshold = 0.0001;
    // t also needs to be less than shape to light
    float *t = new float(0);
    LocalGeo *local = new LocalGeo(Point(), Normal());
    
    for (int i = 0; i < num_objects; i++) {
        *t = 0;
        *local = LocalGeo(Point(), Normal());
        if (shapes[i] != shape) {
            if (shapes[i]->intersect(ray, t, local)) {
                if ((*t) >= threshold && (*t) < 1) {
                    delete t;
                    delete local;
                    return true;
                }
            }
        }
    }
    delete t;
    delete local;
    
    return false;
}

Ray Scene::createReflectedRay(LocalGeo local, Ray ray){
    vec4 v = vec4(local.normal.p,0);
    float x = 2;
    return Ray(local.point.p, glm::normalize(ray.dir + x * v), ray.t, ray.t_min, ray.t_max);
}

Color Scene::computePointLight(vec3 direction,
                               vec3 lightColor,
                               vec3 normal,
                               vec3 halfvec,
                               Shape* shape){
    
    // lambert: diffuse * light color * max (normal dot direction, 0);
    float nDotL = glm::dot(normal, direction);
    float m = nDotL > 0? nDotL:0;
    
    vec3 diffuse = vec3(shape->diffuse[0], shape->diffuse[1], shape->diffuse[2]);
    vec3 specular = vec3(shape->specular[0], shape->specular[1], shape->specular[2]);
    
    Color lambert = Color(diffuse.x * lightColor.x * m,
                          diffuse.y * lightColor.y * m,
                          diffuse.z * lightColor.z * m);
    
    /*if (m > 0.01) {
        printf("lambert: %.2f, %.2f, %.2f\n", lambert.r, lambert.g, lambert.b);
    }*/
    
    // phong: myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess)
    float nDotH = glm::dot(normal, halfvec);
    float n = nDotH > 0? nDotH : 0;
    float pn = powf(n, shape->shininess);
    
    Color phong = Color(specular.x * lightColor.x * pn,
                        specular.y * lightColor.y * pn,
                        specular.z * lightColor.z * pn);
    
    /*if (pn > 0.01 && (phong.r > 0 || phong.g > 0 || phong.b > 0)) {
        printf("normal: %.2f %.2f %.2f\n", normal.x, normal.y, normal.z);
        printf("halfvec: %.2f %.2f %.2f\n", halfvec.x, halfvec.y, halfvec.z);
        printf("nDotH: %.3f ", nDotH);
        printf("pn: %.3f ", pn);
        printf("phong: %.2f %.2f %.2f\n", phong.r, phong.g, phong.b);
    }*/
    
    return Color(lambert.r + phong.r,
                 lambert.g + phong.g,
                 lambert.b + phong.b);
    
    
}

void Scene::rayTrace(Ray &ray, int depth, Color *color) {
    
    if (depth > maxdepth) {
        *color = Color(0,0,0);
        return;
    }
   
    float *thit = new float(INFINITY);
    Intersection *in = new Intersection();
    if (intersect(ray, thit, in)) {
        *color = findColor(in);
        // recurse
        Ray reflectedRay = createReflectedRay(*(in->localGeo), ray);
        Color *tempColor = new Color(0,0,0);
        rayTrace(reflectedRay, depth+1, tempColor);
        (*color).r += (*tempColor).r;
        (*color).g += (*tempColor).g;
        (*color).b += (*tempColor).b;
        
        delete tempColor;
    }else{
        *color = Color(0, 0, 0);
    }
    
    delete thit;
    delete in;
    
}

Color Scene::findColor(Intersection *in) {
    
    Color color = Color(0, 0, 0);
    
    vec3 eyepos = vec3(0,0,0);
    vec4 _mypos = in->shape->transform * in->localGeo->point.p;
    vec3 mypos = vec3(_mypos.x / _mypos.w, _mypos.y / _mypos.w, _mypos.z / _mypos.w);
    vec3 eyedirn = glm::normalize(eyepos - mypos);
    // normal: M_-t
    vec4 _normal = glm::transpose(glm::inverse(in->shape->transform)) * vec4(in->localGeo->normal.p,0);
    vec3 normal = glm::normalize(vec3(_normal.x,
                                      _normal.y,
                                      _normal.z));
    
    // add ambient
    color.r += in->shape->ambient[0];
    color.g += in->shape->ambient[1];
    color.b += in->shape->ambient[2];
    
    // add emission
    color.r += in->shape->emission[0];
    color.g += in->shape->emission[1];
    color.b += in->shape->emission[2];
    
    
    for (int i = 0; i < num_lights; i++) {
        
        if (dynamic_cast<PointLight*>(lights[i]) != 0) {
            PointLight* light = dynamic_cast<PointLight*>(lights[i]);
            vec3 lightposn = light->position;
            
            vec3 direction = glm::normalize(lightposn - mypos);
            
            vec3 lightColor = vec3(light->color.r, light->color.g, light->color.b);
            vec3 halfvec = glm::normalize(eyedirn + direction);
            
            float distance = glm::distance(lightposn, mypos);
            float attenFactor = 1.0f / (attenuation[0] + distance * attenuation[1] + distance * distance * attenuation[2]);
            
            // shadows
            Ray shadowRay = Ray(mypos, lightposn-mypos, 0, 0, 100);
            bool shadow = intersectP(shadowRay, in->shape);
            /*if (shadow) {
                lightColor;
            }*/
            
            if (!shadow) {
                Color pointColor = computePointLight(direction,
                                               lightColor,
                                               normal,
                                               halfvec,
                                               in->shape);
        
            
                color.r += attenFactor * pointColor.r;
                color.g += attenFactor * pointColor.g;
                color.b += attenFactor * pointColor.b;
            }
        }
        
        if (dynamic_cast<DirectionalLight*>(lights[i]) != 0) {
            DirectionalLight* light = dynamic_cast<DirectionalLight*>(lights[i]);
            
            vec3 lightposn = light->position;
            
            vec3 direction = glm::normalize(lightposn);
            vec3 lightColor =vec3(light->color.r, light->color.g, light->color.b);
            vec3 halfvec = glm::normalize(eyedirn + direction);
            
            float distance = glm::distance(lightposn, mypos);
            float attenFactor = 1.0f / (attenuation[0] + distance * attenuation[1] + distance * distance * attenuation[2]);
            
            // shadows
            Ray shadowRay = Ray(mypos, lightposn-mypos, 0, 0, 100);
            
            if (!intersectP(shadowRay, in->shape)) {
                Color directionalColor = computePointLight(direction, lightColor, normal, halfvec, in->shape);
                
                color.r += attenFactor * directionalColor.r;
                color.g += attenFactor * directionalColor.g;
                color.b += attenFactor * directionalColor.b;
            }
        }
        
    }
    
    return color;
}
