#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H


#include "visualobject.h"
#include <iostream>
#include <fstream>
//#include "vector3d.h"

class TriangleSurface : public VisualObject
{
public:
   TriangleSurface();
   TriangleSurface(std::string filnavn);
   ~TriangleSurface() override;
   void readFile(std::string filnavn);
   void readFileI(std::string filnavn);
   void writeFile(std::string filnavn);
   void writeFileI(std::string filnavn);
   void init(GLint shader) override;
   void init() override;
   void draw() override;

   QVector3D PointOnTriangle(float x, float y, float z) override;
   int aIndices() override;

private:
   int n;
};


#endif // TRIANGLESURFACE_H
