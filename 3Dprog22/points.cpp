#include "points.h"
#include <vector>

Points::Points()
{

}

Points::Points(QVector3D f, int i)
{
    Values = f;
    id = i;
}

Points::~Points()
{

}

void Points::insert_neighbor(int id)
{
    neighbor_id.push_back(id);
}

std::vector<int> Points::reteve_neighbors()
{
    return neighbor_id;
}

void Points::Delete_neighbor(int n_id)
{
    int k{0};
    for (int i = 0; i < neighbor_id.size(); i++){
        if (neighbor_id[i] == n_id){
            k = i;
        }
    }
    neighbor_id.erase(neighbor_id.begin() + k);
}

void Points::insert_VectorValues(float x, float y, float z)
{
    Values = QVector3D(x, y, z);
}

QVector3D Points::return_Values()
{
    return Values;
}

int Points::get_id()
{
    return id;
}
