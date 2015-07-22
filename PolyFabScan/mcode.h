#ifndef MCODE_H
#define MCODE_H

namespace MCode {
/**
 * @brief The MCodeFlags enum Base value for Polybox M-Code.
 */
enum MCodeFlags
{
    ASK_CNC_UPDATE=600, ASK_PRINTER_UPDATE=630, ASK_LABVIEW_UPDATE=620, ASK_SCANNER_UPDATE=610
};

#define MCODE_RESET_LINE_NUMBER                 110
#define MCODE_GET_ENDSTOP_STATUS                119

/**      CNC MCODE    **/

#define MCODE_CNC_TOOL_PLUGGED                  611
#define MCODE_CNC_LUBRICANT_PLUGGED             612
#define MCODE_CNC_LUBRICANT_LEVEL               613
#define MCODE_CNC_VACUUM_DETECTED               614
#define MCODE_CNC_RECYCLE_STATE                 615
#define MCODE_CNC_VACUUM_STATE                  616
#define MCODE_CNC_LUBRICANT_STATE               617

/**      SCANNER MCODE    **/

#define MCODE_SCANNER_TURNTABLE_PLUGGED         621
#define MCODE_SCANNER_GET_LASER_PLUGGED         622
#define MCODE_SCANNER_SET_TURNTABLE_STATE       624
#define MCODE_SCANNER_TURN_ROT_X_DEG            625
#define MCODE_SCANNER_SET_CLOCK_DIRECTION_TABLE 627
#define MCODE_SCANNER_SET_LASER_STATE           629
#define MCODE_SCANNER_SET_LASER_ROTATION_STATE  631
#define MCODE_SCANNER_LASER_TURN_X_STEPS        632
#define MCODE_SCANNER_SET_CLOCK_DIRECTION_LASER 634

/**      LABVIEW MCODE    **/

#define MCODE_LABVIEW_GET_STATUS                640
#define MCODE_LABVIEW_SET_STATUS                641
#define MCODE_LABVIEW_GET_GLOBAL_RGB            642
#define MCODE_LABVIEW_SET_GLOBAL_RGB            643
#define MCODE_LABVIEW_GET_GLOBAL_I              644
#define MCODE_LABVIEW_SET_GLOBAL_I              645
#define MCODE_LABVIEW_GET_FACE_I                646
#define MCODE_LABVIEW_SET_FACE_I                647
#define MCODE_LABVIEW_LED_PLUGGED               648
#define MCODE_LABVIEW_SET_CONTROLLER            649
#define MCODE_LABVIEW_GET_FACE_RGB              650
#define MCODE_LABVIEW_SET_FACE_RGB              651

/**      PRINTER MCODE    **/

#define MCODE_PRINTER_GET_STATUS                660
#define MCODE_PRINTER_SET_STATUS                661
#define MCODE_PRINTER_TOOL_PLUGGED              662
#define MCODE_PRINTER_WIRE_DETECTED             663
#define MCODE_PRINTER_BED_PLUGGED               664
#define MCODE_PRINTER_PEL_PLUGGED               665
#define MCODE_PRINTER_SET_FAN_SPEED             667
#define MCODE_PRINTER_GET_COLDEND_STATUS        668
#define MCODE_PRINTER_GET_BED_TEMP_C            669
#define MCODE_PRINTER_SET_BED_TEMP              670
#define MCODE_PRINTER_SET_BED_TEMP_FAST         671
#define MCODE_PRINTER_SET_ALL_FAN               672
#define MCODE_PRINTER_GET_CHAMBER_TEMP          673
#define MCODE_PRINTER_SET_CHAMBER_TEMP          674
#define MCODE_PRINTER_SET_CHAMBER_FAST          675
#define MCODE_PRINTER_GET_BUSE_CLOGGED          676
#define MCODE_PRINTER_GET_CHAMBER_TEMP_ALL      677
#define MCODE_PRINTER_GET_COOLER_BOX_STATUS     679

/**      GLOBAL MCODE    **/

#define MCODE_GLOBAL_GET_STATUS                 680
#define MCODE_GLOBAL_SET_STATUS                 681
#define MCODE_GLOBAL_GET_BOX_OPEN               682
#define MCODE_GLOBAL_GET_ARDUINO_DETECTED       683
#define MCODE_GLOBAL_GET_IC_OPEN                684
#define MCODE_GLOBAL_GET_PREASI                 685
#define MCODE_GLOBAL_GET_IC_TEMP                686
#define MCODE_GLOBAL_GET_POWER_STATUS           687
#define MCODE_GLOBAL_GET_TOOL_STATE             688
#define MCODE_GLOBAL_SET_TOOL_STATE             689
#define MCODE_GLOBAL_SET_POWER_STATUS           690
#define MCODE_GLOBAL_SET_PAUSE                  691

#define MCODE_GLOBAL_GET_GYRO_ABSOLU            693
#define MCODE_GLOBAL_GET_GYRO_RELATIF           694
#define MCODE_GLOBAL_SET_GYRO_OFFSET            695
#define MCODE_GLOBAL_MOVE_MOTOR_ANGLE           696

#define MCODE_PING_PONG                         703
#define MCODE_RESET_SLAVES                      704
#define MCODE_END_CONNECTION                    705
#define MCODE_START_CONNECTION                  706
#define MCODE_SEND_GCODE                        710


#define GCODE_MOVE_AXIS_RAPID                   0
#define GCODE_MOVE_AXIS_LINEAR                  1

#define GCODE_HOMING                            28
#define GCODE_SINGLE_ZPROBE                     30
#define GCODE_ZPROBE_CAPTOR_VALUE               31

#define GCODE_ATX_POWER_ON                      80
#define GCODE_ATX_POWER_OFF                     81
#define GCODE_DISABLE_MOTOR                     84

#define GCODE_SET_ABSOLUTE_POSITIONING          90
#define GCODE_SET_RELATIVE_POSITIONING          91

#define GCODE_MOTOR_GOTO                        201
#define GCODE_MOTOR_SET_ORIGIN                  202
#define GCODE_MOTOR_GET_POS                     203
#define GCODE_MOTOR_SET_ENABLE                  204

#define MCODE_ERROR_COM_SLAVE_SIDE              666

}

#endif // MCODE_H
