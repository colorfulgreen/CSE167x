
#include "Film.h"
#include <FreeImage.h>

#include <stdio.h>

Film::Film(int _w, int _h){
    w = _w;
    h = _h;
}

void Film::writeImage(){
    
    printf("outputting image...\n");
    
    FreeImage_Initialise();
    
    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
    
    char fname[] = "output.png";
    
    FreeImage_Save(FIF_PNG, img, fname, 0);
    
    FreeImage_DeInitialise();
}

void Film::commit(Sample sample, Color color){
    // assign color to pixels
    int x = (int) (sample.x - 0.5);
    int y = (int) (sample.y - 0.5);
    
    int i = y * w + x;
    
    int j = i * 3; // starting element
   
    // TODO
    color.clamp();
    
    // free image using BGR order
    pixels[j] = (unsigned char) (color.b * 255);
    pixels[j+1] = (unsigned char) (color.g * 255);
    pixels[j+2] = (unsigned char) (color.r * 255);
}

unsigned char Film::getPixel(int i){
    return pixels[i];
}
