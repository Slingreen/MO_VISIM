#include <string>
#include <vector>

#include "underlag.h"
#include "math.h"

QVector3D Underlag::NormalMath(QVector3D A, QVector3D B, QVector3D C)
{
    QVector3D AB = B-A;
    QVector3D AC = C-A;
    return QVector3D().crossProduct(AB,AC);
}

void Underlag::findPoints(const float x, const float z)
{
    QVector3D a, b, c, one, two;
    QVector3D loc(x,0,z);

    int i = index;

    float areal;
    if (inner){
        a.setX(mVertices[i].GetX());
        a.setZ(mVertices[i].GetZ());
        a.setY(0);
        b.setX(mVertices[i + 1].GetX());
        b.setZ(mVertices[i + 1].GetZ());
        b.setY(0);
        c.setX(mVertices[i + 2].GetX());
        c.setZ(mVertices[i + 2].GetZ());
        c.setY(0);
        P = a;
        Q = b;
        R = c;
        P.setY(mVertices[i].GetY());
        Q.setY(mVertices[i + 1].GetY());
        R.setY(mVertices[i + 2].GetY());
    }else {
        a.setX(mVertices[i].GetX());
        a.setZ(mVertices[i].GetZ());
        a.setY(0);
        b.setX(mVertices[i - 2].GetX());
        b.setZ(mVertices[i - 2].GetZ());
        b.setY(0);
        c.setX(mVertices[i - 1].GetX());
        c.setZ(mVertices[i - 1].GetZ());
        c.setY(0);
        P = a;
        Q = b;
        R = c;
        P.setY(mVertices[i].GetY());
        Q.setY(mVertices[i - 2].GetY());
        R.setY(mVertices[i - 1].GetY());
    }

        QVector3D pab = b-a;
        QVector3D pac = c-a;

        QVector3D n = QVector3D().crossProduct(pab,pac);
        areal = n.length();

        QVector3D bayc;
        //u
        one = b - loc;
        two = c - loc;
        n = QVector3D().crossProduct(one, two);
        bayc.setX(n.y()/areal);

        //v
        one = c - loc;
        two = a - loc;
        n = QVector3D().crossProduct(one, two);
        bayc.setY(n.y()/areal);

        //w
        one = a - loc;
        two = b - loc;
        n = QVector3D().crossProduct(one, two);
        bayc.setZ(n.y()/areal);

        u = bayc.x();
        v = bayc.y();
        w = bayc.z();
}

void Underlag::findTriangle(float x, float z)
{
    //int j;
    QVector2D origo{0,0}, X, P1, P2, P3, OX, P4, OP1, OP4;

    X.setX(x);
    X.setY(z);

    OX = X - origo;

    //float remember = maxx * maxz;

    for (int j = 0; j < ((maxx - 1) * (maxz - 1))*6 - 5; j += 6){
        P1.setX(mVertices[j].GetX());
        P1.setY(mVertices[j].GetZ()); //bottom left (x,y)
        OP1 = X - P1;
        //OP1 = P1 - origo;
        P3.setX(mVertices[j + 1].GetX());
        P3.setY(mVertices[j + 1].GetZ()); //bottom right (x+1,y)

        P2.setX(mVertices[j + 2].GetX());
        P2.setY(mVertices[j + 2].GetZ()); //top left (x,y+1)

        P4.setX(mVertices[j + 5].GetX());
        P4.setY(mVertices[j + 5].GetZ()); //top right (x+1,y+1)
        OP4 = P4 - X;
        //OP4 = P4 - origo;

        if (((x >= P1.x()) && (x <= P4.x())) && ((z >= P1.y()) && (z <= P4.y()))){
            if (OP1.length() < OP4.length()){
                index = j;
                inner = true;
            }else {
                index = j + 5;
                inner = false;
            }
            break;
        }else {
            index = -1;
        }
    }
}

Underlag::Underlag()
{
    shaderType=2;
    image = new bitmap_image(fil);
    float y[3];
        rgb_t colour;
        for (int x = 0; x < maxx - 1; x++){
            for (int z = 0; z < maxz - 1; z++){
                image->get_pixel(x, z, colour);
                y[0] = (colour.red + colour.green + colour.blue)/ 3.0f;
                image->get_pixel(x + 1, z, colour);
                y[1] = (colour.red + colour.green + colour.blue)/ 3.0f;
                image->get_pixel(x, z + 1, colour);
                y[2] = (colour.red + colour.green + colour.blue)/ 3.0f;
                tempNormal = NormalMath(
                                        QVector3D(x*l, y[0]*h, z*l),
                                        QVector3D(x*l, y[2]*h, (z + 1)*l),
                                        QVector3D((x+1)*l, y[1]*h, z*l)
                                        );
                tempNormal.normalize();
                mVertices.push_back(Vertex{x*l, y[0]*h, z*l, tempNormal.x(),tempNormal.y(),tempNormal.z()});

                mVertices.push_back(Vertex{(x + 1)*l, y[1]*h, z*l, tempNormal.x(),tempNormal.y(),tempNormal.z()});

                mVertices.push_back(Vertex{x*l, y[2]*h, (z + 1)*l, tempNormal.x(),tempNormal.y(),tempNormal.z()});

                image->get_pixel(x + 1, z, colour);
                y[0] = (colour.red + colour.green + colour.blue)/ 3.0f;
                image->get_pixel(x, z + 1, colour);
                y[1] = (colour.red + colour.green + colour.blue)/ 3.0f;
                image->get_pixel(x + 1, z + 1, colour);
                y[2] = (colour.red + colour.green + colour.blue)/ 3.0f;
                tempNormal = NormalMath(
                                        QVector3D((x + 1)*l, y[2]*h, (z + 1)*l),
                                        QVector3D((x+1)*l, y[0]*h, z*l),
                                        QVector3D(x*l, y[1]*h, (z + 1)*l)


                                        );
                tempNormal.normalize();
                mVertices.push_back(Vertex{(x+1)*l, y[0]*h, z*l, tempNormal.x(),tempNormal.y(),tempNormal.z()});
                mVertices.push_back(Vertex{x*l, y[1]*h, (z + 1)*l, tempNormal.x(),tempNormal.y(),tempNormal.z()});
                mVertices.push_back(Vertex{(x + 1)*l, y[2]*h, (z + 1)*l, tempNormal.x(),tempNormal.y(),tempNormal.z()});

            }
        }
    mPosition.setToIdentity();
}

Underlag::~Underlag(){

}

void Underlag::init(GLint matrixUniform){

}

void Underlag::draw(){
    glBindVertexArray( mVAO );
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size()); //GL_LINES, GL_TRIANGLES
    glBindVertexArray(0);
}

float Underlag::BeregnY(float x, float z){
    float y;
    if (!(x <= maxx && x >= 0) || !(z <= maxz && z >= 0)){
        return 0;
    }
    findTriangle(x, z);
    if (index < 0){
        return 0;
    }
    findPoints(x, z);
    y = u * P.y() + v * Q.y() + w * R.y();
    if (inner){
        return y * (-1);
    }else {
        return y;
    }
}

void Underlag::init()
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
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof( Vertex ),
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);
    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}


// <3
