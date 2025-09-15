#ifndef CURVE_FOLLOWER_H
#define CURVE_FOLLOWER_H

struct Fluid_Line {
  float valve_upstream_pressure;       // psi
  float valve_downstream_pressure;     // psi
  float venturi_differential_pressure; // psi
  float venturi_temperature;           // K
  float valve_temperature;             // K
};

struct Sensor_Data {
  float chamber_pressure; // psi
  Fluid_Line ox;
  Fluid_Line ipa;
};

namespace AutoSequence {

void begin();
void print_all_sensors();
void arm();

}; // namespace AutoSequence

#endif