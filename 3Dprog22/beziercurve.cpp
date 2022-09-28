#include "beziercurve.h"

BezierCurve::BezierCurve()
{
    c[0]= {-1, -1, 0};
    c[1]= {2, 2, 0};
    c[2]= {2, 0, 0};
    c[3]= {0, 2, 0};
//    mVertices.push_back(Vertex{c[0].x(),c[0].y(),c[0].z(),1,0,0});
//    mVertices.push_back(Vertex{c[1].x(),c[1].y(),c[1].z(),1,0,0});
//    mVertices.push_back(Vertex{c[2].x(),c[2].y(),c[2].z(),0,0,1});
//    mVertices.push_back(Vertex{c[3].x(),c[3].y(),c[3].z(),0,0,1});
    for (int i = 0; i<d; i++)
    {
        mVertices.push_back(Vertex{c[i].x(),c[i].y(),c[i].z(),0,1,0});
        mVertices.push_back(Vertex{c[i+1].x(),c[i+1].y(),c[i+1].z(),0,1,0});
    }

    QVector3D a;
    for(float t = 0; t < 1; t+= 0.1)
    {
      a = evaluateBezier(t);
      mVertices.push_back(Vertex{a.x(),a.y(),0,1,1,1});
      a = evaluateBezier(t+0.1);
      mVertices.push_back(Vertex{a.x(),a.y(),0,1,1,1});
    }
    mMatrix.setToIdentity();
}

BezierCurve::~BezierCurve()
{

}

void BezierCurve::init(GLint matrixUniform)
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

void BezierCurve::draw()
{
    //What objects to draw
    glBindVertexArray( mVAO );


    glUniformMatrix4fv( mMatrixUniform,
                        1,
                        GL_FALSE,
                        mMatrix.constData());

    //Draw Call
    glDrawArrays(GL_LINES,
                 0,
                 mVertices.size());
}

QVector3D BezierCurve::evaluateBezier(float t)
{
    QVector3D a[4]; //4 = d+1
    for (int i = 0; i < d+1; i++)
        a[i] = c[i];

    for (int k=d; k>0; k--)
    {
        for (int i = 0; i<k; i++)
             a[i] = a[i] * (1-t) + a[i+1] * t;
    }
    return a[0];
}
