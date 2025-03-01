/**
 * \file opcontrol.cpp
 *
 * Contains user implemented opcontrol. User must use the
 * parameters to the opcontrol function when referencing
 * the master V5 controller or partner V5 controller.
 */

 #include "api.h"
#include "pros/llemu.hpp"
 #include "pros/misc.h"
 #include "pros/motors.h"
 #include "pros/rtos.hpp"
 #include "umbc.h"
 #include "umbc/robot.hpp"
  
 #include <cstdint>
 #include <time.h>
 #include <vector>
   
 using namespace pros;
 using namespace umbc;
 using namespace std;
  
  
 #define MOTOR_RED_GEAR_MULTIPLIER       100
 #define MOTOR_GREEN_GEAR_MULTIPLIER     200
 #define MOTOR_BLUE_GEAR_MULTIPLIER      600
 #define MOTOR_REVERSE                   true
 #define REVERSED(port)                  -port
 
 
 //left and right are relative to the robot's left and right
 
 //DOUBLE CHECK MOTORS!!!!!
  
 //ports for left motors(blue)
 #define LEFT_MOTOR_ONE          16 //L1
 #define LEFT_MOTOR_TWO          17 //L2
 #define LEFT_MOTOR_THREE        REVERSED(18) //L3
 #define LEFT_MOTOR_FOUR         19 //L4
 #define LEFT_MOTOR_FIVE         20 //L5
  
 //ports for right motors(blue)
 #define RIGHT_MOTOR_ONE         5 //R1
 #define RIGHT_MOTOR_TWO         6 //R2
 #define RIGHT_MOTOR_THREE       REVERSED(8) //R3
 #define RIGHT_MOTOR_FOUR        9 //R4
 #define RIGHT_MOTOR_FIVE        10 //R5
 
 //DID NOT CHECK MOTORS FOR REVERESE!!!! NEED TO
  
 //left lift motor ports(red)
 #define LIFT_MOTOR_TOP_LEFT      -12 //LTL
 #define LIFT_MOTOR_BOTTOM_LEFT   11 //LBL
 
 //right lift motor ports(red)
 #define LIFT_MOTOR_TOP_RIGHT     -3 //LTR
 #define LIFT_MOTOR_BOTTOM_RIGHT  4 //LBR
 
 //claw motor port (red)
 #define CLAW_MOTOR               13 //CW
 
 //claw wrist port (red)
 #define CLAW_WRIST_MOTOR_LEFT    -14 //CWR
 #define CLAW_WRIST_MOTOR_RIGHT   15 //CWL
 
 // not sure abt this code... 
 // //limits
 // #define MAX_LIFT_POS             1
 
 // //Encoders values (measured to ticks/rev)
 // #define RED_MOTOR_ENCODER      900
 
  
 
 void umbc::Robot::opcontrol() {
  
 
     // nice names for controllers (do not edit)
     umbc::Controller* controller_master = this->controller_master;
     umbc::Controller* controller_partner = this->controller_partner;
 
     //starting position of motors
 
  
     // initialize motors and sensors
  
     std::vector<int8_t> left_motors, right_motors, lift_motors, claw_motor, claw_wrist_motors;
  
  
     //left drive
     left_motors.assign({LEFT_MOTOR_ONE, LEFT_MOTOR_TWO, LEFT_MOTOR_THREE, LEFT_MOTOR_FOUR, LEFT_MOTOR_FIVE});
  
     MotorGroup drive_left (left_motors);
     drive_left.set_brake_modes(E_MOTOR_BRAKE_COAST);
     drive_left.set_gearing(E_MOTOR_GEAR_BLUE);
  
     //right drive
     right_motors.assign({RIGHT_MOTOR_ONE, RIGHT_MOTOR_TWO, RIGHT_MOTOR_THREE, RIGHT_MOTOR_FOUR, RIGHT_MOTOR_FIVE});
      
     MotorGroup drive_right(right_motors); 
     drive_right.set_brake_modes(E_MOTOR_BRAKE_COAST);
     drive_right.set_gearing(E_MOTOR_GEAR_BLUE);
 
 
     //lift motors
     lift_motors.assign({LIFT_MOTOR_BOTTOM_LEFT, LIFT_MOTOR_BOTTOM_RIGHT, LIFT_MOTOR_TOP_LEFT, LIFT_MOTOR_TOP_RIGHT});
  
     MotorGroup lift(lift_motors);
     lift.set_brake_modes(E_MOTOR_BRAKE_HOLD);
     lift.set_gearing(E_MOTOR_GEAR_RED);
 
     //claw motor
     claw_motor.assign({CLAW_MOTOR});
 
     MotorGroup claw(claw_motor);
     claw.set_brake_modes(E_MOTOR_BRAKE_COAST);
     claw.set_gearing(E_MOTOR_GEAR_RED);
 
     //claw wrist
     claw_wrist_motors.assign({CLAW_WRIST_MOTOR_LEFT, CLAW_WRIST_MOTOR_RIGHT});
 
     MotorGroup claw_wrist (claw_wrist_motors);
     claw_wrist.set_gearing(E_MOTOR_GEAR_GREEN);
     claw_wrist.set_brake_modes(E_MOTOR_BRAKE_HOLD);
 
     // if(umbc::Robot::get_mode()){
     //     lift.move_relative(lift.get_position()+20, 25);
     //     claw_wrist.move_relative(claw_wrist.get_position()+10, 5);
     //     pros::delay(5000);
     // }
 
     //NOT YET TESTED
     // lift.move_relative(lift.get_position()+20, 25);
     // claw_wrist.move_relative(claw_wrist.get_position()+10, 5);
     // pros::delay(5000);
     //moveArmAllTheWayUp
 
 
 
 
 
     while(1) {
  
     // implement opcontrols
         
 
     //getting the values from the controller
     int32_t arcade_y = controller_master->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
     int32_t arcade_x = controller_master->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
 
     int32_t drive_left_velocity = (int32_t)(((double)(arcade_y + arcade_x) / (double)E_CONTROLLER_ANALOG_MAX)
                                     * MOTOR_BLUE_GEAR_MULTIPLIER);
 
     int32_t drive_right_velocity = (int32_t)(((double)(arcade_y - arcade_x) / (double)E_CONTROLLER_ANALOG_MAX)
                                     * MOTOR_BLUE_GEAR_MULTIPLIER);
 
     //setting drive velocity
     drive_left.move_velocity(drive_left_velocity);
     drive_right.move_velocity(drive_right_velocity);
 
         
     
     //lift up
     if (controller_master->get_digital(E_CONTROLLER_DIGITAL_L1)){
         lift.move_velocity(MOTOR_RED_GEAR_MULTIPLIER*0.5);
     }
     //down
     else if(controller_master->get_digital(E_CONTROLLER_DIGITAL_R1)){
         lift.move_velocity(-MOTOR_RED_GEAR_MULTIPLIER*0.5);
     }
     else{
         lift.brake();
     }
     pros::lcd::print(0, "%f", lift.get_positions()[0]);     
 
     //claw
     
     if(controller_master -> get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
         claw.move_velocity(MOTOR_RED_GEAR_MULTIPLIER);
     } else if (controller_master -> get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
         claw.move_velocity(-MOTOR_RED_GEAR_MULTIPLIER);
     }
     else{
        claw.brake();
     }
     
 
     //claw wrist
     
     if(controller_master -> get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
         claw_wrist.move_velocity(MOTOR_GREEN_GEAR_MULTIPLIER);
     } else if (controller_master -> get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
         claw_wrist.move_velocity(-MOTOR_GREEN_GEAR_MULTIPLIER);
     }
     else{
        claw_wrist.brake();
     }
     
    
 
 
     // required loop delay (do not edit)
     pros::Task::delay(this->opcontrol_delay_ms);
 }
 }