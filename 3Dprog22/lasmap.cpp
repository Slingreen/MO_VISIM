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
        // GL_FALSE for QMatrix4x4
//        glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
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
        Vertex vertex;
        inn >> n;
        for (int i=0; i<n; i++) {
             inn >> x;
             inn >> y;
             inn >> z;

             Points.push_back(QVector3D (x,y,z));

             //vertex = Vertex(x - 659428, z - 59, y - 7153117, 1, 1, 1, 0, 1);

             //mVertices.push_back(vertex);
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
/*
    writeFile("newPoints.txt");/**/

//    int l = Points.size();
//    int k = new_Points.size();
//    int id{0};
//    int square{1};
//    int maxsquares{0};
//    int maxrows{0};
//    for (int i = 0; i < new_Points.size(); i++){
//        if (new_Points[i].x() == 0){
//            maxsquares++;
//        }
//        if (new_Points[i].z() == 0){
//            maxrows++;
//        }
//    }
//    int maxTriangles = (maxsquares - 1) * 2;
//    maxrows--;
//    int row{1};
//    const int doubleE = ekvidistanse + ekvidistanse + ekvidistanse;
    for (float dz = 0; (dz + ekvidistanse) < (yMax - yMin); dz += ekvidistanse){
        for (float dx = 0; (dx + ekvidistanse) < (xMax - xMin); dx += ekvidistanse){
            int A{0}, B{0}, C{0}, D{0};
            //int NextC{0}, NextB{0};

            for (int i = 0; i < new_Points.size(); i++){
//                int ax = new_Points[i].x();
//                int bx = new_Points[i].z();
/*
                if ((new_Points[i].x() >= dx - (ekvidistanse/2) && new_Points[i].x() <= dx + (ekvidistanse/2)) && ((new_Points[i].z() >= dz - (ekvidistanse/2) && new_Points[i].z() <= dz + (ekvidistanse/2)))){//dosen't trigger?
                    A = i;
                }else if ((new_Points[i].x() >= dx - (ekvidistanse/2) + ekvidistanse && new_Points[i].x() <= dx + (ekvidistanse/2) + ekvidistanse) && ((new_Points[i].z() >= dz - (ekvidistanse/2) && new_Points[i].z() <= dz + (ekvidistanse/2)))){//dosen't trigger?
                    B = i;
                }else if ((new_Points[i].x() >= dx - (ekvidistanse/2) && new_Points[i].x() <= dx + (ekvidistanse/2)) && ((new_Points[i].z() >= dz - (ekvidistanse/2) + ekvidistanse && new_Points[i].z() <= dz + (ekvidistanse/2) + ekvidistanse))){//dosen't trigger?
                    C = i;
                }else if ((new_Points[i].x() >= dx - (ekvidistanse/2) + ekvidistanse && new_Points[i].x() <= dx + (ekvidistanse/2) + ekvidistanse) && ((new_Points[i].z() >= dz - (ekvidistanse/2) + ekvidistanse && new_Points[i].z() <= dz + (ekvidistanse/2) + ekvidistanse))){//dosen't trigger?
                    D = i;
                }*/

                if (new_Points[i].x() == dx && new_Points[i].z() == dz){//dosen't trigger? triggers randomly
                    A = i;
                }else if (new_Points[i].x() == dx + ekvidistanse && new_Points[i].z() == dz){//dosen't trigger? triggers randomly
                    B = i;
                }else if (new_Points[i].x() == dx && new_Points[i].z() == dz + ekvidistanse){//dosen't trigger? triggers randomly
                    C = i;
                }else if (new_Points[i].x() == dx + ekvidistanse && new_Points[i].z() == dz + ekvidistanse){//dosen't trigger? triggers randomly
                    D = i;
                }/*else if (new_Points[i].x() == dx && new_Points[i].z() == dz + doubleE){//dosen't trigger? triggers randomly
                    NextC = i;
                }else if (new_Points[i].x() == dx + doubleE && new_Points[i].z() == dz){//dosen't trigger? triggers randomly
                    NextB = i;
                }*/
            }


            float g = Surface.size();


            mIndices.push_back(B);
            mIndices.push_back(A);
            mIndices.push_back(C);


            mIndices.push_back(B);
            mIndices.push_back(C);
            mIndices.push_back(D);

            /*switch (square){
            case 1:
                Surface.push_back({ A, B, C, id + 1, -1, -1});
                id++;
                Surface.push_back({ C, B, D, id + 1, id + maxTriangles - 1, id - 1});
                id++;
                square++;
                break;
            case 2:
                Surface.push_back({ A, B, C, id + 1, id - 1, -1});
                id++;
                Surface.push_back({ C, B, D, id + 1, id + maxTriangles - 1, id - 1});
                id++;
                if (NextB == 0){
                    square++;
                }
                break;
            case 3:
                Surface.push_back({ A, B, C, id + 1, id - 1, -1});
                id++;
                Surface.push_back({ C, B, D, -1, id + maxTriangles - 1, id - 1});
                id++;
                square++;
                break;
            case 4:
                Surface.push_back({ A, B, C, id + 1, -1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ C, B, D, id + 1, id + maxTriangles - 1, id - 1});
                id++;
                square++;
                break;
            case 5:
                Surface.push_back({ A, B, C, id + 1, id - 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ C, B, D, id + 1, id + maxTriangles - 1, id - 1});
                id++;
                if (NextB == 0){
                    square++;
                }
                break;
            case 6:
                Surface.push_back({ A, B, C, id + 1, id - 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ C, B, D, -1, id + maxTriangles - 1, id - 1});
                id++;
                if (NextC == 0){
                    square++;
                }else{
                    square -= 2;
                }
                break;
            case 7:
                Surface.push_back({ A, B, C, id + 1, -1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ C, B, D, id + 1, -1, id - 1});
                id++;
                square++;
                break;
            case 8:
                Surface.push_back({ A, B, C, id + 1, id - 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ C, B, D, id + 1, -1, id - 1});
                id++;
                if (NextB == 0){
                    square++;
                }
                break;
            case 9:
                Surface.push_back({ A, B, C, id + 1, id - 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ C, B, D, -1, -1, id - 1});
                id++;
                square++;
                break;
            case 10:
                break;
            }*/
        }
        //row++;
    }
//    float g = Surface.size();

    for (int i = 0; i < new_Points.size(); i++){
        float xsum{0};
        float zsum{0};
        int count{0};
        float y;

        for (int j = 0; j < mIndices.size(); j++){
            if (mIndices[j] == i || mIndices[j] == i || mIndices[j] == i){
                xsum += new_Points[i].x();
                zsum += new_Points[i].z();
                count++;
                y = new_Points[i].y();
            }
        }

        QVector3D rgb = QVector3D (xsum/count, y, zsum/count);
        rgb.normalize();/**/

        Vertex r(new_Points[i].x(), new_Points[i].y(), new_Points[i].z(), rgb.x(), rgb.y(), rgb.z());
        //Vertex r(new_Points[i].x(), new_Points[i].y(), new_Points[i].z(), 1, 1, 1);
        mVertices.push_back(r);
    }


    /*for (int i = 0; i < Surface.size(); i++){
        int pA = Surface[i][0];
        int pB = Surface[i][1];
        int pC = Surface[i][2];
/*
        Vertex p(new_Points[A].x(), new_Points[A].y(), new_Points[A].z(), 1, 1, 1, 0, 1);
        Vertex p1(new_Points[B].x(), new_Points[B].y(), new_Points[B].z(), 1, 1, 1, 0, 1);
        Vertex p2(new_Points[C].x(), new_Points[C].y(), new_Points[C].z(), 1, 1, 1, 0, 1);

        mVertices[A] = p;
        mVertices[B] = p1;
        mVertices[C] = p2;
*/
        //mIndices.push_back(pA);
        //mIndices.push_back(pB);
        //mIndices.push_back(pC);
    //}/**/
    //writeFile("newPoints.txt");/**/
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

    int l = new_Points.size();
    lasth += sum/count;
    float h = lasth;
    if (h != h){
        h = 0;
    }
    new_Points.push_back(QVector3D(x, h, y));

    /*if (h != 0){
        float xx = x;
        float yy = y;
        new_Points.push_back(QVector3D(xx, h, yy)); //fix for y values
        if (x + e >= xMax){
            new_Points.push_back(QVector3D(xx + e, h, yy));
        }
    }//else if(count >= 1 && *loop )

    if (!(x + e >= xMax - xMin)){
        Reduce_Points(x + e, y, zMin, e, xMin, yMin, xMax); //fix y values
    }else{
        QVector2D PrevPoint = QVector2D (Points[0].x(), Points[0].y());
        QVector2D Point = QVector2D (x, y);
        int g{0};
        for (int i = 0; i < Points.size(); i++){
            QVector2D p = QVector2D (Points[i].x(), Points[i].y());
            if (Point.length() - PrevPoint.length() > Point.length() - p.length()){
                PrevPoint = p;
                g = i;
            }
        }
        new_Points.push_back(QVector3D(x, Points[g].z() - zMin, y)); //fix y values
    }*/
}
