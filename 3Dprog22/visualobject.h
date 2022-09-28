#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"



class VisualObject: public QOpenGLFunctions_4_1_Core
{
public:
   VisualObject();
   ~VisualObject();
   virtual void init()=0;
   virtual void init(GLint matrixUniform)=0;
   virtual void draw()=0;
   virtual void move(float x, float y, float z) {};
   virtual void move(float dt) {};

   std::pair<float, float> getPosition2D();
   void setName(std::string name);
   std::string getName() const;

   void setShder(int s);
   int getShader() const;
   QMatrix4x4 mMatrix;

   virtual void collision();
   void readObject(std::string filnavn);

   virtual QVector3D PointOnTriangle(float x, float y, float z){ return {0,0,0}; };
   virtual int aIndices();
   Vertex getVertex(int x);
protected:
   std::vector<Vertex> mVertices;
   std::vector<GLuint> mIndices;


   GLuint mVAO{0};
   GLuint mVBO{0};
   GLuint mEAB{0};
//   GLuint mEBO{0};
   GLuint mIBO{0};

   GLint mMatrixUniform{0};


   QMatrix4x4 mPosition;
   QMatrix4x4 mRotation;
   QMatrix4x4 mScale;
   int shaderType{0};

private:
   std::string navn;

};

#endif // VISUALOBJECT_H
