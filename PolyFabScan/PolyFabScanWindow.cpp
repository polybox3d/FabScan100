#include "PolyFabScanWindow.h"
#include "ui_PolyFabScanWindow.h"

PolyFabScanWindow* PolyFabScanWindow::_instance = 0;

PolyFabScanWindow* PolyFabScanWindow::getInstance()
{
    if ( _instance == 0 )
    {
        _instance = new PolyFabScanWindow( );
    }
    return _instance;
}

PolyFabScanWindow::PolyFabScanWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PolyFabScanWindow)
{
    this->blockSignals( true );
    ui->setupUi(this);
    this->setMouseTracking( true );

    unmoveLines();
    ui->laserLine->setVisible( false );

    _update = false;
    _currentScanDegree = 0;
    _currentScanY = 0;
    installEventFilter( this );
    /*ui->bluLine->installEventFilter( this );
    ui->cropBotLine->installEventFilter( this );
    ui->cropTopLine->installEventFilter( this );*/

    hwTimer.setSingleShot(500);
    hwTimer.start();
    connect(&hwTimer, SIGNAL(timeout()), this, SLOT(updateHardware())  ) ;

    dialog = new FSDialog(this);
    //controlPanel = new FSControlPanel(this);
    FSController::getInstance()->mainwindow = this;
    //FSController::getInstance()->controlPanel = controlPanel;
    ui->scanningView->setStyleSheet("border: 1px solid black;");
    applyState(POINT_CLOUD);
    //resolution: Good
    FSController::getInstance()->turntableStepSize = 16*FSController::getInstance()->turntable->degreesPerStep;
    FSController::getInstance()->yDpi = 1;

    ui->selectSerial->addItem("Polybox3D");
    ui->laserWidget->setScannerModule( PolyboxModule::getInstance()->scannerModule() );
    displayLoadingFrame("View");

    on_editLines_toggled(false);

}

PolyFabScanWindow::~PolyFabScanWindow()
{
    delete ui;
}

void PolyFabScanWindow::unmoveLines()
{
    _moveBluLine = false;
    _moveCropBotLine = false;
    _moveCropTopLine = false;
    _moveLaserLine = false;
}

void PolyFabScanWindow::updateHardware()
{
    this->enumerateWebCams();
}

void PolyFabScanWindow::closeEvent(QCloseEvent *event)
{
    this->blockSignals( true );
    event->accept();
}

void PolyFabScanWindow::applyState(FSState s)
{
    state = s;
    switch(state){
    case SCANNING:
        this->ui->scanningView->drawState=0;
        this->ui->scanButton->setText("Stop Scan");
        this->ui->scanButton->setIcon(QIcon(":/img/img/laser.png"));
        break;
    case POINT_CLOUD:
        this->ui->scanButton->setText("Start Scan");
        this->ui->scanButton->setIcon(QIcon(":/img/img/play_green.png"));
        //the following lines are uncommented since we do not support showing the mesh anymore but just compute and save it
        /*if(FSController::getInstance()->meshComputed){
            this->ui->toggleViewButton->setText("Show SurfaceMesh");
        }else{
            this->ui->toggleViewButton->setText("Compute SurfaceMesh");
        }*/
        break;
    case SURFACE_MESH:
        this->ui->scanButton->setText("Start Scan");
        this->ui->scanButton->setIcon(QIcon(":/img/img/play_green.png"));
        break;
    }
}


