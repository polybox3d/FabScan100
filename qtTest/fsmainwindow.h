#ifndef FsMainWindow_H
#define FsMainWindow_H

#include "staticHeaders.h"
#include "fsdialog.h"
#include "fscontrolpanel.h"

#include "HomeButton.h"
#include "MainWindow.h"
#include "SerialPort.h"

QT_BEGIN_NAMESPACE
class QBasicTimer;
class QGLShaderProgram;
QT_END_NAMESPACE

#include <QCameraInfo>

typedef enum _FSState {
    POINT_CLOUD,
    SURFACE_MESH,
    SCANNING
} FSState;

namespace Ui {
class FsMainWindow;
}

class FsMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit FsMainWindow(SerialPort* serial, QWidget *parent = 0);
    ~FsMainWindow();
    void showDialog(QString dialogText);
    void redraw();
    FSControlPanel* controlPanel;
    void doneScanning();


private slots:
    void on_scanButton_clicked();
    void timerEvent(QTimerEvent *e);

    void onSelectSerialPort();
    void onSelectWebCam();
    void openPointCloud();
    void savePointCloud();
    void newPointCloud();
    void showControlPanel();
    void exportSTL();
    void readConfiguration();

    void on_resolutionComboBox_currentIndexChanged(const QString &arg1);

private:
    QBasicTimer *hwTimer; //updates connected hw:arduino,webcam,...
    Ui::FsMainWindow *ui;
    FSState state;
    FSDialog* dialog;

    void setupMenu();
    void enumerateSerialPorts();
    void enumerateWebCams();
    void applyState(FSState s);
};

#endif // FsMainWindow_H
