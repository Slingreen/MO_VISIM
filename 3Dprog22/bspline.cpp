#include "bspline.h"

bSpline::bSpline(VisualObject *b)
{
//    ball = b;
    int i = 0;
    while(i<d+1){
        t[i]=0;
        i++;
    }
    int j = 0;
    while(i<(n)){
        t[i]=j;
        i++;
        j++;
    }
    while(i<(n+1+d)){
        t[i]=j;
        i++;
    }
}

bSpline::~bSpline()
{

}

void bSpline::init()
{
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
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //drawElemet
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void bSpline::init(GLint matrixUniform)
{

}

void bSpline::draw()
{
    //What objects to draw
    glBindVertexArray( mVAO );


    glUniformMatrix4fv( mMatrixUniform,
                        1,
                        GL_FALSE,
                        mMatrix.constData());

    //Draw Call
    glDrawArrays(GL_LINES/*GL_LINE_STRIP*/,
                 0,
                 mVertices.size());
}

void bSpline::move(float dt)
{

}

int bSpline::findKnoteIntervall(float x)
{
    int my = n-1; //indeks til siste kontrollpunkt
    while ( x < t[my] && my>d ){
        my--;
    }
    return my;
}

QVector3D bSpline::evaluateBSpline(float x)
{
    int my = findKnoteIntervall(x);
    std::vector<QVector3D> a;
    a.reserve(d+1);
    for(int j =0; j<=d ; j++){
        a[d-j] = c[my-j];
    }
    for (int k=d ; k>0; k--){
        int j = my-k;
        for(int i =0; i<k; i++){
            j++;
            float w = (x-t[j])/(t[j+k] - t[j]);
            a[i] = a[i] * (1-w) + a[i+1] * w;
        }
    }
    return a [0];
}
