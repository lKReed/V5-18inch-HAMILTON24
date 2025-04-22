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

motor leftFront = motor(PORT12, true);
motor leftBack = motor(PORT19, true);
motor_group leftDrive = motor_group(leftFront, leftBack);

motor rightFront = motor(PORT13, false);
motor rightBack = motor(PORT11, false);
motor_group rightDrive = motor_group(rightFront, rightBack);

motor intake = motor(PORT2, true);
motor conveyer = motor(PORT20, false);
// motor pusher = motor(PORT14, false);

inertial Inertial = inertial(PORT14);

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

  intake.setVelocity(95, rpm);
  conveyer.setVelocity(95, rpm);
  // pusher.setVelocity(60, rpm);
  Inertial.calibrate();

  return;
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

// Axis 3 forward/backwards
// Axis 1 left/right
void alexDrive() {
  // velocity formula for exponential speed instead of linear speed
  double sideVelocity = (pow(abs(Controller.Axis3.position()), 1.4) / 1000) * 100;
  if (Controller.Axis3.position() < 0)
    sideVelocity *= -1;

  double leftVelocity = sideVelocity;
  double rightVelocity = sideVelocity;

  double forwardVelocity = (pow(abs(Controller.Axis1.position()), 1.4) / 1000) * 100;
  if (Controller.Axis1.position() > 0)
    forwardVelocity *= -1;

  // subtract (and add) the value of left/right velocity from the opposite wheel to turn
  if (forwardVelocity > 0) {
    rightVelocity -= abs(forwardVelocity);
    leftVelocity += abs(forwardVelocity);
  }
  else if (forwardVelocity < 0) {
    leftVelocity -= abs(forwardVelocity);
    rightVelocity += abs(forwardVelocity);
  }

  leftDrive.spin(forward, leftVelocity, percent);
  rightDrive.spin(forward, rightVelocity, percent);
}

void usercontrol(void) {
  // User control code here, inside the loop
  std::cout << "\nIN TELEOP\n\n";

  while (1) {

    // Drive Code
    alexDrive();

    // Intake + Conveyer
    if (Controller.ButtonR1.pressing()) {
      std::cout << "Intake In\n";
      intake.spin(forward);
      conveyer.spin(forward);
    }
    else if (Controller.ButtonR2.pressing()) {
      std::cout << "Intake Out\n";
      intake.spin(reverse);
      conveyer.spin(reverse);
    }
    else {
      intake.stop();
    }

  // Pusher
    // if (Controller.ButtonL1.pressing()) {
    //   std::cout << "Pusher Down\n";
    //   pusher.spin(reverse);
    // }
    // else if (Controller.ButtonL2.pressing()) {
    //   std::cout << "Pusher Up\n";
    //   pusher.spin(forward);
    // }
    // else {
    //   pusher.stop();
    // }

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
      if (Controller.ButtonR1.pressing() == false && (Controller.ButtonR2.pressing() == false)) {
        conveyer.stop();
      }
    }

    double test = Inertial.rotation(degrees);
    std::cout << test << std::endl;

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
