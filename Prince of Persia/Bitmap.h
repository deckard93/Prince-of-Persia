#pragma once
#include <d3d9.h>

struct BitmapFileHeader {                
   unsigned int fileSize;                       
   unsigned short int reserved1, reserved2;
   unsigned int offsetToPixelData;                     
};

struct Pixel24 {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct BitmapInfoHeader {
   unsigned int headerSize;               
   int width,height;                
   unsigned short int planes;       
   unsigned short int bits;         
   unsigned int compression;        
   unsigned int imageSize;          
   int xResolution,yResolution;     
   unsigned int nColours;           
   unsigned int importantColours;   
};

void LoadBmp(const char* filename, D3DCOLOR * surface);