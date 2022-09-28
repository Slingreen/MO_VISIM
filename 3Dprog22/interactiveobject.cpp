#include "interactiveobject.h"

InteractiveObject::InteractiveObject() : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    //Forside
//    mVertices.push_back(Vertex{ 0.00f, 0.00f, 0.00f,1,1,0});
//    mVertices.push_back(Vertex{ 0.25f, 0.00f, 0.00f,1,1,0});
//    mVertices.push_back(Vertex{ 0.00f, 0.25f, 0.00f,1,1,0});
    mPosition.translate(0,0,0);

}

InteractiveObject::~InteractiveObject()
{
    //writeFile("Utskrift.txt");
}

void InteractiveObject::init(GLint matrixUniform) {
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

void InteractiveObject::draw()
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

void InteractiveObject::move(float x, float y, float z)
{
//    mx += x;
//    my += y;
//    mz += z;
//    mMatrix.translate(mx,my,mz);
    mMatrix.translate(x,y,z);
}

void InteractiveObject::collision()
{

}