void PolyFabScanWindow::on_scanButton_clicked()
{
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

void PolyFabScanWindow::showDialog(QString dialogText)
{
    dialog->setStandardButtons(QDialogButtonBox::Ok);
    dialog->setText(dialogText);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void PolyFabScanWindow::redraw()
{
    //ui->widget->drawState = 0;
    ui->scanningView->updateGL();
}

void PolyFabScanWindow::displayLoadingFrame(QString text)
{
    ui->displayFrame->setText(text);
    ui->displayFrame->setStyleSheet("background-color: black; color: white;");

}

void PolyFabScanWindow::displayFrame(cv::Mat frame)
{
    ui->displayFrame->setText("");
    cv::resize(frame,frame,cv::Size(360,270));
    ui->displayFrame->setPixmap( PolyFabScanWindow::cvMat2qpixmap(frame) );
}

void PolyFabScanWindow::displayScanProgress()
{
    ui->estimatedTime->setText(QString::number(_currentScanY)+"  ("+QString::number(_currentScanDegree)+"/360)");
    this->repaint();
}

void PolyFabScanWindow::setScanProgressCurrentDegree(double degree)
{
    _currentScanDegree = degree;
    this->displayScanProgress();
}

void PolyFabScanWindow::setScanProgressCurrentYPos(int y)
{
    _currentScanY = y;
    this->displayScanProgress();
}

void PolyFabScanWindow::doneScanning()
{
    QSound::play("done.wav");
    ////this->ui->scanButton->setText("Start Scan");
    FSController::getInstance()->laser->disable();
    FSController::getInstance()->turntable->disable();
    ////applyState(POINT_CLOUD);
}


void PolyFabScanWindow::on_resolutionComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1.compare("Best")==0){
        //laserStepSize = 2*laser->degreesPerStep;
        FSController::getInstance()->turntableStepSize = FSController::getInstance()->turntable->degreesPerStep;
        FSController::getInstance()->yDpi = 1;
        ui->estimatedTime->setText("Time: ~190 min");
    }
    if(arg1.compare("Good")==0){
        FSController::getInstance()->turntableStepSize = 16*FSController::getInstance()->turntable->degreesPerStep;
        FSController::getInstance()->yDpi = 1;
        ui->estimatedTime->setText("Time: ~12min");
    }
    if(arg1.compare("Normal")==0){
        FSController::getInstance()->turntableStepSize = 2*16*FSController::getInstance()->turntable->degreesPerStep;
        FSController::getInstance()->yDpi = 5;
        ui->estimatedTime->setText("Time: ~6 min");
    }
    if(arg1.compare("Poor")==0){
        FSController::getInstance()->turntableStepSize = 10*16*FSController::getInstance()->turntable->degreesPerStep;
        FSController::getInstance()->yDpi = 10;
        ui->estimatedTime->setText("Time: ~1.5 min");
    }
}

void PolyFabScanWindow::on_newFile_clicked()
{
    FSController::getInstance()->model->pointCloud->clear();
    FSController::getInstance()->model->surfaceMesh.polygons.clear();
    ui->scanningView->updateGL();
    applyState(POINT_CLOUD);
    FSController::getInstance()->meshComputed=false;
}

void PolyFabScanWindow::on_loadFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File","","Files (*.pcd) ;; PLY (*.ply)");
    if(fileName.isEmpty() ) return;
    if(fileName.endsWith(".pcd", Qt::CaseInsensitive) ){
        FSController::getInstance()->model->loadPointCloudFromPCD(fileName.toStdString());
    }else if(fileName.endsWith(".ply", Qt::CaseInsensitive) ){
        FSController::getInstance()->model->loadPointCloudFromPLY(fileName.toStdString());
    }
    ui->scanningView->drawState = 0;
    ui->scanningView->updateGL();
    applyState(POINT_CLOUD);
    FSController::getInstance()->meshComputed=false;
}

void PolyFabScanWindow::on_saveFile_clicked()
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

    ui->scanningView->drawState = 0;
    ui->scanningView->updateGL();
}

void PolyFabScanWindow::on_exportFile_clicked()
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

void PolyFabScanWindow::on_selectWebcam_currentIndexChanged(int index)
{

    FSController::getInstance()->webcam->info.portName= ui->selectWebcam->itemText( index );
    FSController::getInstance()->webcam->setCamera(ui->selectWebcam->itemData(index).toByteArray());
 //   this->enumerateWebCams();
}


void PolyFabScanWindow::enumerateWebCams()
{

    if(QCamera::availableDevices().size()==0){
       QAction* a = new QAction("No camera found.", this);
       a->setEnabled(false);
       ui->selectWebcam->clear();
       ui->selectWebcam->addAction(a);
    }
    else
    {

        QCamera* camera;
        ui->selectWebcam->clear();
        int i = 0;
        foreach(const QByteArray &deviceName, QCamera::availableDevices())
        {
            QString description = camera->deviceDescription(deviceName);
            ui->selectWebcam->addItem( description, deviceName );

            if (FSController::getInstance()->webcam->info.portName.compare(description)==0) {
                ui->selectWebcam->setCurrentIndex( i );
            }
            ++i;
        }
    }

}
void PolyFabScanWindow::on_loadDefault_clicked()
{
    if(FSController::config->readConfiguration(QDir(":/settings/configuration.xml").absolutePath().toStdString()))
    {
        this->showDialog("Successfully read configuration file!");
    }
    else
    {
        this->showDialog("Configuration file not found or corrupt!");
    }
}

