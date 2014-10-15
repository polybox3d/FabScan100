#include "fsmainwindow.h"
#include "ui_fsmainwindow.h"
#include "fscontroller.h"
#include "fsdialog.h"

#include <QBasicTimer>
#include <QDialogButtonBox>
#include <QFuture>
#include <QtCore>
//#include <QtConcurrentRun>
#include <QtConcurrent/QtConcurrentRun>
#include <QCamera>
#include <QSound>

#include <boost/bind.hpp>

#ifdef LINUX
#include <boost/filesystem.hpp>
#endif

FsMainWindow::FsMainWindow(SerialPort *serial, QWidget *parent) :
    QMainWindow(parent),
    hwTimer(new QBasicTimer),
    ui(new Ui::FsMainWindow)
{
    FSController::getInstance()->serial = serial;
    ui->setupUi(this);


    this->setupMenu();
  //  this->enumerateSerialPorts();
    this->enumerateWebCams();

    // Addd home button
    /*HomeButton* hb = new HomeButton( 50,50, this );
    hb->setGeometry( this->width()-hb->width()-10,
                     this->height()-hb->height(),
                     hb->width(), hb->height());
    connect( hb, SIGNAL(clicked()), MainWindow::getMainWindow(), SLOT(backToModulePage()));
*/

    hwTimer->start(5000, this); //timer that checks periodically for attached hardware (camera, arduino)
    dialog = new FSDialog(this);
    controlPanel = new FSControlPanel(this);
    FSController::getInstance()->mainwindow=this;
    FSController::getInstance()->controlPanel=controlPanel;
    ui->widget->setStyleSheet("border: 1px solid black;");
    applyState(POINT_CLOUD);
    //resolution: Good
    FSController::getInstance()->turntableStepSize = 16*FSController::getInstance()->turntable->degreesPerStep;
    FSController::getInstance()->yDpi = 1;

    this->setWindowFlags(Qt::Widget);
}

FsMainWindow::~FsMainWindow()
{
    //this->setUpdatesEnabled(false);
    hwTimer->stop();
    delete ui;
    FSController::getInstance()->scanning=false;
}

void FsMainWindow::setupMenu()
{
    QAction* newPointCloudAction = new QAction("New", this);
    newPointCloudAction->setShortcuts(QKeySequence::New);
    connect(newPointCloudAction,SIGNAL(triggered()),this, SLOT(newPointCloud()));
    ui->menuFile->addAction(newPointCloudAction);

    QAction* openPointCloudAction = new QAction("Open PointCloud...", this);
    openPointCloudAction->setShortcuts(QKeySequence::Open);
    connect(openPointCloudAction,SIGNAL(triggered()),this, SLOT(openPointCloud()));
    ui->menuFile->addAction(openPointCloudAction);

    QAction* savePointCloudAction = new QAction("Save PointCloud...", this);
    savePointCloudAction->setShortcuts(QKeySequence::Save);
    connect(savePointCloudAction,SIGNAL(triggered()),this, SLOT(savePointCloud()));
    ui->menuFile->addAction(savePointCloudAction);

    QAction* exportSTLAction = new QAction("Export .STL...", this);
    connect(exportSTLAction,SIGNAL(triggered()),this, SLOT(exportSTL()));
    ui->menuFile->addAction(exportSTLAction);

    QAction* readConfiguartion = new QAction("Read Configuration", this);
    connect(readConfiguartion,SIGNAL(triggered()),this, SLOT(readConfiguration()));
    ui->menuFile->addAction(readConfiguartion);

    QAction* showControlPanelAction = new QAction("Control Panel...", this);
    showControlPanelAction->setShortcuts(QKeySequence::Preferences);
    connect(showControlPanelAction,SIGNAL(triggered()),this, SLOT(showControlPanel()));
    ui->menuFile->addAction(showControlPanelAction);
}

