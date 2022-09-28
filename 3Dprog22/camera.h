#ifndef CAMERA_H
#define CAMERA_H


#include <QMatrix4x4>
#include <QVector3D>
/**
  This class still have some bugs. It mostly work, but when you rotate the camera 180 degrees
  the forward / backward is wrong, when steered with W and S.
 */
class Camera
{
public:
    Camera();

    void pitch(float degrees);
    void yaw(float degrees);
    void updateRightVector();
    void updateForwardVector();
    void update();

    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    void setPosition(const QVector3D &position);

    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);

    QVector3D position() const;
    QVector3D up() const;

    QVector3D mPosition{0.f, 0.f, 0.f};
private:
    QVector3D mForward{0.0f, 0.0f, -1.0f};
    QVector3D mRight{1.f, 0.0f, 0.0f};
    QVector3D mUp{0.0f, 1.0f, 0.0f};


    float mPitch{0.f};
    float mYaw{0.f};

    QMatrix4x4 mYawMatrix;
    QMatrix4x4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector

    void RotateX(float angle, QVector3D& r);
    void RotateY(float angle, QVector3D& r);
//    void RotateZ(float angle, QVector3D r);

    void RotateX(QMatrix4x4& m, float angle);
    void RotateY(QMatrix4x4& m, float angle);
//    void RotateZ(QMatrix4x4& m, float angle);


};

#endif // CAMERA_H
