#include "pointcloud.h"

Pointcloud::Pointcloud()
{

}

Pointcloud::Pointcloud(std::string filanvn)
{
    readFile(filanvn);
    mMatrix.setToIdentity();
    mMatrix.scale(sizeincrease,sizeincrease,sizeincrease);
}

Pointcloud::~Pointcloud()
{

}

void Pointcloud::init(GLint shader)
{
    mMatrixUniform = shader;

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

void Pointcloud::init()
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

void Pointcloud::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

    glDrawArrays(GL_POINTS, 0, mVertices.size());
}

void Pointcloud::readFile(std::string filnavn)
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

    for (int i = 0; i < Points.size(); i++){
        Vertex r(Points[i].x() - xMin, Points[i].z() - zMin, Points[i].y() - yMin, 1, 1, 1, 0, 1);
        mVertices.push_back(r);
    }
}
