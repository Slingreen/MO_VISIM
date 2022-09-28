#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    RollingBall(int n, VisualObject* surface);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void init() override;
    void draw() override;
    void move(float dt) override;
//    void setSurface(VisualObject* surface) { triangle_surface = surface; }
protected:
    VisualObject* triangle_surface;
private:
    void UpdateNormal();
    Vec3 old_normal{0.0, 1.0, 0.0};
    int old_index{0};

    const float g{9.81};
    Vec3 a;
    Vec3 v_0{0,0,0};
    Vec3 p{0,15,0};
    float y{15};
    float oy{15};
    float r{0.5};
};
#endif // ROLLINGBALL_H