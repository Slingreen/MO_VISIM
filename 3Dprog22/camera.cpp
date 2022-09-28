#include "camera.h"
#include <QDebug>
#include <cmath>

Camera::Camera()
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();
}

void Camera::pitch(float degrees)
{
    //  rotate around mRight
    mPitch -= degrees;
    updateForwardVector();
}

void Camera::yaw(float degrees)
{
    // rotate around mUp
    mYaw -= degrees;
    updateForwardVector();
}

void Camera::updateRightVector()
{
//    mRight = mForward^mUp;
//    mRight.normalize();
    mRight = QVector3D::crossProduct(mForward, mUp);
    mRight.normalize();
//    qDebug() << "Right " << mRight;
}

void Camera::updateForwardVector()
{
    mRight = QVector3D(1.f, 0.f, 0.f);

    //mRight.rotateY(mYaw);
    RotateY(mYaw,mRight);
    mRight.normalize();
    mUp = QVector3D(0.f, 1.f, 0.f);

    //mUp.rotateX(mPitch);
    RotateX(mPitch,mUp);

    //mUp.rotateY(mYaw);
    RotateY(mYaw,mUp);
    mUp.normalize();

    //mForward = mUp^mRight;
    mForward = QVector3D::crossProduct(mUp, mRight);

    updateRightVector();
}

void Camera::update()
{
    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    //mPitchMatrix.rotateX(mPitch);
    RotateX(mPitchMatrix, mPitch);

    //mYawMatrix.rotateY(mYaw);
    RotateY(mYawMatrix, mYaw);

    mPosition = mPosition - (mForward * mSpeed);

    mViewMatrix = mPitchMatrix* mYawMatrix;
    mViewMatrix.translate(-mPosition);
}

void Camera::setPosition(const QVector3D &position)
{
    mPosition = position;
    update();
}

void Camera::setSpeed(float speed)
{
    mSpeed = speed;
}

void Camera::updateHeigth(float deltaHeigth)
{
//    mPosition.y += deltaHeigth;
    float temp = deltaHeigth + mPosition.y();
    mPosition.setY(temp);

    //mPosition.setY(deltaHeigth + mPosition.y());
}

void Camera::moveRight(float delta)
{
    //This fixes a bug in the up and right calculations
    //so camera always holds its height when straifing
    //should be fixed thru correct right calculations!
//    gsl::Vector3D right = mRight;
//    right.y = 0.f;
//    mPosition += right * delta;
    QVector3D right = mRight;
    right.setY(0.f);
    mPosition = mPosition + (right * delta);

    // mPosition += mRight * delta;
}

QVector3D Camera::position() const
{
    return mPosition;
}

QVector3D Camera::up() const
{
    return mUp;
}

void Camera::RotateX(float angle, QVector3D& r)
{
    QVector3D dir;
    angle = (angle * M_PI)/180.0f;

    dir.setX(r.x());
    dir.setY((r.y() * std::cos(angle)) + (r.z() * (-std::sin(angle))));
    dir.setZ((r.y() * std::sin(angle)) + (r.z() * std::cos(angle)));

    r.setX(dir.x());
    r.setY(dir.y());
    r.setZ(dir.z());

}

void Camera::RotateY(float angle, QVector3D& r)
{
    QVector3D dir;
    angle = (angle * M_PI)/180.0f;

    dir.setX((r.x() * std::cos(angle)) + (r.z() * std::sin(angle)));
    dir.setY(r.y());
    dir.setZ((r.x() * (-std::sin(angle))) + (r.z() * std::cos(angle)));

    r.setX(dir.x());
    r.setY(dir.y());
    r.setZ(dir.z());

}

void Camera::RotateX(QMatrix4x4 &m, float angle)
{
    angle = (angle * M_PI)/180.0f;

        QMatrix4x4 temp =
        {
            1.f,   0.f,       0.f,    0.f,
            0.f, std::cos(angle),  std::sin(angle), 0.f,
            0.f, -std::sin(angle), std::cos(angle), 0.f,
            0.f,   0.f,       0.f,    1.f
        };

        m = (m)*temp;
}

void Camera::RotateY(QMatrix4x4 &m, float angle)
{
    angle = (angle * M_PI)/180.0f;

        QMatrix4x4 temp =
        {
            std::cos(angle), 0.f, -std::sin(angle), 0.f,
            0.f,       1.f,      0.f,       0.f,
            std::sin(angle), 0.f,  std::cos(angle), 0.f,
            0.f,       0.f,      0.f,       1.f
        };

        m = (m)*temp;
}

//void Camera::RotateZ(float angle, QVector3D r)
//{

//}