void PolyFabScanWindow::on_loadProfil_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Selectionner une configuration"), ".", tr("Configuration File (*.xml)"));
    if ( fileName != NULL && ! fileName.isEmpty() )
    {
        if(FSController::config->readConfiguration(fileName.toStdString()))
        {
            this->showDialog("Successfully read configuration file!");
        }
        else
        {
            this->showDialog("Configuration file not found or corrupt!");
        }
    }

}

void PolyFabScanWindow::on_saveProfil_clicked()
{

}

cv::Mat PolyFabScanWindow::frameGUITransformation(Mat &src)
{
    if ( ui->xMirror->isChecked() )
    {
            cv::Mat reversed;
            flipMat( src , reversed);
            src = reversed;
    }
    if ( ui->yMirror->isChecked() )
    {
            cv::Mat reversed;
            cv::flip(src,reversed,1);
            src = reversed;
    }
    return src;
}

void PolyFabScanWindow::on_fetchFrameButton_clicked()
{
    displayLoadingFrame("Loading...");
    FSController::getInstance()->fetchFrame();
}

void PolyFabScanWindow::on_diffImageButton_clicked()
{
    displayLoadingFrame("Loading...");
    if(FSController::getInstance()->webcam->info.portName.isEmpty()){
        FSController::getInstance()->mainwindow->showDialog("No webcam selected!");
        return;
    }
    cv::Mat shot = FSController::getInstance()->diffImage();
    cv::resize(shot,shot,cv::Size(800,600));

    displayFrame(frameGUITransformation(shot));

    /*cv::imshow("Laser Frame",shot);
    cv::waitKey(10000);
    cvDestroyWindow("Laser Frame");
    this->raise();
    this->focusWidget();
    this->setVisible(true);*/
}

void PolyFabScanWindow::on_laserImageButton_clicked()
{
    displayLoadingFrame("Loading...");
    FSController::getInstance()->laser->enable();
    FSController::getInstance()->laser->turnOff();
    QThread::msleep(200);
    cv::Mat laserOffFrame = FSController::getInstance()->webcam->getFrame();
    FSController::getInstance()->laser->turnOn();
    QThread::msleep(200);
    cv::Mat laserOnFrame = FSController::getInstance()->webcam->getFrame();
    cv::resize( laserOnFrame,laserOnFrame,cv::Size(1280,960) );
    cv::resize( laserOffFrame,laserOffFrame,cv::Size(1280,960) );

    qDebug() << "pressed";
    cv::Mat shot = FSVision::subLaser2(laserOffFrame, laserOnFrame);
    cv::resize(shot,shot,cv::Size(800,600));

    displayFrame(frameGUITransformation(shot));

    /*cv::imshow("Laser Frame",shot);
    cv::waitKey(10000);
    cvDestroyWindow("Laser Frame");
    this->raise();
    this->focusWidget();
    this->setVisible(true);*/
}

void PolyFabScanWindow::on_detectLaserButton_clicked()
{
    if(FSController::getInstance()->webcam->info.portName.isEmpty()){
        FSController::getInstance()->mainwindow->showDialog("No webcam selected!");
        return;
    }
    displayLoadingFrame("Loading...");
    FSController::getInstance()->detectLaserLine();
    cv::Mat shot = FSController::getInstance()->webcam->getFrame();
    cv::resize( shot,shot,cv::Size(1280,960) );
    shot = FSController::getInstance()->vision->drawLaserLineToFrame(shot);
    cv::resize(shot,shot,cv::Size(800,600));

    displayFrame(frameGUITransformation(shot));
    /*cv::imshow("Laser Frame",shot);
    cv::waitKey(10000);
    cvDestroyWindow("Laser Frame");
    this->raise();
    this->focusWidget();
    this->setVisible(true);*/

}

void PolyFabScanWindow::on_bluLine_pressed()
{
    unmoveLines();
    _moveBluLine = true;
}

void PolyFabScanWindow::on_bluLine_released()
{
    if ( _moveBluLine )
        setConfigFromLines();
}

void PolyFabScanWindow::on_cropTopLine_pressed()
{
    unmoveLines();
    _moveCropTopLine = true;
}

void PolyFabScanWindow::on_cropTopLine_released()
{
    if ( _moveCropTopLine )
        setConfigFromLines();
}

