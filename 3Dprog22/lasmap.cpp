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
//    glDrawElements(GL_POINTS, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0)); //GL_POINTS, GL_TRIANGLES
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

             vertex = Vertex(x - 659428, z - 59, y - 7153117, 1, 1, 1, 0, 1);

             mVertices.push_back(vertex);
        }
        inn >> n;
        inn.close();
    }/*

    float xMax = Points[0].x(), xMin= Points[0].x(), yMax= Points[0].y(), yMin = Points[0].y();
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
    }

    float ekvidistanse{5};
    float t = xMax - xMin;
    if ((int)t % (int)ekvidistanse != 0){
        int sav = (int)t % (int)ekvidistanse;
        sav += 1;
        t = sav * ekvidistanse;
        xMax = t + xMin;
    }
    t = yMax - yMin;
    if ((int)t % (int)ekvidistanse != 0){
        int sav = (int)t % (int)ekvidistanse;
        sav += 1;
        t = sav * ekvidistanse;
        yMax = t + yMin;
    }
    int index{0};
    for (auto xx = xMin; xx < xMax; xx += ekvidistanse){
        for (auto yy = yMin; yy < yMax; yy += ekvidistanse){
            int count{0}; //for the avrage
            float sum{0}; //for the avrage
            for (int i = 0; i < Points.size(); i++){
                if (Points[i].x() == xx && Points[i].y() == yy){
                    new_Points.push_back(QVector3D((xx - xMin), Points[i].z(), (yy - yMin)));
                }
                if (Points[i].x() == xx + ekvidistanse && Points[i].y() == yy){
                    new_Points.push_back(QVector3D(((xx + ekvidistanse) - xMin), Points[i].z(), (yy - yMin)));
                }
                if (Points[i].x() == xx && Points[i].y() == yy + ekvidistanse){
                    new_Points.push_back(QVector3D((xx - xMin), Points[i].z(), ((yy + ekvidistanse) - yMin)));
                }
                if (Points[i].x() == xx + ekvidistanse && Points[i].y() == yy + ekvidistanse){
                    new_Points.push_back(QVector3D(((xx + ekvidistanse) - xMin), Points[i].z(), ((yy + ekvidistanse) - yMin)));
                }
                if ((Points[i].x() >= xx && Points[i].x() <= xx + ekvidistanse) && (Points[i].y() >= yy && Points[i].y() <= yy + ekvidistanse)){
                    count++;
                    sum += Points[i].z();
                }
            }
            new_Points.push_back(QVector3D(((xx + ekvidistanse) - xMin)/2, sum/count, ((yy + ekvidistanse) - yMin)/2));

            //std::vector <std::vector <int>> temp;
            int b{0}, a{0}, c{0}, d{0}, id{0}, A{0}, B{0}, C{0}, D{0};
            b = (xMax/ekvidistanse) * 4;
            d = b;
            //for (int i = 0; i < new_Points.size(); i += 5){
                if (xx == xMin && yy == yMin){
                    //for (int j = 0; j < 4; j++){
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, -1});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, a});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, b});
                    id++;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, -1});
                    id++;
                    //}
                }
                else if ((xx > xMin && xx + ekvidistanse < xMax) && yy == yMin){
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, -1});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, a});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, b});
                    id++;
                    c = id - 6;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, c});
                    id++;
                }
                else if(xx + ekvidistanse == xMax && yy == yMin){
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, -1});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, -1});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, b});
                    id++;
                    c = id - 6;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, c});
                    id++;
                }
                else if(xx == xMin && (yy > yMin && yy + ekvidistanse < yMax)){
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    d -= A;
                    d += 2;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, d});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, a});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, b});
                    id++;
                    c = id - 6;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, -1});
                    id++;
                }
                else if(xx + ekvidistanse == xMax && (yy > yMin && yy + ekvidistanse < yMax)){
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    d -= A;
                    d += 2;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, d});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, -1});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, b});
                    id++;
                    c = id - 6;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, c});
                    id++;
                }
                else if(xx == xMin && yy + ekvidistanse == yMax){
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    d -= A;
                    d += 2;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, d});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, a});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, -1});
                    id++;
                    c = id - 6;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, -1});
                    id++;
                }
                else if((xx > xMin && xx + ekvidistanse < xMax) && yy + ekvidistanse == yMax){
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    d -= A;
                    d += 2;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, d});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, a});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, -1});
                    id++;
                    c = id - 6;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, c});
                    id++;
                }
                else if (xx + ekvidistanse == xMax && yy + ekvidistanse == yMax){
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    d -= A;
                    d += 2;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, d});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, -1});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, -1});
                    id++;
                    c = id - 6;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, c});
                    id++;
                }
                else {
                    A = id;
                    B = A + 1;
                    C = B + 1;
                    D = C + 1;
                    d -= A;
                    d += 2;
                    Surface.push_back({id, index, index + 1, index + 4, A, D, d});
                    b += id;
                    id++;
                    a = id + 6;
                    Surface.push_back({id, index + 1, index + 2, index + 4, C, A, a});
                    id++;
                    Surface.push_back({id, index + 2, index + 3, index + 4, D, B, b});
                    id++;
                    c = id - 6;
                    Surface.push_back({id, index + 3, index, index + 4, A, C, c});
                    id++;
                }
                index++;
            //}
        }
    }/*
    float k = new_Points.size();
    float g = Surface.size();
    for (int i = 0; i < new_Points.size(); i++){
        Vertex p(new_Points[i].x(), new_Points[i].y(), new_Points[i].z(), 1, 1, 1, 1, 0);
        mVertices.push_back(p);
    }
    /*for (int i = 0; i < Surface.size(); i++){
        float ix = Surface[i][1];
        float iy = Surface[i][2];
        float iz = Surface[i][3];

        Vertex p(new_Points[ix].x(), new_Points[ix].y(), new_Points[ix].z(), 1, 1, 1, 1, 0);
        Vertex p1(new_Points[iy].x(), new_Points[iy].y(), new_Points[iy].z(), 1, 1, 1, 1, 0);
        Vertex p2(new_Points[iz].x(), new_Points[iz].y(), new_Points[iz].z(), 1, 1, 1, 1, 0);

        mVertices.push_back(p);
        mVertices.push_back(p1);
        mVertices.push_back(p2);
    }*/
}
