#ifndef LASMAP_H
#define LASMAP_H

#include "trianglesurface.h"

class lasmap : public TriangleSurface
{
public:
    lasmap();
    lasmap(std::string filanvn);
    ~lasmap() override;

    void init(GLint shader) override;
    void init() override;
    void draw() override;

private:
    void readFile(std::string filnavn);

    std::vector <std::vector<int>> Surface;

    std::vector <QVector3D> Points;
    std::vector <QVector3D> new_Points;
};

#endif // LASMAP_H
