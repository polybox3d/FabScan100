#ifndef MAINWIDGET_H
#define MAINWIDGET_H



#include <../eigen3/Eigen/Core>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#ifndef Q_MOC_RUN
#include <pcl/point_types.h>
#endif

#include <QFileDialog>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>
#include <QtOpenGL/QGLShaderProgram>
#include <QPointF>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>



QT_BEGIN_NAMESPACE
class QBasicTimer;
class QGLShaderProgram;
QT_END_NAMESPACE

class GeometryEngine;

class MainWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    virtual ~MainWidget();
    char drawState;
    void paintGL();

signals:

public slots:

protected:
    void wheelEvent(QWheelEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);

    void initializeGL();
    void resizeGL(int w, int h);

    void initShaders();
    void initTextures();

private:
    QBasicTimer *timer;
    QGLShaderProgram *program;
    GeometryEngine *geometries;
    GLuint texture;

    //Transformation matrices for the camera
    QMatrix4x4 projection;
    QVector3D eye;
    QVector3D center;
    QVector3D up;
    double angleX;
    double angleY;
    double angleXtmp;
    double angleYtmp;
    double distance;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
};

#endif // MAINWIDGET_H
