#include "trophy.h"

Trophy::Trophy()
{

    DrawKube();
    mPosition.translate(0,0,0);
    //mRotation.rotate(90, 1, 1);
    mScale.scale(0.25, 0.25, 0.25);

    mMatrix = mPosition * mRotation * mScale;

}

Trophy::Trophy(float x, float y, float z)
{
    DrawKube();
    mPosition.translate(x,y,z);
    //mRotation.rotate(90, 1, 1);
    mScale.scale(0.25, 0.25, 0.25);

    mMatrix = mPosition * mRotation * mScale;
}

Trophy::~Trophy()
{

}

void Trophy::init()
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

void Trophy::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER,
                  mVertices.size()*sizeof( Vertex ),
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
                          reinterpret_cast<GLvoid*>(0   ));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof( Vertex ),
                          reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void Trophy::draw()
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
}

void Trophy::collision()
{
     mPosition.setToIdentity();
     mPosition.translate(-3,-3,0);
     mMatrix = mPosition;
}

void Trophy::DrawKube()
{
    //Forside
    mVertices.push_back(Vertex{-1.0f, 1.0f, 1.0f, 1,1,0});
    mVertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f, 1,1,1});
    mVertices.push_back(Vertex{ 1.0f,-1.0f, 1.0f, 1,1,0});

    mVertices.push_back(Vertex{ 1.0f,-1.0f, 1.0f, 1,1,0});
    mVertices.push_back(Vertex{-1.0f,-1.0f, 1.0f, 1,1,1});
    mVertices.push_back(Vertex{-1.0f, 1.0f, 1.0f, 1,1,0});

    //Venstre side
    mVertices.push_back(Vertex{-1.0f, 1.0f, 1.0f, 0,1,0});
    mVertices.push_back(Vertex{-1.0f, 1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{-1.0f,-1.0f, 1.0f, 0,1,0});

    mVertices.push_back(Vertex{-1.0f,-1.0f, 1.0f ,0,1,0});
    mVertices.push_back(Vertex{-1.0f,-1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{-1.0f, 1.0f,-1.0f ,0,1,0});

    //Bakside
    mVertices.push_back(Vertex{-1.0f, 1.0f,-1.0f, 1,1,0});
    mVertices.push_back(Vertex{ 1.0f, 1.0f,-1.0f, 1,1,0});
    mVertices.push_back(Vertex{-1.0f,-1.0f,-1.0f, 1,1,0});

    mVertices.push_back(Vertex{-1.0f,-1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f, 1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f,-1.0f,-1.0f, 0,1,0});

    //Høyre side
    mVertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f, 1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f,-1.0f, 1.0f, 0,1,0});

    mVertices.push_back(Vertex{ 1.0f,-1.0f, 1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f,-1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f, 1.0f,-1.0f, 0,1,0});

    //Bunn
    mVertices.push_back(Vertex{ 1.0f,-1.0f, 1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f,-1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{-1.0f,-1.0f, 1.0f, 0,1,0});

    mVertices.push_back(Vertex{-1.0f,-1.0f, 1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f,-1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{-1.0f,-1.0f,-1.0f, 0,1,0});

    //Topp
    mVertices.push_back(Vertex{ 1.0f, 1.0f, 1.0f, 1,1,0});
    mVertices.push_back(Vertex{ 1.0f, 1.0f,-1.0f, 1,1,0});
    mVertices.push_back(Vertex{-1.0f, 1.0f, 1.0f, 1,1,0});

    mVertices.push_back(Vertex{-1.0f, 1.0f, 1.0f, 0,1,0});
    mVertices.push_back(Vertex{ 1.0f, 1.0f,-1.0f, 0,1,0});
    mVertices.push_back(Vertex{-1.0f, 1.0f,-1.0f, 0,1,0});


    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
    mMatrix.setToIdentity();
}
