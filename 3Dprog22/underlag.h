#ifndef UNDERLAG_H
#define UNDERLAG_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include <QBitmap>
#include <string>

#include "Bitmap/bitmap_image.hpp"

#include "vertex.h"
#include "visualobject.h"
#include "texture.h"

class Underlag : public VisualObject
{
private:
    std::string fil = "../3Dprog22/Assets/height_map.bmp";
    float h = 0.1f/*0.002f*/, l = 1.f/*0.1f*/;
    int maxx = 512;
    int maxz = 512;
    bool inner = true;
    bitmap_image* image; //bruker klasse som lar oss lese inn bitmaps direkte til et bitmap, hentet: https://www.partow.net/programming/bitmap/

    QVector3D tempNormal;

    QVector3D NormalMath(QVector3D A, QVector3D B, QVector3D C);

    typedef unsigned short int OWORD;    //should be 16 bit
    typedef unsigned int ODWORD;         //should be 32 bit
    typedef int OLONG;                   //should be 32 bit

    //Bitmap file header
    struct OBITMAPFILEHEADER {
        OWORD  bfType;      //will contain ascii BM == 0x4D42 == 19778 decimal
        ODWORD bfSize;      //often set to 0, not to actual size
        OWORD  bfReserved1;
        OWORD  bfReserved2;
        ODWORD bfOffBits;   //often set to 0, not to offset
    };

    //DIB header (bitmap information header)
    struct OBITMAPINFOHEADER {
        ODWORD biSize;      //size of this header (in the file)
        OLONG  biWidth;
        OLONG  biHeight;
        OWORD  biPlanes;
        OWORD  biBitCount;
        ODWORD biCompression;   //most often set to 0 if no compression
        ODWORD biSizeImage; //size of actual image
        OLONG  biXPelsPerMeter;
        OLONG  biYPelsPerMeter;
        ODWORD biClrUsed;
        ODWORD biClrImportant;
    };

    QVector3D P, Q, R;
    float u, v, w;

    void findPoints(float x,float z);
    void findTriangle(float x, float z);
    int index = 0;

public:
   Underlag();
   //Underlag(const std::string fil);
   ~Underlag() override;
   void init(GLint matrixUniform) override;
   void draw() override;
   void init() override;

   float BeregnY(const float x, const float z);
};

#endif // UNDERLAG_H
