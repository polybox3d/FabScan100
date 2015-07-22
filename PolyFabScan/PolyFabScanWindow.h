#ifndef POLYFABSCANWINDOW_H
#define POLYFABSCANWINDOW_H

#include "staticHeaders.h"
#include "fsdialog.h"

#include <QWidget>
#include <QSound>
#include <QTimer>
#include <QThread>
#include <QImage>
#include <QPixmap>

#include "fscontroller.h"
#include "LaserDockWidget.h"


QT_BEGIN_NAMESPACE
class QBasicTimer;
class QGLShaderProgram;
QT_END_NAMESPACE


typedef enum _FSState {
    POINT_CLOUD,
    SURFACE_MESH,
    SCANNING
} FSState;

namespace Ui {
class PolyFabScanWindow;
}

class PolyFabScanWindow : public QWidget
{
    Q_OBJECT

public:
    static PolyFabScanWindow* getInstance();
    ~PolyFabScanWindow();
    void showDialog(QString dialogText);
    void redraw();
    void doneScanning();
    void setScanProgressCurrentDegree(double degree);
    void setScanProgressCurrentYPos(int y);
    void displayScanProgress();

    void displayFrame(cv::Mat frame);
    void displayLoadingFrame(QString text);


    cv::Mat frameGUITransformation(cv::Mat& src);

    static QPixmap cvMat2qpixmap(cv::Mat mat)
    {
        Mat rgb;
        QPixmap p;
        cvtColor(mat, rgb, CV_BGR2RGB);
        p.convertFromImage(QImage(rgb.data, rgb.cols, rgb.rows, QImage::Format_RGB888));
        return p;
    }
    static void flipMat(cv::Mat &src, cv::Mat& dst )
    {
        cv::flip(src,dst,0);
        Point2f src_center(dst.cols/2.0F, dst.rows/2.0F);

        cv::Mat rot_matrix = getRotationMatrix2D(src_center, 180.0, 1.0);

        cv::Mat rotated_img(Size(dst.size().height, dst.size().width), dst.type());

        warpAffine(dst, rotated_img, rot_matrix, dst.size());
    }

    Ui::PolyFabScanWindow *ui;

public slots:

    void closeEvent(QCloseEvent *event);

private slots:

    void updateHardware();

    void on_scanButton_clicked();


    void on_resolutionComboBox_currentIndexChanged(const QString &arg1);

    void on_newFile_clicked();

    void on_loadFile_clicked();

    void on_saveFile_clicked();

    void on_exportFile_clicked();

    void on_selectWebcam_currentIndexChanged(int index);

    void on_loadProfil_clicked();

    void on_loadDefault_clicked();

    void on_saveProfil_clicked();

    void on_fetchFrameButton_clicked();

    void on_diffImageButton_clicked();

    void on_laserImageButton_clicked();

    void on_detectLaserButton_clicked();


    void on_bluLine_pressed();
    void on_bluLine_released();

    void on_cropTopLine_pressed();
    void on_cropTopLine_released();

    void on_cropBotLine_pressed();
    void on_cropBotLine_released();

    void on_laserLine_pressed();
    void on_laserLine_released();

    void on_editLines_clicked();

    void on_editLines_toggled(bool checked);

protected:

    bool eventFilter(QObject *obj, QEvent *event);

private:

    QTimer hwTimer; //updates connected hw:arduino,webcam,...
    void applyState(FSState s);
    void enumerateWebCams();
    void unmoveLines();
    void setConfigFromLines();
    void setLinesFromConfig();

    static PolyFabScanWindow* _instance;
    PolyFabScanWindow(QWidget *parent = 0);
    FSState state;
    FSDialog* dialog;
    bool _update;
    double _currentScanDegree;
    int _currentScanY;

    bool _moveBluLine;
    bool _moveCropTopLine;
    bool _moveCropBotLine;
    bool _moveLaserLine;

};

#endif // POLYFABSCANWINDOW_H
