#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include <vector>
#include <unordered_map>

#include "visualobject.h"
#include "interactiveobject.h"
#include "TQuadtre.h"
#include "camera.h"
#include "Input.h"
#include "underlag.h"
#include <random>

#include "rollingball.h"


class QOpenGLContext;
class Shader;
class MainWindow;
class Texture;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    //********************** UI interaction: **********************
    bool mRotate{true};     //Check if triangle should rotate
    bool mTerrain{false};
    bool mTask31{false};
    bool mTask32{false};

    float bX{0}, bZ{0};
//    void toggleWireFrame(bool toggle);
//    void toggleBackfaceCulling(bool toggle);


    class PointLight *mLight{nullptr};
private slots:
    void render();          //the actual render - function

private:
    //********************** OpenGL stuff: **********************

    void init();            //initialize things we need before rendering

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

    //********************** Timey stuff: **********************
    float dt{0};
    int old_t;
    std::chrono::time_point<std::chrono::system_clock> start;
    float bdt{0}, rdt{0}; // ball sin delta time, og regn sin delta time

    //********************** Object stuff: **********************
    std::vector<VisualObject*> mObjects;
    VisualObject* Terreng = nullptr;
    VisualObject* PointCloud = nullptr;
    VisualObject* Surface = nullptr;
    VisualObject* ball = nullptr;
    std::vector<RollingBall*> mBalls;
    std::vector<VisualObject*> mSplines;
    int aBalls{5};                                          //(a)mount of Balls
    std::unordered_map<std::string, VisualObject*> mMap;
    //gsml::QuadTre<std::string, VisualObject*> mQuadTre;

    //VisualObject* iob;
    VisualObject* ny;
    InteractiveObject* iob;

    int shaderInt =0;   //Helps rendering

    //********************** Camera stuff: **********************

    void setCameraSpeed(float value);
    void handleInput();

    Camera *mCurrentCamera{nullptr};
    float mAspectratio{1.f};

    Input mInput;
    float mCameraSpeed{0.1f};
    float mCameraRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};

    //********************** Texture stuff: **********************

    void setupPlainShader(int shaderIndex);
        GLint mMatrixUniform0{-1};
        GLint vMatrixUniform0{-1};
        GLint pMatrixUniform0{-1};

    void setupTextureShader(int shaderIndex);
        GLint mMatrixUniform1{-1};
        GLint vMatrixUniform1{-1};
        GLint pMatrixUniform1{-1};
        GLint mTextureUniform{-1};

    void setupPhongShader(int shaderIndex);
        GLint mMatrixUniform2{-1};
        GLint vMatrixUniform2{-1};
        GLint pMatrixUniform2{-1};

        GLint mLightColorUniform{-1};
        GLint mObjectColorUniform{-1};
        GLint mAmbientLightStrengthUniform{-1};
        GLint mLightPositionUniform{-1};
        GLint mCameraPositionUniform{-1};
        GLint mSpecularStrengthUniform{-1};
        GLint mSpecularExponentUniform{-1};
        GLint mLightPowerUniform{-1};
        GLint mTextureUniform2{-1};

    void setupPhongShader2(int shaderIndex);
        GLint mMatrixUniform3{-1};
        GLint vMatrixUniform3{-1};
        GLint pMatrixUniform3{-1};

        GLint mLightColorUniform2{-1};
        GLint mObjectColorUniform2{-1};
        GLint mAmbientLightStrengthUniform2{-1};
        GLint mLightPositionUniform2{-1};
        GLint mCameraPositionUniform2{-1};
        GLint mSpecularStrengthUniform2{-1};
        GLint mSpecularExponentUniform2{-1};
        GLint mLightPowerUniform2{-1};

    //Shader *mShaderProgram{nullptr};    //holds pointer the GLSL shader program
    Texture *mTexture[4]{nullptr}; //We can hold 4 textures
    Shader *mShaderProgram[4]{nullptr}; //We can hold 4 shaders

    void textureRender(VisualObject *vo);
    //********************** Render stuff: **********************

//    GLint  mPmatrixUniform;
//    GLint  mVmatrixUniform;
//    GLint  mMatrixUniform;              //OpenGL reference to the Uniform in the shader program

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

//  !!!!!! Dont know if this is still needed !!!!!!
//    GLuint mVAO;                        //OpenGL reference to our VAO
//    GLuint mVBO;                        //OpenGL reference to our VBO

//    QMatrix4x4 *mMmatrix{nullptr};         //The matrix with the transform for the object we draw
//    QMatrix4x4 *mPmatrix{nullptr};         // erstatninger
//    QMatrix4x4 *mVmatrix{nullptr};         // til mMVPmatrix
//  !!!!!! --------------------------------- !!!!!!

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application


    void ColCheck();

protected:
    //The QWindow that we inherit from have these functions to capture
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H