void PolyFabScanWindow::on_cropBotLine_pressed()
{
    unmoveLines();
    _moveCropBotLine = true;
}

void PolyFabScanWindow::on_cropBotLine_released()
{
    if ( _moveCropBotLine )
        setConfigFromLines();
}

void PolyFabScanWindow::on_laserLine_pressed()
{
    unmoveLines();
    _moveLaserLine = true;
}

void PolyFabScanWindow::on_laserLine_released()
{
    if ( _moveLaserLine )
        setConfigFromLines();
}



void PolyFabScanWindow::setConfigFromLines()
{
    FSController::config->ORIGIN_Y = (float)(ui->bluLine->y()-10.0)/ (float)270.0;
    FSController::config->LOWER_ANALYZING_FRAME_LIMIT = (280.0 - (float)(ui->cropBotLine->y()))*FSController::config->CAM_IMAGE_HEIGHT / 270.0 ;
    FSController::config->UPPER_ANALYZING_FRAME_LIMIT = (float)(ui->cropTopLine->y()-10.0)*FSController::config->CAM_IMAGE_HEIGHT / 270.0;

/*    cout<<FSController::config->LOWER_ANALYZING_FRAME_LIMIT<<"lower"<<endl;
    cout<<FSController::config->UPPER_ANALYZING_FRAME_LIMIT<<endl;*/
}

void PolyFabScanWindow::setLinesFromConfig()
{
    ui->bluLine->setGeometry(ui->bluLine->x(), (FSController::config->ORIGIN_Y *270 + 10 ), ui->bluLine->width(),ui->bluLine->height() );
    ui->cropBotLine->setGeometry(ui->cropBotLine->x(), (280- (FSController::config->LOWER_ANALYZING_FRAME_LIMIT*270/FSController::config->CAM_IMAGE_HEIGHT)),ui->cropBotLine->width(), ui->cropBotLine->height()) ;
    ui->cropTopLine->setGeometry(ui->cropTopLine->x(), (FSController::config->UPPER_ANALYZING_FRAME_LIMIT*270/FSController::config->CAM_IMAGE_HEIGHT)+10, ui->cropTopLine->width(), ui->cropTopLine->height()) ;
}

bool PolyFabScanWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseMove)
  {
      if ( _moveBluLine )
      {
          QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
          QPoint pos = mouseEvent->pos();
          pos.setX( ui->bluLine->x());
          pos.setY( pos.y()-23 );
          if ( pos.y() > 280 )
          {
              pos.setY(280);
          }
          if ( pos.y() < 10 )
          {
              pos.setY(10);
          }
          ui->bluLine->move( pos );
      }
      if ( _moveCropTopLine )
      {
          QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
          QPoint pos = mouseEvent->pos();
          pos.setX( ui->cropTopLine->x());
          pos.setY( pos.y()-23 );
          if ( pos.y() > 280 )
          {
              pos.setY(280);
          }
          if ( pos.y() < 10 )
          {
              pos.setY(10);
          }
          ui->cropTopLine->move( pos );
      }
      if ( _moveCropBotLine )
      {
          QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
          QPoint pos = mouseEvent->pos();
          pos.setX( ui->cropBotLine->x());
          pos.setY( pos.y()-23 );
          if ( pos.y() > 280 )
          {
              pos.setY(280);
          }
          if ( pos.y() < 10 )
          {
              pos.setY(10);
          }
          ui->cropBotLine->move( pos );
      }
      if ( _moveLaserLine )
      {
          QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
          QPoint pos = mouseEvent->pos();
          pos.setY( ui->laserLine->y());
          pos.setX( pos.x() );
          if ( pos.x() > 760 )
          {
              pos.setX(760);
          }
          if ( pos.x() < 400 )
          {
              pos.setX(400);
          }
          ui->laserLine->move( pos );
      }
  }
  if (event->type() == QEvent::MouseButtonRelease)
  {
      setConfigFromLines();
      unmoveLines();
  }
  return false;
}

void PolyFabScanWindow::on_editLines_clicked()
{

}

void PolyFabScanWindow::on_editLines_toggled(bool checked)
{
    ui->bluLine->setVisible(checked );
    ui->cropBotLine->setVisible(checked );
    ui->cropTopLine->setVisible(checked );
//    ui->laserLine->setVisible(checked );
    setLinesFromConfig();
}
