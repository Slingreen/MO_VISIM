#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    ~InteractiveObject() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float x, float y, float z) override;
    void collision();
protected:
    float mx, mz, my;
};

#endif // INTERACTIVEOBJECT_H
