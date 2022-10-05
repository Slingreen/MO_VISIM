#include "lasmap.h"

lasmap::lasmap()
{

}

lasmap::lasmap(std::string filnavn)
{
    readFile(filnavn);
    mMatrix.setToIdentity();
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

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void lasmap::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    // GL_FALSE for QMatrix4x4
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

    /*for (int i = 0; i < Surface.size(); i++){
        int A = Surface[i][0];
        int B = Surface[i][1];
        int C = Surface[i][2];

        mIndices.push_back(A);
        mIndices.push_back(B);
        mIndices.push_back(C);
    }

    glDrawElements(GL_POINTS, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0)); //GL_POINTS, GL_TRIANGLES
    /**/
    glDrawArrays(GL_POINTS, 0, mVertices.size());
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

    const int ekvidistanse{1};

    float last_height{0};
    int loopnr{1};
    for (float yy = yMin; yy < yMax; yy += ekvidistanse){
        for (float xx = xMin; xx < xMax; xx += ekvidistanse){
            Reduce_Points(xx, yy, zMin, ekvidistanse, &last_height, xMin, yMin, &loopnr, xMax);
        }
    }
/*
    for (int i = 0; i < new_Points.size(); i++){
        Vertex r(new_Points[i].x(), new_Points[i].y(), new_Points[i].z(), 1, 1, 1, 0, 1);
        mVertices.push_back(r);
    }
    writeFile("newPoints.txt");/**/

    int l = Points.size();
    int k = new_Points.size();
    int id{0};
    int square{1};
    int maxsquares{1};
    int maxrows{1};
    for (int i = 0; i < new_Points.size(); i++){
        if (new_Points[i].x() == 0){
            maxsquares++;
        }
        if (new_Points[i].z() == 0){
            maxrows++;
        }
    }
    int maxTriangles = maxsquares * 2;
    int row{1};
    for (int dz = 0; dz < yMax - yMin; dz += ekvidistanse){
        for (int dx = 0; dx < xMax - xMin; dx += ekvidistanse){
            int A{0}, B{0}, C{0}, D{0};

            for (int i = 0; i < new_Points.size(); i++){
                int ax = new_Points[i].x();
                int bx = new_Points[i].z();

                if (new_Points[i].x() == dx && new_Points[i].z() == dz){//dosen't trigger?
                    A = i;
                }else if (new_Points[i].x() == dx + ekvidistanse && new_Points[i].z() == dz){//dosen't trigger?
                    B = i;
                }else if (new_Points[i].x() == dx && new_Points[i].z() == dz + ekvidistanse){//dosen't trigger?
                    C = i;
                }else if (new_Points[i].x() == dx + ekvidistanse && new_Points[i].z() == dz + ekvidistanse){//dosen't trigger?
                    D = i;
                }
            }

            switch (square){
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
                if (((maxTriangles * row) - 1) - id == 2){
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
                if (((maxTriangles * row) - 1) - id == 2){
                    square++;
                }
                break;
            case 6:
                Surface.push_back({ A, B, C, id + 1, id - 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ C, B, D, -1, id + maxTriangles - 1, id - 1});
                id++;
                if (row == maxrows - 1){
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
                if (((maxTriangles * row) - 1) - id == 2){
                    square++;
                }
                break;
            case 9:
                Surface.push_back({ A, B, C, id + 1, id - 1, id - maxTriangles - 1});
                id++;
                Surface.push_back({ C, B, D, -1, -1, id - 1});
                id++;
                break;
            }
        }
        row++;
    }
    float g = Surface.size();

    for (int i = 0; i < Surface.size(); i++){
        int A = Surface[i][0];
        int B = Surface[i][1];
        int C = Surface[i][2];

        Vertex p(new_Points[A].x(), new_Points[A].y(), new_Points[A].z(), 1, 1, 1, 0, 1);
        Vertex p1(new_Points[B].x(), new_Points[B].y(), new_Points[B].z(), 1, 1, 1, 0, 1);
        Vertex p2(new_Points[C].x(), new_Points[C].y(), new_Points[C].z(), 1, 1, 1, 0, 1);

        mVertices.push_back(p);
        mVertices.push_back(p1);
        mVertices.push_back(p2);
    }/**/
}

void lasmap::Reduce_Points(float x, float y, float zMin, int e, float* lasth, float xMin, float yMin, int* loop, float xMax){
    int count{0};
    float sum{0};

    for (int i = 0; i < Points.size(); i++){
        if ((Points[i].x() >= x && Points[i].x() < x + e) && (Points[i].y() >= y && Points[i].y() < y + e)){
            sum += Points[i].z() - zMin;
            count++;
        }
    }
    if (count >= 1){
        *lasth += sum/count;
        int read_loop = *loop;
        new_Points.push_back(QVector3D(x - xMin, *lasth/read_loop, y - yMin)); //fix for y values
        *lasth = 0;
        *loop = 1;
    }else if (!(x + e >= xMax)){
        Reduce_Points(x + e, y + e, zMin, e, lasth, xMin, yMin, loop++, xMax); //fix y values
    }/*else{
        new_Points.push_back(QVector3D(x - xMin, 0, y - yMin)); //fix y values
    }*/
}
