#ifndef QUAD_H
#define QUAD_H

#include "visualobject.h"

class Quad : public VisualObject
{
public:
    Quad();
    void init(GLint Shader) override;
    void init() override;
    void draw() override;
};


#endif // QUAD_H
