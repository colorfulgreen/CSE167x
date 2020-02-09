#pragma once

class Color{
public:
    float r,g,b;
    Color(float _r, float _g, float _b){
        r = _r;
        g = _g;
        b = _b;
    };
    
    void clamp(){
        r = r > 1? 1:r;
        g = g > 1? 1:g;
        b = b > 1? 1:b;
    }
};