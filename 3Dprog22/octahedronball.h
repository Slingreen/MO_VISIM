#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H
#include "visualobject.h"
#include <QMatrix4x4>
#include <QVector3D>
#include "vertex.h"

class Vertex;
typedef QVector3D Vec3;
class OctahedronBall : public VisualObject {
private:
   int m_rekursjoner;
   int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
   void lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3);
   void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
   void oktaederUnitBall();
public:
   OctahedronBall(int n);
   ~OctahedronBall();
   void init(GLint matrixUniform) override;
   void init() override;
   void draw() override;
};

#endif
