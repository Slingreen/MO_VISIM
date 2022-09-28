#ifndef TROPHY_H
#define TROPHY_H

#include "interactiveobject.h"

class Trophy : public InteractiveObject
{
public:
    Trophy();
    Trophy(float x, float y, float z);
    ~Trophy() override;
    void init() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void collision() override;
private:
    void DrawKube();
};

#endif // TROPHY_H
