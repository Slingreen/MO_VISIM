#include "texturetriangle.h"

TextureTriangle::TextureTriangle()
{
    shaderType = 1;
                               // Positions            // Colors       //UV
    mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
    mMatrix.setToIdentity();
    //mMatrix = mPosition * mRotation * mScale;
}

TextureTriangle::~TextureTriangle() {    }

void TextureTriangle::init(GLint Shader)
{

}

void TextureTriangle::init() {

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
                          reinterpret_cast<GLvoid*>(0)
                          );

    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))
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

    glBindVertexArray(0);
}

void TextureTriangle::draw()
{
    //What objects to draw
    glBindVertexArray( mVAO );


    glUniformMatrix4fv( mMatrixUniform,
                        1,
                        GL_FALSE,
                        mMatrix.constData());

    //Draw Call
    glDrawArrays(GL_TRIANGLES,
                 0,
                 mVertices.size());

    glBindVertexArray(0);
    /*
    glUseProgram(mShaderProgram[1]->getProgram());
    glUniformMatrix4fv( vMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
    glUniformMatrix4fv( mMatrixUniform1, 1, GL_TRUE, this->mMatrix.constData());
    glUniform1i(mTextureUniform, 1);
    //*/
}
