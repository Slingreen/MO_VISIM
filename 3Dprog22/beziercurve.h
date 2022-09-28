#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "visualobject.h"
#include "QVector3D"

class BezierCurve : public VisualObject
{
public:
    BezierCurve();
    //BezierCurve(std::string filnavn);
    ~BezierCurve() override;
    //void readFile(std::string filnavn);
    //void writeFile(std::string filnavn);
    void init(GLint shader) override;
    void draw() override;
    QVector3D evaluateBezier(float t);
private:
    int d = 3;
    QVector3D c[4];
};

#endif // BEZIERCURVE_H
