#include "lasmap.h"

lasmap::lasmap()
{

}

lasmap::lasmap(std::string filnavn)
{
    readFile(filnavn);
    mMatrix.setToIdentity();/*
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
    mScale.scale(sizeincrease,sizeincrease,sizeincrease);
    mMatrix  = mPosition * mRotation * mScale;*/
    //mMatrix.scale(sizeincrease,sizeincrease,sizeincrease);
    shaderType = 3;
}

lasmap::~lasmap()
{

}

void lasmap::init(GLint matrixUniform)
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

    glBindVertexArray(0);
}

void lasmap::init()
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

void lasmap::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

void lasmap::readFile(std::string filnavn)
{
    float x{0}, y{0}, z{0};
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open()) {
        int n;
        inn >> n;
        for (int i=0; i<n; i++) {
             inn >> x;
             inn >> y;
             inn >> z;

             Points.push_back(QVector3D (x,y,z));
        }
        inn >> n;
        inn.close();
    }

    float xMax = Points[0].x(), xMin = Points[0].x(), yMax = Points[0].y(), yMin = Points[0].y(), zMin = Points[0].z();
    for (int i = 0; i < Points.size(); i++){
        if (Points[i].x() < xMin){
            xMin = Points[i].x();
        }
        if (Points[i].x() > xMax){
            xMax = Points[i].x();
        }
        if (Points[i].y() < yMin){
            yMin = Points[i].y();
        }
        if (Points[i].y() > yMax){
            yMax = Points[i].y();
        }
        if (zMin > Points[i].z()){
            zMin = Points[i].z();
        }
    }

    const float ekvidistanse{1};

    for (float yy = 0; yy < yMax - yMin; yy += ekvidistanse){
        for (float xx = 0; xx < xMax - xMin; xx += ekvidistanse){
            lasth = 0;
            loop_counter = 1;
            Reduce_Points(xx, yy, zMin, ekvidistanse, xMin, yMin, xMax);
        }
    }
    int id{0};
    int square{1};
    int maxsquares{0};
    for (int i = 0; i < new_Points.size(); i++){
       if (new_Points[i].x() == 0){
            maxsquares++;
        }
    }
    int maxTriangles = (maxsquares - 1) * 2;
    const float doubleE = ekvidistanse + ekvidistanse + ekvidistanse;
    for (float dz = 0; (dz + ekvidistanse) < (yMax - yMin); dz += ekvidistanse){
        for (float dx = 0; (dx + ekvidistanse) < (xMax - xMin); dx += ekvidistanse){
            int A{0}, B{0}, C{0}, D{0};
            int NextC{0}, NextB{0};

            for (int i = 0; i < new_Points.size(); i++){
                if (new_Points[i].x() == dx && new_Points[i].z() == dz){
                    A = i;
                }else if (new_Points[i].x() == dx + ekvidistanse && new_Points[i].z() == dz){
                    B = i;
                }else if (new_Points[i].x() == dx && new_Points[i].z() == dz + ekvidistanse){
                    C = i;
                }else if (new_Points[i].x() == dx + ekvidistanse && new_Points[i].z() == dz + ekvidistanse){
                    D = i;
                }else if (new_Points[i].x() == dx && new_Points[i].z() == dz + doubleE){
                    NextC = i;
                }else if (new_Points[i].x() == dx + doubleE && new_Points[i].z() == dz){
                    NextB = i;
                }
            }
            switch (square){
            case 1:
                Surface.push_back({ B, A, C, -1, id + 1, -1});
                id++;
                Surface.push_back({ B, C, D, id + maxTriangles - 1, id + 1, id - 1});
                id++;
                square++;
                break;
            case 2:
                Surface.push_back({ B, A, C, id - 1, id + 1, -1});
                id++;
                Surface.push_back({ B, C, D, id + maxTriangles - 1, id + 1, id - 1});
                id++;
                if (NextB == 0){
                    square++;
                }
                break;
            case 3:
                Surface.push_back({ B, A, C, id - 1, id + 1, -1});
                id++;
                Surface.push_back({ B, C, D, id + maxTriangles - 1, -1, id - 1});
                id++;
                square++;
                break;
            case 4:
                Surface.push_back({ B, A, C,  -1, id + 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ B, C, D, id + maxTriangles - 1, id + 1, id - 1});
                id++;
                square++;
                break;
            case 5:
                Surface.push_back({ B, A, C, id - 1, id + 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ B, C, D, id + maxTriangles - 1, id + 1, id - 1});
                id++;
                if (NextB == 0){
                    square++;
                }
                break;
            case 6:
                Surface.push_back({ B, A, C, id - 1, id + 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ B, C, D, id + maxTriangles - 1, -1, id - 1});
                id++;
                if (NextC == 0){
                    square++;
                }else{
                    square -= 2;
                }
                break;
            case 7:
                Surface.push_back({ B, A, C, -1, id + 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ B, C, D, -1, id + 1, id - 1});
                id++;
                square++;
                break;
            case 8:
                Surface.push_back({ B, A, C, id - 1, id + 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ B, C, D, -1, id + 1, id - 1});
                id++;
                if (NextB == 0){
                    square++;
                }
                break;
            case 9:
                Surface.push_back({ B, A, C, id - 1, id + 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ B, C, D, -1, -1, id - 1});
                id++;
                square++;
                break;
            case 10:
                break;
            }
        }
    }
    for (int i = 0; i < Surface.size(); i++){
        int pA = Surface[i][0];
        int pB = Surface[i][1];
        int pC = Surface[i][2];

        mIndices.push_back(pA);
        mIndices.push_back(pB);
        mIndices.push_back(pC);
    }

    for (int i = 0; i < new_Points.size(); i++){
        float xsum{0};
        float zsum{0};
        int count{0};
        float y;

        for (int j = 0; j < mIndices.size(); j++){
            if (mIndices[j] == i){
                xsum += new_Points[i].x();
                zsum += new_Points[i].z();
                count++;
                y = new_Points[i].y();
            }
        }

        QVector3D rgb = QVector3D (xsum/count, y, zsum/count);
        rgb.normalize();

        Vertex r(new_Points[i].x(), new_Points[i].y(), new_Points[i].z(), rgb.x(), rgb.y(), rgb.z());
        mVertices.push_back(r);
    }
}

void lasmap::Reduce_Points(float x, float y, float zMin, float e, float xMin, float yMin, float xMax){
    int count{0};
    float sum{0};

    for (int i = 0; i < Points.size(); i++){
        if ((Points[i].x() - xMin >= x && Points[i].x() - xMin < x + e) && (Points[i].y() - yMin >= y && Points[i].y() - yMin < y + e)){
            sum += Points[i].z() - zMin;
            count++;
        }
    }

    lasth += sum/count;
    float h = lasth;
    if (h != h){
        h = 0;
    }
    new_Points.push_back(QVector3D(x, h, y));
}
