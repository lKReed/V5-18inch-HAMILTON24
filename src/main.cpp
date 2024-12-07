/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       lkree                                                     */
/*    Created:      12/6/2024, 6:25:27 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include <iostream>

using namespace vex;

// A global instance of competition
competition Competition;

// Motor and Controller definitions
controller Controller = controller();

motor leftFront = motor(PORT12, false);
motor leftBack = motor(PORT19, false);
motor_group leftDrive = motor_group(leftFront, leftBack);

motor rightFront = motor(PORT13, true);
motor rightBack = motor(PORT11, true);
motor_group rightDrive = motor_group(rightFront, rightBack);

motor intake = motor(PORT2, true);

motor conveyer = motor(PORT20, false);

motor clamp = motor(PORT5, false);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  std::cout << "IN PRE-AUTO\n";

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  std::cout << "IN AUTO\n";

  leftDrive.spinFor(forward, 30, degrees);
  rightDrive.spinFor(forward, 30, degrees);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  std::cout << "\nIN TELEOP\n\n";

  while (1) {

    // Basic tank drive
    leftDrive.spin(forward, Controller.Axis3.position(), percent);  // Left Drive Code
    rightDrive.spin(forward, Controller.Axis2.position(), percent); // Right Drive Code

    // Intake + Conveyer
    if (Controller.ButtonL2.pressing()) {
      std::cout << "Intake Out\n";
      intake.spin(forward);
      conveyer.spin(forward);
    }
    else if (Controller.ButtonL1.pressing()) {
      std::cout << "Intake In\n";
      intake.spin(reverse);
      conveyer.spin(reverse);
    }
    else {
      intake.stop();
    }

    // Conveyer
    if (Controller.ButtonB.pressing()) {
      std::cout << "Conveyer Down";
      conveyer.spin(forward);
    }
    else if (Controller.ButtonA.pressing()) {
      std::cout << "Conveyer Up";
      conveyer.spin(reverse);
    }
    else {
      if (Controller.ButtonL1.pressing() == false && (Controller.ButtonL2.pressing() == false)) {
        conveyer.stop();
      }
    }

    // Emergency Stop
    if (Controller.ButtonX.pressing()) {
      std::cout << "EMERGENCY STOP: RESET REQUIRED\n";
      return;
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
