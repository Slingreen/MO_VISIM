#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <string>

#include "shader.h"
#include "mainwindow.h"
#include "logger.h"

#include "xyz.h"

#include "trianglesurface.h"
#include "lasmap.h"
#include "octahedronball.h"

#include "texture.h"
#include "texturetriangle.h"
#include <QVector3D>
#include "pointlight.h"
#include "quad.h"

#include "bspline.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);


}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
//    glDeleteVertexArrays( 1, &mVAO );
//    glDeleteBuffers( 1, &mVBO );

//    for (int i = mObjects.size()-1; i > 0; i--){
//        delete mObjects[i];
//        mObjects.pop_back();
//    }
}


// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    start = std::chrono::system_clock::now();
//  !!!!!! Dont know if this is still needed !!!!!!
    //Vertex Buffer Object to hold vertices - VBO
    //glGenBuffers( 1, &mVBO );
    //glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    //xyz.setContext(mContext);//DN190111
//  !!!!!! --------------------------------- !!!!!!

    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));

    //Print info about opengl texture limits on this GPU:
    int textureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
    tempString += std::string("  This GPU as ") + std::to_string(textureUnits) + std::string(" texture units / slots in total, \n");

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    tempString += std::string("  and supports ") + std::to_string(textureUnits) + std::string(" texture units pr shader");

    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //********************** General OpenGL stuff: **********************
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.

    //set up alpha blending for textures
    glEnable(GL_BLEND);// you enable blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //********************** Compile shaders: **********************
    // Get the matrixUniform location from the shader
    // This has to match the "matrix" variable name in the vertex shader
    // The uniform is used in the render() function to send the model matrix to the shader

