#include "pointlight.h"

PointLight::PointLight()
{
    mVertices.insert( mVertices.end(),
       {
        Vertex{-0.25f, -0.25f,  0.25f,    0.8f,  0.8f,  0.3f,    0.0f,  0.0f},   //Left low
        Vertex{ 0.25f, -0.25f,  0.25f,    0.8f,  0.8f,  0.3f,    1.0f,  0.0f},   //Right low
        Vertex{ 0.0f,   0.25f,  0.00f,    0.8f,  0.8f,  0.3f,    0.5f,  0.5f},  //Top
        Vertex{ 0.0f,  -0.25f, -0.25f,    0.8f,  0.8f,  0.3f,    0.5f,  0.5f}   //Back low
        });

       mIndices.insert( mIndices.end(),
       { 0, 1, 2,
         1, 3, 2,
         3, 0, 2,
         0, 3, 1
       });

       mMatrix.setToIdentity();
}

void PointLight::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER,
                  mVertices.size()*sizeof(Vertex),
                  mVertices.data(),
                  GL_STATIC_DRAW
                  );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)0
                          );

    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)(3 * sizeof(GLfloat))
                          );

    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)( 6 * sizeof(GLfloat) )
                          );
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mIndices.size() * sizeof(GLuint),
                 mIndices.data(),
                 GL_STATIC_DRAW
                 );

    glBindVertexArray(0);
}

void PointLight::init(GLint Shader)
{

}

void PointLight::draw()
{
    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glUniformMatrix4fv( mMatrixUniform,
                        1,
                        GL_FALSE,
                        mMatrix.constData());
    glBindVertexArray(0);
}
