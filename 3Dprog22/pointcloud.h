#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "lasmap.h"

class Pointcloud : public lasmap
{
public:
    Pointcloud();
    Pointcloud(std::string filanvn);
    ~Pointcloud() override;

    void init(GLint shader) override;
    void init() override;
    void draw() override;

private:
    void readFile(std::string filnavn);
};

#endif // POINTCLOUD_H
