#include "AutoSequence.h"

#include "WindowComparator.h"
#include "PressureSensor.h"
#include "Thermocouples.h"
#include "AutoSeqLogger.h"
#include "SDCard.h"
#include "Safety.h"
#include "Loader.h"
#include "Router.h"

#define LOG_INTERVAL_US 1000

namespace AutoSequence {

// gets sensor data from PTs and TCs and performs safety checks
Sensor_Data get_sensor_data() {
  Sensor_Data sd;

  sd.ox.valve_upstream_pressure = PT::lox_valve_upstream.getPressure();
  sd.ox.valve_downstream_pressure = PT::lox_valve_downstream.getPressure();
  sd.ox.venturi_differential_pressure = PT::lox_venturi_differential.getPressure();
  sd.ox.valve_temperature = TC::lox_valve_temperature.getTemperature_Kelvin();
  sd.ox.venturi_temperature = TC::lox_venturi_temperature.getTemperature_Kelvin();

  sd.ipa.valve_upstream_pressure = PT::ipa_valve_upstream.getPressure();
  sd.ipa.valve_downstream_pressure = PT::ipa_valve_downstream.getPressure();
  sd.ipa.venturi_differential_pressure = PT::ipa_venturi_differential.getPressure();

  sd.chamber_pressure = PT::chamber.getPressure();

  WindowComparators::lox_valve_upstream_pressure.check(sd.ox.valve_upstream_pressure);
  WindowComparators::lox_valve_downstream_pressure.check(sd.ox.valve_downstream_pressure);
  WindowComparators::lox_venturi_differential_pressure.check(sd.ox.venturi_differential_pressure);
  WindowComparators::lox_valve_temperature.check(sd.ox.valve_temperature);
  WindowComparators::lox_venturi_temperature.check(sd.ox.venturi_temperature);

  WindowComparators::ipa_valve_upstream_pressure.check(sd.ipa.valve_upstream_pressure);
  WindowComparators::ipa_valve_downstream_pressure.check(sd.ipa.valve_downstream_pressure);
  WindowComparators::ipa_venturi_differential_pressure.check(sd.ipa.venturi_differential_pressure);

  WindowComparators::chamber_pressure.check(sd.chamber_pressure);
  return sd;
}

void print_labeled_sensor(const char *msg, float sensor_value, const char *unit) {
  Router::info_no_newline(msg);
  Router::info_no_newline(sensor_value);
  Router::info(unit);
}

void print_all_sensors() {
  Router::info("  Sensor Status ");
  print_labeled_sensor("      PT LOX Valve Upstream: ", PT::lox_valve_upstream.getPressure(), " psi");
  print_labeled_sensor("    PT LOX Valve Downstream: ", PT::lox_valve_downstream.getPressure(), " psi");
  print_labeled_sensor("PT LOX Venturi Differential: ", PT::lox_venturi_differential.getPressure(), " psi");

  print_labeled_sensor("      PT IPA Valve Upstream: ", PT::ipa_valve_upstream.getPressure(), " psi");
  print_labeled_sensor("    PT IPA Valve Downstream: ", PT::ipa_valve_downstream.getPressure(), " psi");
  print_labeled_sensor("PT IPA Venturi Differential: ", PT::ipa_venturi_differential.getPressure(), " psi");

  print_labeled_sensor("                 PT Chamber: ", PT::chamber.getPressure(), " psi");

  print_labeled_sensor("               TC LOX Valve: ", TC::lox_valve_temperature.getTemperature_F(), " F");
  print_labeled_sensor("             TC LOX Venturi: ", TC::lox_venturi_temperature.getTemperature_F(), " F");
  Router::info(" "); // newline
}

/**
 * Follows an angle curve by interpolating between LOX and IPA positions.
 */
void follow_auto_sequence() {
  int kill_reason = DONT_KILL;
  elapsedMicros timer = elapsedMicros();
  unsigned long lastloop = timer;

  WindowComparators::reset();

  long counter = 0;

  for (int i = 0; i < Loader::num_points - 1; i++) {
    while (timer / 1000000.0 < 10) { // TODO - make this run time correct
      float seconds = timer / 1000000.0;
      Sensor_Data sd = get_sensor_data();
      AutoSeqLogger::log_seq_state(); // TODO - actuate DOs
      counter++;
      kill_reason = Safety::check_for_kill(seconds);
      if (kill_reason != DONT_KILL) {
        Safety::kill_response(kill_reason);
        break;
      }
      unsigned long target_slp = LOG_INTERVAL_US - (timer - lastloop);
      delayMicroseconds(target_slp < LOG_INTERVAL_US ? target_slp : 0); // don't delay for too long
      lastloop += LOG_INTERVAL_US;
    }
    if (kill_reason != DONT_KILL) {
      break;
    }
  }
  Router::info_no_newline("Finished ");
  Router::info_no_newline(counter);
  Router::info(" loop iterations.");
}

// init CurveFollower and add relevant router cmds
void begin() {
  Router::add({print_all_sensors, "print_sensors"});
  Router::add({arm, "arm"});
}

// prompt user for log file name, then follow curve
void arm() {
  if (!Loader::loaded_seq) {
    Router::info("ARMING FAILURE: no seq loaded.");
    return;
  }

  if (!PT::zeroed_since_boot) {
    Router::info("ARMING FAILURE: pt boards have not been zeroed.");
    return;
  }

  // filenames use DOS 8.3 standard
  Router::info_no_newline("Enter log filename (1-8 chars + '.' + 3 chars): ");
  String log_file_name = Router::read(50);
  AutoSeqLogger::create_seq_log(log_file_name.c_str()); // lower case files have issues on teensy

  Router::info_no_newline("ARMING COMPLETE. Type `y` and press enter to confirm. ");
  String final_check_str = Router::read(50);
  if (final_check_str != "y") {
    Router::info("ARMING FAILURE: Cancelled by operator.");
    AutoSeqLogger::close_seq_log();
    return;
  }

  follow_auto_sequence();

  Router::info("Finished following sequence!");
  AutoSeqLogger::close_seq_log();
}

} // namespace AutoSequence