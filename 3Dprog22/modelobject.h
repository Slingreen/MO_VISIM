#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "visualobject.h"

class ModelObject : public VisualObject
{
public:
    ModelObject();
    ModelObject(std::string filnavn);
    ~ModelObject() override;
    void init(GLint matrixUniform) override;
    void init() override;
    void draw() override;
};

#endif // MODELOBJECT_H
