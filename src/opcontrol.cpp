/**
 * \file opcontrol.cpp
 *
 * Contains user implemented opcontrol. User must use the
 * parameters to the opcontrol function when referencing
 * the master V5 controller or partner V5 controller.
 */

 #include "api.h"
#include "pros/misc.h"
#include "pros/motors.h"
 #include "umbc.h"
 
 #include <cstdint>
 #include <vector>
  
 using namespace pros;
 using namespace umbc;
 using namespace std;
 
 
 #define MOTOR_RED_GEAR_MULTIPLIER       100
 #define MOTOR_GREEN_GEAR_MULTIPLIER     200
 #define MOTOR_BLUE_GEAR_MULTIPLIER      600
 #define MOTOR_REVERSE                   true        //may or may not use idk yet

 
 //ports for left motors
 #define LEFT_MOTOR_ONE   1
 #define LEFT_MOTOR_TWO   2
 #define LEFT_MOTOR_THREE -3
 #define LEFT_MOTOR_FOUR 4
 #define LEFT_MOTOR_FIVE 5
 
 //ports for right motors
 #define RIGHT_MOTOR_ONE 11
 #define RIGHT_MOTOR_TWO 12
 #define RIGHT_MOTOR_THREE -13
 #define RIGHT_MOTOR_FOUR 14
 #define RIGHT_MOTOR_FIVE 15
 
 
 
 void umbc::Robot::opcontrol() {
 
     // nice names for controllers (do not edit)
     umbc::Controller* controller_master = this->controller_master;
     umbc::Controller* controller_partner = this->controller_partner;
 
     // initialize motors and sensors

 
     std::vector<int8_t> left_motors, right_motors, intake_wheels, intake_ramp, grabber_motor, lift_motors;
 
 
     //left drive
     left_motors.assign({LEFT_MOTOR_ONE, LEFT_MOTOR_TWO, LEFT_MOTOR_THREE, LEFT_MOTOR_FOUR, LEFT_MOTOR_FIVE});
 
     MotorGroup drive_left (left_motors);
     drive_left.set_brake_mode(E_MOTOR_BRAKE_COAST);
     drive_left.set_gearing(E_MOTOR_GEAR_BLUE);         //maybe blue
 
     //right drive
     right_motors.assign({RIGHT_MOTOR_ONE, RIGHT_MOTOR_TWO, RIGHT_MOTOR_THREE, RIGHT_MOTOR_FOUR, RIGHT_MOTOR_FIVE});
     
     MotorGroup drive_right(right_motors); 
     drive_right.set_brake_mode(E_MOTOR_BRAKE_COAST);
     drive_right.set_gearing(E_MOTOR_GEAR_BLUE);         //maybe blue

 
         // required loop delay (do not edit)
         pros::Task::delay(this->opcontrol_delay_ms);
     }