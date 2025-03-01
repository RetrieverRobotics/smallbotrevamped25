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
 #define MOTOR_REVERSE                   true


 //left and right are relative to the robot's left and right
 
 //ports for left motors(blue)
 #define LEFT_MOTOR_ONE         1
 #define LEFT_MOTOR_TWO         2
 #define LEFT_MOTOR_THREE      -3
 #define LEFT_MOTOR_FOUR        4
 #define LEFT_MOTOR_FIVE        5
 
 //ports for right motors(blue)
 #define RIGHT_MOTOR_ONE        11
 #define RIGHT_MOTOR_TWO        12
 #define RIGHT_MOTOR_THREE     -13
 #define RIGHT_MOTOR_FOUR       14
 #define RIGHT_MOTOR_FIVE       15
 
 //left lift motor ports(red)
 #define LIFT_MOTOR_TOP_LEFT        7  
 #define LIFT_MOTOR_BOTTOM_LEFT     7

 //right lift motor ports(red)
 #define LIFT_MOTOR_TOP_RIGHT       7
 #define LIFT_MOTOR_BOTTOM_RIGHT    7

 //claw motor port(s)(red)
 #define CLAW_MOTOR             7
 
 void umbc::Robot::opcontrol() {
 
     // nice names for controllers (do not edit)
     umbc::Controller* controller_master = this->controller_master;
     umbc::Controller* controller_partner = this->controller_partner;
 
     // initialize motors and sensors

 
     std::vector<int8_t> left_motors, right_motors, lift_motors, claw_motor;
 
 
     //left drive
     left_motors.assign({LEFT_MOTOR_ONE, LEFT_MOTOR_TWO, LEFT_MOTOR_THREE, LEFT_MOTOR_FOUR, LEFT_MOTOR_FIVE});
 
     MotorGroup drive_left (left_motors);
     drive_left.set_brake_mode(E_MOTOR_BRAKE_COAST);
     drive_left.set_gearing(E_MOTOR_GEAR_BLUE);
 
     //right drive
     right_motors.assign({RIGHT_MOTOR_ONE, RIGHT_MOTOR_TWO, RIGHT_MOTOR_THREE, RIGHT_MOTOR_FOUR, RIGHT_MOTOR_FIVE});
     
     MotorGroup drive_right(right_motors); 
     drive_right.set_brake_mode(E_MOTOR_BRAKE_COAST);
     drive_right.set_gearing(E_MOTOR_GEAR_BLUE);


     //lift motors
     lift_motors.assign({LIFT_MOTOR_BOTTOM_LEFT, LIFT_MOTOR_BOTTOM_RIGHT, LIFT_MOTOR_TOP_LEFT, LIFT_MOTOR_TOP_RIGHT});
 
     MotorGroup lift(lift_motors);
     lift.set_brake_mode(E_MOTOR_BRAKE_COAST);
     lift.set_gearing(E_MOTOR_GEAR_RED);

     //claw motor
     claw_motor.assign({CLAW_MOTOR});

     MotorGroup claw(claw_motor);
     claw.set_brake_mode(E_MOTOR_BRAKE_COAST);
     claw.set_gearing(E_MOTOR_GEAR_RED);





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

        
        /*
        //lift up
        if (controller_master->get_digital(E_CONTROLLER_DIGITAL_R1)){
            lift.move_velocity(MOTOR_RED_GEAR_MULTIPLIER);
        }
        //down
        else if(controller_master->get_digital(E_CONTROLLER_DIGITAL_R2)){
            lift.move_velocity(-MOTOR_RED_GEAR_MULTIPLIER);
        }
        else{
            lift.brake();
        }
        */

        //claw


        // required loop delay (do not edit)
        pros::Task::delay(this->opcontrol_delay_ms);
    }
}