void FsMainWindow::
showDialog(QString dialogText)
{
    dialog->setStandardButtons(QDialogButtonBox::Ok);
    dialog->setText(dialogText);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

//===========================================
// Action Methods
//===========================================

void FsMainWindow::exportSTL()
{
    if(FSController::getInstance()->model->pointCloud->empty()){
        this->showDialog("PointCloud is empty! Perform a scan, or open a pointcloud.");
        return;
    }
    QFileDialog d(this, "Save File","","STL (*.stl)");
    d.setAcceptMode(QFileDialog::AcceptSave);
    if(d.exec()){

        QString fileName = d.selectedFiles()[0];
        if(fileName.isEmpty() ) return;
        qDebug() << fileName;

        if(!FSController::getInstance()->meshComputed){
            qDebug() << "Computing mesh...";
            this->showDialog("Will now compute surface mesh, this may take a while...");
            FSController::getInstance()->computeSurfaceMesh();
            FSController::getInstance()->meshComputed = true;
        }
        cout << "Done computing surface mesh, now stl export..." << endl;
        FSController::getInstance()->model->saveToSTLFile(fileName.toStdString());
        this->showDialog("STL export done!");

    }
}

void FsMainWindow::showControlPanel()
{
    controlPanel->show();
    controlPanel->raise();
    controlPanel->activateWindow();
}

void FsMainWindow::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    //this->enumerateSerialPorts();
    //this->enumerateWebCams();
}

//===========================================
// Menu Methods
//===========================================

void FsMainWindow::onSelectSerialPort()
{
 /*   QAction* action=qobject_cast<QAction*>(sender());
    if(!action) return;
    //set new path
    FSController::getInstance()->serial->serialPortPath->clear();
    FSController::getInstance()->serial->serialPortPath->append(action->iconText());
    this->enumerateSerialPorts();
    FSController::getInstance()->serial->connectToSerialPort();*/
}

void FsMainWindow::onSelectWebCam()
{
    QAction* action=qobject_cast<QAction*>(sender());
    if(!action) return;
    FSController::getInstance()->webcam->info.portName=action->iconText(); //eigentlich doppelt gemoppelt, das hier kann weg muss jedoch gekukt werden
    FSController::getInstance()->webcam->setCamera(action->data().toByteArray());
    this->enumerateWebCams();
}

void FsMainWindow::openPointCloud()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File","","Files (*.pcd) ;; PLY (*.ply)");
    if(fileName.isEmpty() ) return;
    if(fileName.endsWith(".pcd", Qt::CaseInsensitive) ){
        FSController::getInstance()->model->loadPointCloudFromPCD(fileName.toStdString());
    }else if(fileName.endsWith(".ply", Qt::CaseInsensitive) ){
        FSController::getInstance()->model->loadPointCloudFromPLY(fileName.toStdString());
    }
    ui->widget->drawState = 0;
    ui->widget->updateGL();
    applyState(POINT_CLOUD);
    FSController::getInstance()->meshComputed=false;
}

void FsMainWindow::savePointCloud()
{
    QFileDialog d(this, "Save File","","PCD (*.pcd) ;; PLY (*.ply)");
    d.setAcceptMode(QFileDialog::AcceptSave);
    if(d.exec()){
        QString fileName = d.selectedFiles()[0];
        //fileName.append(d.selectedNameFilter());
        if(fileName.isEmpty() ) return;
        qDebug() << fileName;
        if(fileName.endsWith(".pcd", Qt::CaseInsensitive) ){
            qDebug() << "Save as pcd file.";
            FSController::getInstance()->model->savePointCloudAsPCD(fileName.toStdString());
        }else if(fileName.endsWith(".ply", Qt::CaseInsensitive) ){
            qDebug() << "Save as ply file.";
            FSController::getInstance()->model->savePointCloudAsPLY(fileName.toStdString());
        }
    }

    ui->widget->drawState = 0;
    ui->widget->updateGL();
}

void FsMainWindow::newPointCloud()
{
    FSController::getInstance()->model->pointCloud->clear();
    FSController::getInstance()->model->surfaceMesh.polygons.clear();
    ui->widget->updateGL();
    applyState(POINT_CLOUD);
    FSController::getInstance()->meshComputed=false;
}

void FsMainWindow::readConfiguration()
{
    if(FSController::config->readConfiguration()){
        this->showDialog("Successfully read configuration file!");
    }else{
        this->showDialog("Configuration file not found or corrupt!");
    }
}

