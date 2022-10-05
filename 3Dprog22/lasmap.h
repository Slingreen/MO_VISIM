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
protected:
    std::vector <QVector3D> Points;
    const int sizeincrease{2};
private:
    void readFile(std::string filnavn);
    void Reduce_Points(float x, float y, float zMin, float e, float xMin, float yMin, float xMax);

    std::vector <std::vector<int>> Surface;

    std::vector <QVector3D> new_Points;

    int temp[5];
    float lasth{0};
    int loop_counter{1};
};

#endif // LASMAP_H
