#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "visualobject.h"

class PointLight : public VisualObject
{
public:
    PointLight();
    virtual void init() override;
    virtual void init(GLint Shader) override;
    virtual void draw() override;

    GLfloat mAmbientStrength{0.3f};
    QVector3D mAmbietntColor{0.3f, 0.3f, 0.3f};

    GLfloat mLightStrength{0.7f};
    QVector3D mLightColor{0.9f, 0.9f, 0.3f};

    GLfloat mSpecularStrength{0.9f};
    //      mSpecularStrenght
    GLint mSpecularExponent{4};
};

#endif // POINTLIGHT_H