//    mShaderProgram[0] = new Shader("../3Dprog22/plainshader.vert", "../3Dprog22/plainshader.frag");
//    mMatrixUniform = glGetUniformLocation( mShaderProgram[0]->getProgram(), "mMatrix" );
//    mPmatrixUniform = glGetUniformLocation( mShaderProgram[0]->getProgram(), "pMatrix" );
//    mVmatrixUniform = glGetUniformLocation( mShaderProgram[0]->getProgram(), "vMatrix" );

    //********************** Compile shaders: **********************
    // Get the matrixUniform location from the shader
    // This has to match the "matrix" variable name in the vertex shader
    // The uniform is used in the render() function to send the model matrix to the shader
    mShaderProgram[0] = new Shader("../3Dprog22/plainshader.vert", "../3Dprog22/plainshader.frag");
    mLogger->logText("Plain shader program id: " + std::to_string(mShaderProgram[0]->getProgram()) );
    mShaderProgram[1]= new Shader("../3Dprog22/textureshader.vert", "../3Dprog22/textureshader.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[1]->getProgram()) );
    mShaderProgram[2]= new Shader("../3Dprog22/PhongPointLight.vert", "../3Dprog22/PhongPointLight.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[1]->getProgram()) );
    mShaderProgram[3]= new Shader("../3Dprog22/PhongNoTexture.vert", "../3Dprog22/PhongNoTexture.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[3]->getProgram()) );


    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);
    setupPhongShader2(3);



    //********************** Texture stuff: **********************
    mTexture[0] = new Texture();
    mTexture[1] = new Texture("../3Dprog22/Assets/hund.bmp");

    //Set the textures loaded to a texture unit (also called a texture slot)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture[0]->id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture[1]->id());

    //********************** Making Objects **********************
    mObjects.push_back(ny = new XYZ());
    ny->setName("XYZ");
    ny->init();
    mObjects.push_back(mLight = new PointLight());
    ny = mLight;
    ny->setName("PointLight");
    ny->init();
    ny->mMatrix.translate(25.f,  20.0f,  10.f);
    mObjects.push_back(Terreng = new TriangleSurface("../3Dprog22/formE4.txt"));
    ny = Terreng;
    ny->setName("form");
    ny->init();
    mObjects.push_back(ny = new lasmap("../3Dprog22/namdal.txt")); //namdal, formE4
    ny->setName("pointCloud");
    ny->init();

//    mObjects.push_back(ny = new RollingBall(3,mObjects[1]));
//    mObjects.push_back(ny = new RollingBall(3,0,20,0,mObjects[1]));
//    ny->setName("ball");
//    ny->init();

//    mObjects.push_back(ny = new bSpline(/*mObjects[2]*/));
//    ny->setName("spline");
//    ny->init();


    //********************** Object Containers **********************
    //  Map
    for (auto it=mObjects.begin(); it!=mObjects.end(); it++)
    {
        mMap.insert(std::pair<std::string, VisualObject*>{(*it)->getName(), *it});
    }

    //  QuadTre
//    gsml::Point2D a{-16, -16}, b{16, -16}, c{16, 16}, d{-16, 16}; // må gjøres ordentlig // 32x32(-16 -> 16)
//    mQuadTre.init(a, b, c, d);
//    mQuadTre.subDivide(1);
//    auto subtre = mQuadTre.find(gsml::Point2D(13,13));  //16x16(0 -> 16)
//    subtre->subDivide(1);
//    subtre = mQuadTre.find(gsml::Point2D(13,13));       //8x8(8 -> 16)
//    subtre->subDivide(1);
//    subtre = mQuadTre.find(gsml::Point2D(13,13));       //4x4(12 -> 16)
//    subtre->subDivide(1);
//    subtre = mQuadTre.find(gsml::Point2D(13,13));       //2x2(12 -> 14)
//    subtre->subDivide(1);

//    for (auto it=mObjects.begin(); it!=mObjects.end(); it++)
//    {
//        mQuadTre.insert((*it)->getPosition2D(), (*it)->getName(), *it);  // bør ha et navn
//    }

    //********************** Camera Setup **********************
    mCurrentCamera = new Camera();

    mCurrentCamera->setPosition(QVector3D(4.0f, 20.f, -5.0f));
    //mCurrentCamera->setPosition(QVector3D(0.0f, 0.0f, 0.0f));
    mCurrentCamera->pitch(40.f);
    mCurrentCamera->yaw(180.f);


    glBindVertexArray(0);       //unbinds any VertexArray - good practice
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
//      Old camera stuff

//    glUseProgram(mShaderProgram[0]->getProgram() );
//    mMmatrix->setToIdentity();
//    mPmatrix->setToIdentity();
//    mVmatrix->setToIdentity();

//    // flere matriser                          Render distance
//    mPmatrix->perspective(60, 4.0f/3.0f, 0.1f, 20.0f);

//    //what shader to use


//    qDebug() << *mPmatrix;
//       // Flytter kamera
//      //                  Side      Høyde    Dybde
//      mVmatrix->translate(0,        0,       -9);
//      // Flere matriser her! Skal legges i kameraklasse
//      glUniformMatrix4fv( mPmatrixUniform, 1, GL_FALSE, mPmatrix->constData());
//      glUniformMatrix4fv( mVmatrixUniform, 1, GL_FALSE, mVmatrix->constData());

    //Keyboard / mouse input
    handleInput();

    mCurrentCamera->update();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //mObjects[4]->move(0.1f, 0.f, 0);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    //auto x = std::chrono::system_clock::now();
    //float y = x.count();

    dt++;
//    float deltaTime = elapsed_time.count();
    float deltaTime = dt/60;
    qDebug() << elapsed_time.count();
//    mObjects[2]->move(deltaTime/1000);
//    mObjects[3]->move(deltaTime/1000);
    /*
    //what shader to use
    glUseProgram(mShaderProgram[0]->getProgram() );

    //send data to shader

    //XYZ
    glUniformMatrix4fv( vMatrixUniform0, 1, GL_FALSE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform0, 1, GL_FALSE, mCurrentCamera->mProjectionMatrix.constData());
    glUniformMatrix4fv( mMatrixUniform0, 1, GL_FALSE, mObjects[0]->mMatrix.constData());
    mObjects[0]->draw();

    //Light
    glUniformMatrix4fv( mMatrixUniform0, 1, GL_FALSE, mObjects[2]->mMatrix.constData());
    mObjects[2]->draw();

    //Special object
    //Texture test
//    glUseProgram(mShaderProgram[1]->getProgram());
//    glUniformMatrix4fv( vMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
//    glUniformMatrix4fv( pMatrixUniform1, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
//    glUniformMatrix4fv( mMatrixUniform1, 1, GL_TRUE, mObjects[1]->mMatrix.constData());
//    glUniform1i(mTextureUniform, 1);
//    mObjects[1]->draw();

    //Light test
    glUseProgram(mShaderProgram[2]->getProgram());
    glUniformMatrix4fv( vMatrixUniform2, 1, GL_FALSE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv( pMatrixUniform2, 1, GL_FALSE, mCurrentCamera->mProjectionMatrix.constData());
    glUniformMatrix4fv( mMatrixUniform2, 1, GL_FALSE, mObjects[1]->mMatrix.constData());
    checkForGLerrors();
    //Additional parameters for light shader:
    glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(),
                mLight->mMatrix.column(3).z());
    glUniform3f(mCameraPositionUniform, mCurrentCamera->mPosition.x(), mCurrentCamera->mPosition.y(), mCurrentCamera->mPosition.z());
    glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
    glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrength);
    //Texture
    glUniform1i(mTextureUniform2, 1);
    //*/
    //*
    // draw call
    for(auto it=mObjects.begin(); it!=mObjects.end(); it++)
    {
        textureRender(*it);
    }//*/

    //  Task 3.1, one ball placed interactively
    if(mTask31){
        if(ball == nullptr){
            bdt = deltaTime;
            ball = new RollingBall(3,bX,20,bZ,Terreng);
            ball->setName("ball");
            ball->init();
        }
        textureRender(ball);
        ball->move((deltaTime-bdt)/100);
    }
    else{
        if(ball != nullptr){
            delete ball;
            ball = nullptr;
        }
    }

    //  Task 3.2, rain of balls
    float fx{0};
    float fz{0};
    if(mTask32){
        if(mBalls.empty()){
            rdt = deltaTime;
            for(int i = 0; i<aBalls; i++){
                fx = (std::rand()%(20));
                fz = (std::rand()%(20));
                mBalls.push_back(new RollingBall(3,fx,60,fz,Terreng));
                mBalls[i]->setName("ball");
                mBalls[i]->init();
            }
        }
        for(auto it=mBalls.begin(); it!=mBalls.end(); it++)
        {
            textureRender(*it);
            (*it)->move((deltaTime-rdt)/100);
            if((*it)->splineCheck()){
                mSplines.push_back(ny = new bSpline((*it)->givePoints()));
                ny->setName("XYZ");
                ny->init();
            }

        }
        for(auto it=mSplines.begin(); it!=mSplines.end(); it++)
        {
            textureRender(*it);
        }
    }
    else{
        if(!mBalls.empty()){
            for(int i = mBalls.size()-1; i>=0; i--){
                delete mBalls[i];
                mBalls.pop_back();
            }
            for(int i = mSplines.size()-1; i>=0; i--){
                delete mSplines[i];
                mSplines.pop_back();
            }
        }
    }

    //iob->draw();

    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
}

void RenderWindow::setupPlainShader(int shaderIndex)
{
    mMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    pMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    vMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    pMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    vMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    mTextureUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupPhongShader(int shaderIndex)
{
    mMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );

    mLightColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightColor" );
    mObjectColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "ambientStrengt" );
    mLightPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularExponent" );
    mLightPowerUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPower" );
    mCameraPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "cameraPosition" );
    mTextureUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupPhongShader2(int shaderIndex)
{
    mMatrixUniform3 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform3 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform3 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );

    mLightColorUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightColor" );
    mObjectColorUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "ambientStrengt" );
    mLightPositionUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularExponent" );
    mLightPowerUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPower" );
    mCameraPositionUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "cameraPosition" );
}

