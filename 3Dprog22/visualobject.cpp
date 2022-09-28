#include "visualobject.h"
#include <iostream>
#include <fstream>
#include <QVector2D>
#include <QVector3D>
#include <string.h>
#include <sstream>

VisualObject::VisualObject() {  }

VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
   //   mVertices.empty();
}

std::pair<float, float> VisualObject::getPosition2D()
{
    auto col = mPosition.column(3);
    return std::pair<float,float>(col.x(),col.z());
}

void VisualObject::setName(std::string name)
{
    navn = name;
}

std::string VisualObject::getName() const
{
    return navn;
}

void VisualObject::setShder(int s)
{
    shaderType = s;
}

int VisualObject::getShader() const
{
    return shaderType;
}

void VisualObject::collision()
{

}

void VisualObject::readObject(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open()) {
//        std::vector<std::vector<float>> temp_vertices;
        std::vector<QVector3D> temp_vertices;
//        temp_vertices.push_back(std::vector<float>());
//        temp_vertices.push_back(std::vector<float>());
//        temp_vertices.push_back(std::vector<float>());
//        std::vector<std::vector<float>> temp_uvs;
        std::vector<QVector2D> temp_uvs;
//        temp_uvs.push_back(std::vector<float>());
//        temp_uvs.push_back(std::vector<float>());

//        std::vector<std::vector<float>> temp_normals;
//        temp_normals.push_back(std::vector<float>());
//        temp_normals.push_back(std::vector<float>());
//        temp_normals.push_back(std::vector<float>());
        std::vector<QVector3D> temp_normals;



        std::string line;
        float f1 = 0,f2 = 0,f3 = 0;
        int ver[3] = {0};
        char start[10];
        char s;
        inn >> start;
        while (!inn.eof()) {
            std::vector<float> input;
            if( std::strcmp( start, "v" ) == 0 ){
                getline(inn,line);
                std::stringstream iss(line); //istringstream
                iss >> f1 >> f2 >> f3;
                QVector3D temp{f1, f2, f3};
                temp_vertices.push_back(temp);
//                input.push_back(f1);
//                input.push_back(f2);
//                input.push_back(f3);
//                temp_vertices.push_back(input);
//                temp_vertices[0].push_back(0.0f);
//                temp_vertices[1].push_back(f2);
//                temp_vertices[2].push_back(f3);
            }
            else if( std::strcmp( start, "vt" ) == 0 ){
                getline(inn,line);
                std::stringstream iss(line);
                iss >> f1 >> f2;
                QVector2D temp{f1, f2};
                temp_uvs.push_back(temp);
//                temp_uvs[0].push_back(f1);
//                temp_uvs[1].push_back(f2);
            }
            else if( std::strcmp( start, "vn" ) == 0 ){
                getline(inn,line);
                std::stringstream iss(line);
                iss >> f1 >> f2 >> f3;
                QVector3D temp{f1, f2, f3};
                temp_normals.push_back(temp);
//                temp_normals[0].push_back(f1);
//                temp_normals[1].push_back(f2);
//                temp_normals[2].push_back(f3);
            }
            else if(std::strcmp( start, "f" ) == 0){
                getline(inn,line);
                std::stringstream iss(line);
                for(int i = 0; i<3; i++){
                    iss >> ver[0] >> s >> ver[1] >> s >> ver[2];
                    mVertices.push_back(Vertex{temp_vertices[ver[0]-1].x(), temp_vertices[ver[0]-1].y(), temp_vertices[ver[0]-1].z(),
                                               temp_normals[ver[2]-1].x(), temp_normals[ver[2]-1].y(), temp_normals[ver[2]-1].z(),
                                               temp_uvs[ver[1]-1].x(), temp_uvs[ver[1]-1].y()});
                }
//                iss >> ver[0] >> s >> ver[1] >> s >> ver[2] >> ver[3] >> s >> ver[4] >> s >> ver[5] >> ver[6] >> s >> ver[7] >> s >> ver[8];
//                mVertices.push_back(Vertex{temp_vertices[ver[0]-1].x(), temp_vertices[ver[0]-1].y(), temp_vertices[ver[0]-1].z(),
//                                           temp_normals[ver[2]-1].x(), temp_normals[ver[2]-1].y(), temp_normals[ver[2]-1].z(),
//                                           temp_uvs[ver[1]-1].x(), temp_uvs[ver[1]-1].y()});

//                mVertices.push_back(Vertex{temp_vertices[0][ver[0]-1], temp_vertices[1][ver[0]-1], temp_vertices[2][ver[0]-1],
//                                           temp_normals[0][ver[2]-1], temp_normals[2][ver[2]-1], temp_normals[3][ver[2]-1],
//                                           temp_uvs[0][ver[1]-1], temp_uvs[0][ver[1]-1]});

            }
            else{
                 getline(inn,line);
            }
            inn >> start;
        }
    }
}

int VisualObject::aIndices()
{
    return 0;
}

Vertex VisualObject::getVertex(int x)
{
    return mVertices.at(mIndices.at(x));
}

