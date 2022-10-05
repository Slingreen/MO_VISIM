#include "trianglesurface.h"
#include <fstream>
#include <ostream>

TriangleSurface::TriangleSurface() : VisualObject()
{
//  //        x   y   z    r g b
//  Vertex v0{0.0,0.0,0.0, 1,0,0};    mVertices.push_back(v0);
//  Vertex v1(0.5,0.0,0.0, 0,1,0);    mVertices.push_back(v1);
//  Vertex v2{0.5,0.5,0.0, 0,0,1};    mVertices.push_back(v2);
//  Vertex v3{0.0,0.0,0.0, 0,0,1};    mVertices.push_back(v3);
//  Vertex v4{0.5,0.5,0.0, 0,1,0};    mVertices.push_back(v4);
//  Vertex v5{0.0,0.5,0.0, 1,0,0};    mVertices.push_back(v5);

  mMatrix.setToIdentity();
}


TriangleSurface::TriangleSurface(std::string filnavn) : VisualObject()
{
   readFileI(filnavn);
   mMatrix.setToIdentity();
   shaderType = 3;
}

TriangleSurface::~TriangleSurface()
{
    //writeFile("Utskrift.txt");
}

void TriangleSurface::readFile(std::string filnavn) {
   std::ifstream inn;
   inn.open(filnavn.c_str());

   if (inn.is_open()) {
       int n;
       Vertex vertex;
       inn >> n;
       //mVertices.reserve(n);
       for (int i=0; i<n; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);
       }
       inn.close();
   }
}

void TriangleSurface::readFileI(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open()) {
        int n;
        Vertex vertex;
        inn >> n;
        //mVertices.reserve(n);
        for (int i=0; i<n; i++) {
             inn >> vertex;
             mVertices.push_back(vertex);
        }
        inn >> n;
        GLuint indice;
        for (int i=0; i<n; i++) {
             inn >> indice;
             mIndices.push_back(indice);
        }
        inn >> n;
        inn.close();
    }
}


void TriangleSurface::writeFile(std::string filnavn) {
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open()) {
        n = mVertices.size();
        ut << n << std::endl;
        for (int i=0; i<n; i++) {
             ut << mVertices.at(i) << std::endl;
        }
        ut.close();
    }
    else
        std::cout << "En feil har skjedd!";
     ut.close();
}

void TriangleSurface::writeFileI(std::string filnavn) {
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open()) {
        n = mVertices.size();
        ut << n << std::endl;
        for (int i=0; i<n; i++) {
             ut << mVertices.at(i) << std::endl;
        }
        n = mIndices.size();
        ut << n << std::endl;
        for (int i=0; i<n; i++) {
             ut << mIndices.at(i) << std::endl;
        }
        ut.close();
    }
    else
        std::cout << "En feil har skjedd!";
     ut.close();
}


void TriangleSurface::init(GLint matrixUniform) {
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

void TriangleSurface::init()
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

//void TriangleSurface::draw()
//{
//    //What objects to draw
//    glBindVertexArray( mVAO );


//    glUniformMatrix4fv( mMatrixUniform,
//                        1,
//                        GL_FALSE,
//                        mMatrix.constData());

//    //Draw Call
//    glDrawArrays(GL_TRIANGLES,
//                 0,
//                 mVertices.size());
//    writeFile("Utskrift.txt"); // Vil helst ha den i deconstructor, men det vil ikke den
//}
void TriangleSurface::draw()
{
    initializeOpenGLFunctions();
        glBindVertexArray( mVAO );
        // GL_FALSE for QMatrix4x4
//        glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
        writeFileI("test.txt");
}

QVector3D TriangleSurface::PointOnTriangle(float x, float y, float z)
{
    QVector3D h{0,0,0};
    QVector3D origo{0,0,0}, X, P1, P2, P3, OX, P4, XP1, XP4; //kan være disse bare skal være vector2d?

    X.setX(x);
    X.setY(y);
    X.setZ(z);

    OX = X - origo;

    for (int i = 0; i < n - 5; i += 6){
        P1.setX(mVertices[i].GetX());
        P1.setY(mVertices[i].GetY());
        P1.setZ(mVertices[i].GetZ());
        XP1 = X - P1;

        P2.setX(mVertices[i + 1].GetX());
        P2.setY(mVertices[i + 1].GetY());
        P2.setZ(mVertices[i + 1].GetZ());

        P3.setX(mVertices[i + 2].GetX());
        P3.setY(mVertices[i + 2].GetY());
        P3.setZ(mVertices[i + 2].GetZ());

        P4.setX(mVertices[i + 5].GetX());
        P4.setY(mVertices[i + 5].GetY());
        P4.setZ(mVertices[i + 5].GetZ());
        XP4 = P4 - X;

        if(((x >= P1.x()) && (x <= P4.x())) && ((z >= P1.z()) && (z <= P4.z()))){

            QVector3D a, b, c, one, two;
            if (XP1.length() < XP4.length()){
                a = P1; //kan være P1 er 2D vector da må a sin y være 0
                b = P2;
                c = P3;

            }else {
                a.setX(mVertices[i + 3].GetX());
                a.setY(mVertices[i + 3].GetY());
                a.setZ(mVertices[i + 3].GetZ());

                b.setX(mVertices[i + 4].GetX());
                b.setY(mVertices[i + 4].GetY());
                b.setZ(mVertices[i + 4].GetZ());

                c = P4;
            }
            QVector3D pab = b-a;
            QVector3D pac = c-a;

            QVector3D n = QVector3D().crossProduct(pab,pac);
            float areal;
            areal = n.length();

            QVector3D bayc;

            one = b-X;
            two = c-X;
            n = n = QVector3D().crossProduct(one,two);
            bayc.setX(n.y()/areal);

            one = c-X;
            two = a-X;
            n = n = QVector3D().crossProduct(one,two);
            bayc.setY(n.y()/areal);

            one = a-X;
            two = b-X;
            n = n = QVector3D().crossProduct(one,two);
            bayc.setZ(n.y()/areal);

            h = bayc;

            break;
        }
    }

    return h;
}

int TriangleSurface::aIndices()
{
    return mIndices.size()/3;
}
