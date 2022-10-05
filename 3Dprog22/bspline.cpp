#include "bspline.h"

bSpline::bSpline(VisualObject *b)
{
    c.push_back(QVector3D(0, 1, 0));
    c.push_back(QVector3D(1, 0, 0));
    c.push_back(QVector3D(2, 1, 0));
    c.push_back(QVector3D(3, 0, 0));
    c.push_back(QVector3D(4, 1, 0));

    QVector3D a;
    // hvis t[] har t+1 like verdier i starten og slutten
    // får da siste 0 verdi posisjon lik d+1-1, siden array starter på 0
    // i likhet får første største verdi posisjon lik:
    // n+d+1-d-1=n
    for(float tx = t[d]; tx < t[n]; tx+= 0.1f)
    {
      a = evaluateBSpline(tx);
      mVertices.push_back(Vertex{a.x(),a.y(),a.z(),1-tx/t[n],tx/t[n],0,});
    }
}

bSpline::bSpline(std::vector<QVector3D> p)
{
    for(int i = 0; i < p.size(); i++){
        c.push_back(p[i]);
    }

    QVector3D a;
    // hvis t[] har t+1 like verdier i starten og slutten
    // får da siste 0 verdi posisjon lik d+1-1, siden array starter på 0
    // i likhet får første største verdi posisjon lik:
    // n+d+1-d-1=n
    for(float tx = t[d]; tx < t[n]; tx+= 0.1f)
    {
      a = evaluateBSpline(tx);
      mVertices.push_back(Vertex{a.x(),a.y(),a.z(),1-tx/t[n],tx/t[n],0,});
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
    glDrawArrays(GL_LINE_STRIP/*GL_LINE_STRIP*/,
                 0,
                 mVertices.size());
}

void bSpline::move(float dt)
{
//    if (dt>=ti){
//        c.push_back(ball->getPosition3D());
//        ti+=dt;
//    }
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
    int dt = d;
    std::vector<QVector3D> a;
//    a.reserve(dt+1);
    for(int k = 0; k<dt+1;k++){
        a.push_back(QVector3D{0.f,0.f,0.f});
    }
    for(int j =0; j<=dt; j++){
        a[dt-j] = c[my-j];
    }
    for (int k=dt ; k>0; k--){
        int j = my-k;
        for(int i =0; i<k; i++){
            j++;
            float w = (x-t[j])/(t[j+k] - t[j]);
            a[i] = a[i] * (1-w) + a[i+1] * w;
        }
    }
    return a [0];
}