void RenderWindow::textureRender(VisualObject *vo)
{
    //it get shader
    shaderInt = vo->getShader();

    // get program
    glUseProgram(mShaderProgram[shaderInt]->getProgram());

    //switch shader type
    switch(shaderInt){
    case 0:                     //  Unshaded
        glUniformMatrix4fv( vMatrixUniform0, 1, GL_FALSE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( pMatrixUniform0, 1, GL_FALSE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( mMatrixUniform0, 1, GL_FALSE, vo->mMatrix.constData());
        vo->draw();
        break;
    case 1:                     //  Texutre
        glUniformMatrix4fv( vMatrixUniform1, 1, GL_FALSE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( pMatrixUniform1, 1, GL_FALSE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( mMatrixUniform1, 1, GL_FALSE, vo->mMatrix.constData());
        glUniform1i(mTextureUniform, 1);
        vo->draw();
        break;
    case 2:                     //  Texture phong shaded
        glUniformMatrix4fv( vMatrixUniform2, 1, GL_FALSE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( pMatrixUniform2, 1, GL_FALSE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( mMatrixUniform2, 1, GL_FALSE, vo->mMatrix.constData());
        checkForGLerrors();
        //Additional parameters for light shader:
        glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(),
                    mLight->mMatrix.column(3).z());
        glUniform3f(mCameraPositionUniform, mCurrentCamera->mPosition.x(), mCurrentCamera->mPosition.y(), mCurrentCamera->mPosition.z());
        glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
        glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrength);
        //Texture
        glUniform1i(mTextureUniform2, 1);
        vo->draw();
        break;
    case 3:                     //  Un-textured phong shaded
        glUniformMatrix4fv( vMatrixUniform3, 1, GL_FALSE, mCurrentCamera->mViewMatrix.constData());
        glUniformMatrix4fv( pMatrixUniform3, 1, GL_FALSE, mCurrentCamera->mProjectionMatrix.constData());
        glUniformMatrix4fv( mMatrixUniform3, 1, GL_FALSE, vo->mMatrix.constData());
        checkForGLerrors();
        //Additional parameters for light shader:
        glUniform3f(mLightPositionUniform2, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(),
                    mLight->mMatrix.column(3).z());
        glUniform3f(mCameraPositionUniform2, mCurrentCamera->mPosition.x(), mCurrentCamera->mPosition.y(), mCurrentCamera->mPosition.z());
        glUniform3f(mLightColorUniform2, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
        glUniform1f(mSpecularStrengthUniform2, mLight->mSpecularStrength);
        vo->draw();
        break;

    }
}

void RenderWindow::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within some min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if (mCameraSpeed > 0.3f)
        mCameraSpeed = 0.3f;
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }

    //calculate aspect ration and set projection matrix
    mAspectratio = static_cast<float>(width()) / height();
//                                              Angle   Aspect      near    far
    mCurrentCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 0.1f, 400.f);
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}


void RenderWindow::handleInput()
{
    //Camera
//*
    mCurrentCamera->setSpeed(0.f);  //cancel last frame movement
    if(mInput.RMB)
    {
        if(mInput.UP)
            mCurrentCamera->setSpeed(-mCameraSpeed);
        if(mInput.DOWN)
            mCurrentCamera->setSpeed(mCameraSpeed);
        if(mInput.RIGHT)
            mCurrentCamera->moveRight(mCameraSpeed);
        if(mInput.LEFT)
            mCurrentCamera->moveRight(-mCameraSpeed);
        if(mInput.Q)
            mCurrentCamera->updateHeigth(-mCameraSpeed);
        if(mInput.E)
            mCurrentCamera->updateHeigth(mCameraSpeed);
    }
    if(mInput.W)
        mCurrentCamera->setSpeed(-mCameraSpeed);
    if(mInput.S)
        mCurrentCamera->setSpeed(mCameraSpeed);
    if(mInput.D)
        mCurrentCamera->moveRight(mCameraSpeed);
    if(mInput.A)
        mCurrentCamera->moveRight(-mCameraSpeed);
    if(mInput.Q)
        mCurrentCamera->updateHeigth(-mCameraSpeed);
    if(mInput.E)
        mCurrentCamera->updateHeigth(mCameraSpeed);
//*/
}


//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }

    //You get the keyboard input like this
    if(event->key() == Qt::Key_A)
    {
        /*  Old movement
        //iob->move(-0.1f,0.0f,0.0f);
        mMap["Player"]->move(-0.1f,0.0f,0.0f);
        ColCheck();
        /*/
        mInput.A = true;
        mMainWindow->statusBar()->showMessage(" AAAA");
    }
    if(event->key() == Qt::Key_S)
    {
        /*  Old movement
        iob->move(0.0f,-0.1f,0.0f);
        ColCheck();
        /*/
        mInput.S = true;
        mMainWindow->statusBar()->showMessage(" SSSS");
    }
    if(event->key() == Qt::Key_D)
    {
        /*  Old movement
        iob->move(0.1f,0.0f,0.0f);
        ColCheck();
        /*/
        mInput.D = true;
        mMainWindow->statusBar()->showMessage(" DDDD");
    }
    if(event->key() == Qt::Key_W)
    {
        /*  Old movement
        iob->move(0.0f,0.1f,0.0f);
        ColCheck();
        /*/
        mInput.W = true;
        mMainWindow->statusBar()->showMessage(" WWWW");
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = true;
        mMainWindow->statusBar()->showMessage(" QQQQ");
    }
    if(event->key() == Qt::Key_E)
    {
        /*  Old movement
        auto posisjon = mMap["Player"]->getPosition2D();
        auto subtre = mQuadTre.find(posisjon);
        for (auto it = subtre->begin(); it != subtre->end(); it ++)
        {
            if((*it)->getName() == "Doori")
            {
                iob->move(0.1f,0.0f,0.0f);
                (*it)->collision();
            }
            if((*it)->getName() == "Dooro")
            {
                (*it)->collision();
            }


        }
        /*/
        mInput.E = true;
        mMainWindow->statusBar()->showMessage(" EEEE");
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = true;
        mMainWindow->statusBar()->showMessage("Up");
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = true;
        mMainWindow->statusBar()->showMessage("Down");
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = true;
        mMainWindow->statusBar()->showMessage("Left");
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = true;
        mMainWindow->statusBar()->showMessage("Right");
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        mInput.W = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = false;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = false;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = false;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = false;
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = false;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = false;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = false;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = false;
    }
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = true;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = true;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = false;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = false;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = false;
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    //if RMB, change the speed of the camera
    if (mInput.RMB)
    {
        if (numDegrees.y() < 1)
            setCameraSpeed(-0.002f);
        if (numDegrees.y() > 1)
            setCameraSpeed(0.002f);
    }
//    event->accept();    //tell Qt that we used the event
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mInput.RMB)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mCurrentCamera->yaw(mCameraRotateSpeed * mMouseXlast);
        if (mMouseYlast != 0)
            mCurrentCamera->pitch(mCameraRotateSpeed * mMouseYlast);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();
}


void RenderWindow::ColCheck(){
//    auto posisjon = mMap["Player"]->getPosition2D();
//    auto subtre = mQuadTre.find(posisjon);
//    for (auto it = subtre->begin(); it != subtre->end(); it ++)
//    {
//        for (int i = 0; i<4; i++){
//            std::string temp = "Trophy ";
//            temp += std::to_string(i+1);
//            if((*it)->getName() == temp)
//            {
//                (*it)->collision();
//            }
//        }
//    }
}
