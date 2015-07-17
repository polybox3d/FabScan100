#include "fslaser.h"
#include "fscontroller.h"
#include "fsserial.h"
#include <math.h>

FSLaser::FSLaser()
{
    laserPointPosition = FSMakePoint(14.0f, 0.0f, 0.0f);
    degreesPerStep = 360.0f/200.0f/16.0f; //the size of a microstep
    direction = FS_DIRECTION_CW;
    rotation = FSMakePoint(0.0f, 0.0f, 0.0f);
    position = FSMakePoint(
                FSController::config->LASER_POS_X,
                FSController::config->LASER_POS_Y,
                FSController::config->LASER_POS_Z);
    //FSController::getInstance()->controlPanel->setLaserSwipeMaxEditText(rotation.y);
}

void FSLaser::selectStepper()
{
    /*char c[2];
    c[0] = MC_SELECT_STEPPER;
    c[1] = MC_LASER_STEPPER;
    ComModule::getInstance()->sendMCode(c);*/
    qDebug()<<"The fuk useless";
}

void FSLaser::turnOn()
{
    ComModule::getInstance()->sendMCode(MC_TURN_LASER_ON);
}

void FSLaser::turnOff()
{
    ComModule::getInstance()->sendMCode(MC_TURN_LASER_OFF);
}

void FSLaser::turnNumberOfSteps(unsigned int steps)
{
    //this->selectStepper();
    qDebug()<<"Steps: " << steps;
    unsigned char size = steps/256*2;
    char c[size];
    unsigned int s = steps;
    for(unsigned int i=0; i<=(steps/256); i++){
        if(s<256){
            ComModule::getInstance()->sendMCode( MC_ROTATE_LASER+(s%256) );
        }else{
            ComModule::getInstance()->sendMCode( MC_ROTATE_LASER+(255) );
            s-=255;
        }
    }

    laserPointPosition.x = position.x - tan(rotation.y*M_PI/180)*position.z;
    qDebug() << "LaserPositionX"<< laserPointPosition.x;
}

void FSLaser::turnNumberOfDegrees(double degrees)
{
    int steps = (int)(degrees/degreesPerStep);
    //make sure to correctly update rotation in degrees, not steps
    degrees = (double)steps*(double)degreesPerStep;
    qDebug()<<"Steps"<<steps<<"Degrees"<<degrees;
    if(direction==FS_DIRECTION_CCW){
      rotation.y += degrees;
    }else if(direction==FS_DIRECTION_CW){
      rotation.y -= degrees;
    }
    qDebug()<<"computed number of steps";
    turnNumberOfSteps(steps);
    FSController::getInstance()->controlPanel->setLaserAngleText(rotation.y);
}

void FSLaser::turnToAngle(float degrees)
{
    double degreesToTurn = this->rotation.y - degrees;
    if(degreesToTurn<0){
        setDirection(FS_DIRECTION_CCW);
        turnNumberOfDegrees(degreesToTurn*-1);
    }else{
        setDirection(FS_DIRECTION_CW);
        turnNumberOfDegrees(degreesToTurn);
    }
}

void FSLaser::setDirection(FSDirection d)
{
    direction = d;
    if ( d==FS_DIRECTION_CW )
    {
       ComModule::getInstance()->sendMCode( MC_SET_LASER_DIRECTION_CW );
    }
    else
    {
        ComModule::getInstance()->sendMCode( MC_SET_LASER_DIRECTION_CCW );
    }

}

void FSLaser::toggleDirection(){
    FSDirection d = (direction == FS_DIRECTION_CW)?FS_DIRECTION_CCW:FS_DIRECTION_CW;
    setDirection(d);
}

void FSLaser::enable(void)
{
    ComModule::getInstance()->sendMCode(MC_TURN_LASER_STEPPER_ON);
}

void FSLaser::disable(void)
{
    ComModule::getInstance()->sendMCode(MC_TURN_LASER_STEPPER_OFF);
}

void FSLaser::setLaserPointPosition(FSPoint p)
{
    laserPointPosition = p;
    double b = position.x - laserPointPosition.x;
    double a = position.z - laserPointPosition.z;
    rotation.y = atan(b/a)*180.0/M_PI;
    qDebug() << "Current laser angle: "<<rotation.y;
    FSController::getInstance()->controlPanel->setLaserAngleText(rotation.y);
}

FSPoint FSLaser::getLaserPointPosition(void)
{
    return laserPointPosition;
}

FSPoint FSLaser::getPosition()
{
    return position;
}

FSPoint FSLaser::getRotation()
{
    return rotation;
}
