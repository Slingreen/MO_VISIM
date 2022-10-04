#ifndef POINTS_H
#define POINTS_H

#include <QVector3D>

class Points
{
public:
    Points();
    Points(QVector3D f, int i);
    ~Points();

    void insert_neighbor(int id);
    std::vector<int> reteve_neighbors();
    void Delete_neighbor(int neighbors_id);
    void insert_VectorValues(float x, float y, float z);
    QVector3D return_Values();
    int get_id();
private:
    int id;
    std::vector <int> neighbor_id;

    QVector3D Values;
};

#endif // POINTS_H
