#pragma once

#include "Sampler.h"
#include "Color.h"

class Film{
private:
    unsigned char pixels[3*1000*1000] = {};
public:
    int w,h;
    Film(int, int);
    void commit(Sample sample, Color color);
    void writeImage();
    unsigned char getPixel(int);
};