void FsMainWindow::enumerateSerialPorts()
{
 /*   QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    ui->menuSerialPort->clear();

    foreach (QextPortInfo info, ports) {
        if(!info.portName.isEmpty() && !info.portName.startsWith("ttyS")){
            QAction* ac = new QAction(info.portName, this);
            ac->setCheckable(true);
            connect(ac,SIGNAL(triggered()),this, SLOT(onSelectSerialPort()));
            if(FSController::getInstance()->serial->serialPortPath->compare(info.portName)==0){
                ac->setChecked(true);
            }
            ui->menuSerialPort->addAction(ac);
        }
        //qDebug() << "port name:"       << info.portName;
        //qDebug() << "friendly name:"   << info.friendName;
        //qDebug() << "physical name:"   << info.physName;
        //qDebug() << "enumerator name:" << info.enumName;
        //qDebug() << "vendor ID:"       << info.vendorID;
        //qDebug() << "product ID:"      << info.productID;
        //qDebug() << "===================================";
    }*/
}

void FsMainWindow::enumerateWebCams()
{
    if(QCamera::availableDevices().size()==0){
       QAction* a = new QAction("No camera found.", this);
       a->setEnabled(false);
       ui->menuCamera->clear();
       ui->menuCamera->addAction(a);
       return;
    }

    QByteArray cameraDevice;
    QCamera* camera;
    ui->menuCamera->clear();
    foreach(const QByteArray &deviceName, QCamera::availableDevices()) {
        QString description = camera->deviceDescription(deviceName);
        QAction *videoDeviceAction = new QAction(description, this);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant(deviceName));
        connect(videoDeviceAction,SIGNAL(triggered()),this, SLOT(onSelectWebCam()));

        /*if(description.contains("Logitech")){
            FSController::getInstance()->webcam->info.portName=description; //eigentlich doppelt gemoppelt, das hier kann weg muss jedoch gekukt werden
            FSController::getInstance()->webcam->setCamera(videoDeviceAction->data().toByteArray());
        }*/

        if (FSController::getInstance()->webcam->info.portName.compare(description)==0) {
            //cameraDevice = deviceName;
            videoDeviceAction->setChecked(true);
        }
        ui->menuCamera->addAction(videoDeviceAction);
    }
}

void FsMainWindow::on_scanButton_clicked()
{
    //doneScanning();
    //QFuture<void> future = QtConcurrent::run(FSController::getInstance(), &FSController::scanThread);
    bool s = FSController::getInstance()->scanning;
    if (s==false){
        applyState(SCANNING);
        FSController::getInstance()->scanThread();
    }else{
        applyState(POINT_CLOUD);
        this->ui->scanButton->setText("Start Scan");
        FSController::getInstance()->scanning = false;
    }

}

void FsMainWindow::doneScanning()
{
    QSound::play("done.wav");
    this->ui->scanButton->setText("Start Scan");
    FSController::getInstance()->laser->disable();
    FSController::getInstance()->turntable->disable();
    applyState(POINT_CLOUD);
}

void FsMainWindow::redraw()
{
    //ui->widget->drawState = 0;
    ui->widget->updateGL();
}

void FsMainWindow::applyState(FSState s)
{
    state = s;
    switch(state){
    case SCANNING:
        this->ui->widget->drawState=0;
        this->ui->scanButton->setText("Stop Scan");
        break;
    case POINT_CLOUD:
        this->ui->scanButton->setText("Start Scan");
        //the following lines are uncommented since we do not support showing the mesh anymore but just compute and save it
        /*if(FSController::getInstance()->meshComputed){
            this->ui->toggleViewButton->setText("Show SurfaceMesh");
        }else{
            this->ui->toggleViewButton->setText("Compute SurfaceMesh");
        }*/
        break;
    case SURFACE_MESH:
        this->ui->scanButton->setText("Start Scan");
        break;
    }
}

void FsMainWindow::on_resolutionComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1.compare("Best")==0){
        //laserStepSize = 2*laser->degreesPerStep;
        FSController::getInstance()->turntableStepSize = FSController::getInstance()->turntable->degreesPerStep;
        FSController::getInstance()->yDpi = 1;
    }
    if(arg1.compare("Good")==0){
        FSController::getInstance()->turntableStepSize = 16*FSController::getInstance()->turntable->degreesPerStep;
        FSController::getInstance()->yDpi = 1;
    }
    if(arg1.compare("Normal")==0){
        FSController::getInstance()->turntableStepSize = 2*16*FSController::getInstance()->turntable->degreesPerStep;
        FSController::getInstance()->yDpi = 5;
    }
    if(arg1.compare("Poor")==0){
        FSController::getInstance()->turntableStepSize = 10*16*FSController::getInstance()->turntable->degreesPerStep;
        FSController::getInstance()->yDpi = 10;
    }
}
