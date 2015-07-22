#include "fsturntable.h"
#include "fscontroller.h"

FSTurntable::FSTurntable()
{
    degreesPerStep = 360.0f/200.0f/16.0f; //the size of a microstep
    direction = FS_DIRECTION_CW;
    rotation = FSMakePoint(0.0f, 0.0f, 0.0f);
}

void FSTurntable::turnNumberOfSteps(unsigned int steps)
{
    unsigned char size = steps/256*2;
    char c[size];
    unsigned int s = steps;
    for(unsigned int i=0; i<=steps/256; i++){
        if(s<256)
        {
            ComModule::getInstance()->sendMCode( s%256 );
        }
        else
        {
            ComModule::getInstance()->sendMCode( 255 );
            s-=255;
        }
    }

}

void FSTurntable::turnNumberOfDegrees(double degrees)
{
    //int steps = (int)(degrees/degreesPerStep);
    MovementModule::setCurrentPosRotByMask(RZAxis, 0);
    if(direction==FS_DIRECTION_CW){

      rotation.y -= degrees;
    }else if(direction==FS_DIRECTION_CCW){
      rotation.y += degrees;
    }
    MovementModule::moveRotByMaskRelative( RZAxis, degrees, direction );
  //  turnNumberOfSteps(steps);
}

void FSTurntable::setDirection(FSDirection d)
{
    direction = d;
    /*if ( d==FS_DIRECTION_CW )
    {
       ComModule::getInstance()->sendMCode( MC_SET_TABLE_DIRECTION_CW );
    }
    else
    {
        ComModule::getInstance()->sendMCode( MC_SET_TABLE_DIRECTION_CCW );
    }*/
}

void FSTurntable::toggleDirection(){
    FSDirection d = (direction == FS_DIRECTION_CW)?FS_DIRECTION_CCW:FS_DIRECTION_CW;
    setDirection(d);
}

void FSTurntable::selectStepper()
{
    /*char c[2];
    c[0] = MC_SELECT_STEPPER;
    c[1] = MC_TURNTABLE_STEPPER;
    ComModule::getInstance()->sendMCode(c);*/
}

void FSTurntable::enable(void)
{
    MovementModule::setEnableRotByMask(RZAxis, true );
}

void FSTurntable::disable(void)
{
    MovementModule::setEnableRotByMask(RZAxis, false );
}

void FSTurntable::setRotation(FSPoint r)
{
    rotation = r;
}

FSPoint FSTurntable::getRotation()
{
    return rotation;
}
