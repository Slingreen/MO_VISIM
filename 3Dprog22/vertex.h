#ifndef VERTEX_H
#define VERTEX_H
#include <iostream>
#include <fstream>

class Vertex{
    friend std::ostream& operator<< (std::ostream&, const Vertex&);

    //! Overloaded ostream operator which reads all vertex data from an open textfile stream
    friend std::istream& operator>> (std::istream&, Vertex&);
private:
    float m_xyz[3];
    float m_normal[3];
    float m_st[2];
public:
    Vertex();
    //Vertex(float x = 0, float y = 0, float z = 0, float r = 0, float g = 0, float b = 0, float u = 0, float v = 0);
    Vertex(float x, float y, float z, float r, float g, float b, float u = 1, float v = 0);
    float GetX();
    float GetY();
    float GetZ();
    void GetXYZ(float (&x)[3]);
};

#endif // VERTEX_H
