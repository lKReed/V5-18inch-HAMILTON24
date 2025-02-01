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
#include <math.h>

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

motor clamp = motor(PORT14, false);

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

  intake.setVelocity(70, rpm);
  conveyer.setVelocity(70, rpm);

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

void tankDrive() {
  double velocity2 = (pow(abs(Controller.Axis2.position()), 1.43) / 1000) * 100;
  if (Controller.Axis2.position() < 0)
    velocity2 *= -1;

  double velocity3 = (pow(abs(Controller.Axis3.position()), 1.43) / 1000) * 100;
  if (Controller.Axis3.position() < 0)
    velocity3 *= -1;

  leftDrive.spin(forward, velocity3, percent);  // Left Drive Code
  rightDrive.spin(forward, velocity2, percent);  // Right Drive Code
}

void usercontrol(void) {
  // User control code here, inside the loop
  std::cout << "\nIN TELEOP\n\n";

  while (1) {

    // Drive Code
    tankDrive();

    // Intake + Conveyer
    if (Controller.ButtonL1.pressing()) {
      std::cout << "Intake In\n";
      intake.spin(forward);
      conveyer.spin(forward);
    }
    else if (Controller.ButtonL2.pressing()) {
      std::cout << "Intake Out\n";
      intake.spin(reverse);
      conveyer.spin(reverse);
    }
    else {
      intake.stop();
    }

  // Clamp
    if (Controller.ButtonR1.pressing()) {
      std::cout << "Clamp Down\n";
      clamp.spin(reverse);
    }
    else if (Controller.ButtonR2.pressing()) {
      std::cout << "Clamp Up\n";
      clamp.spin(forward);
    }

  // Forward
  if (Controller.ButtonUp.pressing()) {
    std::cout << "Forward\n";
    leftDrive.spin(forward);
    rightDrive.spin(forward);
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
