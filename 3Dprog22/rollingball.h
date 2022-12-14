#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"
//#include "bspline.h"

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    RollingBall(int n, VisualObject* surface);
    RollingBall(int n, float x, float y, float z, VisualObject* surface);
//    RollingBall(int n, VisualObject* surface, bSpline *s);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void init() override;
    void draw() override;
    void move(float dt) override;
    bool splineCheck();
    std::vector<QVector3D> givePoints();
//    void setSurface(VisualObject* surface) { triangle_surface = surface; }
protected:
    VisualObject* triangle_surface{nullptr};
//    bSpline* spline{nullptr};
private:
    void UpdateNormal();
    Vec3 old_normal{0.0, 1.0, 0.0};
    int old_index{0};

    const float g{9.81};
    Vec3 a;
    Vec3 v_0{0,0,0};
    Vec3 p{0,15,0};
    float y{p.y()};
    float oy{p.y()};
    float r{0.1};

    bool onTriangle{false};
    bool freeFalling{true};
    bool landed{false};

    float ti{0.002f};   //  tids intervall
    float tm{0};        //  tids måling
    std::vector<QVector3D> loggedPoint;
    bool pointLogged{false};
};
#endif // ROLLINGBALL_H
