#include "main.h"
#include "squiggles.hpp"
#include <cstdio>
#include <memory>

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
  // static bool pressed = false;
  // pressed = !pressed;
  // if (pressed) {
  //   pros::lcd::set_text(2, "I was pressed!");
  // } else {
  //   pros::lcd::clear_line(2);
  // }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();
  // pros::lcd::set_text(1, "Hello PROS User!");

  // pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  // contstraint values
  const double MAX_VEL = 1.0;      // in meters per second
  const double MAX_ACCEL = 2.0;    // in meters per second per second
  const double MAX_JERK = 10.0;    // in meters per second per second per second
  const double ROBOT_WIDTH = 0.32; // in meters

  squiggles::Constraints constraints =
      squiggles::Constraints(MAX_VEL, MAX_ACCEL, MAX_JERK);

  // Generate path using spline
  squiggles::SplineGenerator generator = squiggles::SplineGenerator(
      constraints,
      std::make_shared<squiggles::TankModel>(ROBOT_WIDTH, constraints));
  std::vector<squiggles::ProfilePoint> path = generator.generate(
      {squiggles::Pose(0.0, 0.0, 0.0), squiggles::Pose(1.0, 1.0,0.0)});

  // controller for left tankdrive set
  // auto leftController =
  //     AsyncVelControllerBuilder()
  //         .withMotor({-1, -3})
  //         .withSensor(okapi::IntegratedEncoder(1))
  //         .withGearset({okapi::AbstractMotor::gearset::blue, (84.0 / 36.0)})
  //         .withGains({0.0001, 0., 0., 0.})
  //         .withVelMath(okapi::VelMathFactory::createPtr(okapi::imev5BlueTPR *
  //                                                       (84.0 / 36.0)))
  //         .notParentedToCurrentTask()
  //         .build();

  // controller for right tankdrive set
  // auto rightController =
  //     AsyncVelControllerBuilder()
  //         .withMotor({2, 4})
  //         .withSensor(okapi::IntegratedEncoder(2))
  //         .withGearset({okapi::AbstractMotor::gearset::blue, (84.0 / 36.0)})
  //         .withGains({0.0001, 0., 0., 0.})
  //         .withVelMath(okapi::VelMathFactory::createPtr(okapi::imev5BlueTPR *
  //                                                       (84.0 / 36.0)))
  //         .notParentedToCurrentTask()
  //         .build();


  pros::MotorGroup leftMtrs({-1,-3});
  pros::MotorGroup rightMtrs({2,4});
  // iterate through all points in the path and update controller velocities per
  // time
  double currTime = 0.0;
  for (squiggles::ProfilePoint point : path) {
    lcd::print(0, "%s\n", point.to_csv());
    lcd::print(1, "%d\n",  point.wheel_velocities.size());
    //leftController->setTarget(point.wheel_velocities[0]);
    //rightController->setTarget(point.wheel_velocities[1]);
    leftMtrs.move_velocity(point.wheel_velocities[0]*157);
    rightMtrs.move_velocity(point.wheel_velocities[1]*157);
    pros::delay((point.time - currTime) * 1000);
    currTime = point.time;
  }

  // Stop
  // leftController->setTarget(0);
  // rightController->setTarget(0);
  leftMtrs.move_velocity(0);
  rightMtrs.move_velocity(0);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  /*
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    pros::Motor left_mtr(1);
    pros::Motor right_mtr(2);

    while (true) {
      pros::lcd::print(0, "%d %d %d",
                       (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                       (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                       (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
      int left = master.get_analog(ANALOG_LEFT_Y);
      int right = master.get_analog(ANALOG_RIGHT_Y);

      left_mtr = left;
      right_mtr = right;

      pros::delay(20);
    }
    */
}
