#include "rollingball.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
    mMatrix.setToIdentity();
    mPosition.translate(p);
    mScale.scale(r,r,r);
    mMatrix = mPosition * mRotation * mScale;

}

RollingBall::RollingBall(int n, VisualObject *surface): OctahedronBall (n)
{
    triangle_surface = surface;
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
    mMatrix.setToIdentity();
    mPosition.translate(p);
    mScale.scale(r,r,r);
    mMatrix = mPosition * mRotation * mScale;

}

RollingBall::RollingBall(int n, float x, float y, float z, VisualObject *surface): OctahedronBall (n)
{
    triangle_surface = surface;
    p = Vec3(x,y,z);
    y = oy = p.y();
    mPosition.setToIdentity();
    mRotation.setToIdentity();
    mScale.setToIdentity();
    mMatrix.setToIdentity();
    mPosition.translate(p);
    mScale.scale(r,r,r);
    mMatrix = mPosition * mRotation * mScale;
    shaderType = 0;
}

RollingBall::~RollingBall()
{

}
void RollingBall::move(float dt)
{
    //std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    onTriangle = false;
    mMatrix = mPosition * mScale;

    // Finne trekant
    int n = triangle_surface->aIndices();
    // for i antall trekanter
    for(int i=0; i<n; i++)
    {
        // Finn trekantens vertices v0, v1, v2
        Vertex v0 = triangle_surface->getVertex(i*3);
        Vertex v1 = triangle_surface->getVertex(i*3+1);
        Vertex v2 = triangle_surface->getVertex(i*3+2);
        // Finn ballens posisjon i xy=planet
        p = {mMatrix.operator()(0,3), mMatrix.operator()(1,3), mMatrix.operator()(2,3)};
        // Soek etter triangel som ballen er pa na
        // med barysentriske koordinater
        QVector3D a, b, c, one, two;
        float u, v, w;

        a.setX(v0.GetX());
        a.setY(0);
        a.setZ(v0.GetZ());

        b.setX(v1.GetX());
        b.setY(0);
        b.setZ(v1.GetZ());

        c.setX(v2.GetX());
        c.setY(0);
        c.setZ(v2.GetZ());

        QVector3D pab = b - a;
        QVector3D pac = c - a;

        QVector3D n = QVector3D().crossProduct(pab, pac);
        float areal;
        areal = n.length();

        QVector3D bayc;

        one = b - p;
        two = c - p;
        n = QVector3D().crossProduct(one,two);
        bayc.setX(n.y()/areal);

        one = c - p;
        two = a - p;
        n = QVector3D().crossProduct(one,two);
        bayc.setY(n.y()/areal);

        one = a - p;
        two = b - p;
        n  = QVector3D().crossProduct(one,two);
        bayc.setZ(n.y()/areal);

        u = bayc.x();
        v = bayc.y();
        w = bayc.z();

        //høyde y blir da y = u * v0.GetY() + v * v1.GetY() + w * v2.GetY()
        /*barysentriskekoordinater mellom 0 og 1 */
        if ((0<=u && u<=1) && (0<=v && v<=1) && (0<=w && w<=1))
        {
            onTriangle = true;
            // beregne normal
            Vec3 vec0{v0.GetX(),v0.GetY(),v0.GetZ()};
            Vec3 vec1{v1.GetX(),v1.GetY(),v1.GetZ()};
            Vec3 vec2{v2.GetX(),v2.GetY(),v2.GetZ()};
            Vec3 n = QVector3D().crossProduct(vec1-vec0,vec2-vec0);
            n.normalize();
            y = (u * v0.GetY() + v * v1.GetY() + w * v2.GetY())-oy+0.5;
            oy = (u * v0.GetY() + v * v1.GetY() + w * v2.GetY())+0.5;
            if(getPosition3D().y()>oy){
                a = {0,-1 ,0};
                a*g;
                // Oppdaterer hastighet og posisjon
                p = p + v_0*dt+(1/2)*a*(dt*dt);
                v_0=v_0+a*dt;
            }
            else{
                freeFalling =false;
                    if (dt>=tm && loggedPoint.size()<=5){
                        loggedPoint.push_back(QVector3D(getPosition3D()));
                        tm+=ti;
                    }
                // beregn akselerasjonsvektor = ligning (7)
                a = {n.x()*n.y(),n.y()*n.y()-1 ,n.z()*n.y()};
                a*g;

                // beregner friksjon
                float mu{0};/*
                if (i == 2)
                    mu = 0;
                else
                    mu = 0.272;*/
                Vec3 fric = {n.x()*n.y(),n.y()*n.y()-1 ,n.z()*n.y()};
                fric *= mu;
                a -= fric;
                // Oppdaterer hastighet og posisjon
                p =p + v_0*dt+(1/2)*a*(dt*dt);
                v_0=v_0+a*dt;

                v_0.setY(y);
                if ( i != old_index)
                {
                    // Ballen har rullet over pa nytt triangel
                    // Beregner normalen til kollisjons planet ,
                    // se ligning ( 9 )
                    Vec3 px;/*
                    Vec3 oldNtest = old_normal;
                    Vec3 nTest = n;*/
                    if (Vec3(old_normal+n).length()<0)
                        px = (old_normal + n)/(Vec3(old_normal+n).length()*-1);
                    else
                        px = (old_normal + n)/(Vec3(old_normal+n).length());
    //                px.normalize();
                    // Korrigere posisjon oppover i normalens retning
                    //Vec3 ytest = (p-vec1)*px;




    //                p.setY(p.y()+px.y()*0.5);
    //                Vec3 ds = px*(v_0*px);
    //                mPosition.translate(-ds);
    //                Vec3 yk = (p-vec1)*n;
    //                Vec3 yn = n*(yk*n);
    //                float d = yn.length()-r;
    //                mPosition.translate(0, n.y()*d,0);
    //                p.setY(p.y()+d);
                    // Oppdater hastighetsvektoren , se ligning ( 8 )
                    v_0=v_0-2*(v_0*px)*px;
                    Vec3 FartsMol = v_0;
                    int fakestuff = 0;
    //                v_0.setY(-v_0.y());
                    // Oppdatere posisjon i retning den nye
                    // hastighetsvektoren5
                    /*Vec3 dsn = px*(v_0*px);
                    mPosition.translate((dsn.x()+d), 0,(dsn.z()+d));*/
    //                mPosition.translate(v_0.x()*d,0,v_0.z()*d);
                    //p = v_0*dt+(1/2)*a*(dt*dt);
                    //y =(u * v0.GetY() + v * v1.GetY() + w * v2.GetY());

                    v_0.setY(y);
                }
            }
            if(!freeFalling && !landed){
//                Vec3 crash{(Vec3(-1,0,-1) + n)/(Vec3(Vec3(0,-1,0)+n).length())};
//                crash.normalize();
//                v_0=v_0-2*(v_0*crash)*crash;
                v_0=v_0-2*(v_0*n)*n;
                landed=true;
                v_0.setY(y);
                tm=dt+ti;
            }


            // Oppdater gammel normal og indeks
            old_normal = n;
            old_index = i;
        }
    };
    if(!onTriangle){
        a = {0,-1 ,0};
        a*g;
        // Oppdaterer hastighet og posisjon
        p = v_0*dt+(1/2)*a*(dt*dt);
        v_0=v_0+a*dt;
    }
    //mPosition.setToIdentity();
    //p = p + v_0;
    //float temp = p.y();

    //mPosition.translate(v_0.x(), y, v_0.z());

    mPosition.translate(v_0);
    mMatrix = mPosition * mRotation * mScale;
}

bool RollingBall::splineCheck()
{
    if(loggedPoint.size()==5 && !pointLogged){
        pointLogged = true;
        return true;
    }
    return false;
}

void RollingBall::UpdateNormal()
{
    //qreal x = mMatrix.registeredProperty("x")->value().toreal();
    float x = mMatrix.operator()(0,3);
    float y = mMatrix.operator()(1,3);
    float z = mMatrix.operator()(2,3);

    QVector3D point = triangle_surface->PointOnTriangle(x,y,z);
}

std::vector<QVector3D> RollingBall::givePoints()
{
    return loggedPoint;
}

void RollingBall::init(GLint matrixUniform)
{
   mMatrixUniform = matrixUniform;
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

void RollingBall::init()
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

void RollingBall::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}
