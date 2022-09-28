#ifndef TEXTURETRIANGLE_H
#define TEXTURETRIANGLE_H

#include "visualobject.h"

class TextureTriangle : public VisualObject
{
public:
    TextureTriangle();
    ~TextureTriangle() override;
    void init(GLint Shader) override;
    void init() override;
    void draw() override;
};

#endif // TEXTURETRIANGLE_H
