#ifndef BSPLINE_H
#define BSPLINE_H

#include "visualobject.h"
#include "QVector3D"
#include "rollingball.h"
class bSpline : public VisualObject
{
public:
    bSpline(VisualObject *b);
    bSpline(std::vector<QVector3D> p);
    ~bSpline() override;

    void init() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;

protected:
    VisualObject* ball{nullptr};
private:
    int findKnoteIntervall(float x);
    QVector3D evaluateBSpline (float x);
    static const int d = 2;
    static const int n = 5;
    float t[d+n+1]{0.f,0.f,0.f,1.f,2.f,3.f,3.f,3.f};
    std::vector<QVector3D> c;
    float ti{0.001f};
    bool make{false};
};

#endif // BSPLINE_H
