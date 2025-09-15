#include <Arduino.h>

#include "AutoSequence.h"
#include "PressureSensor.h"
#include "Thermocouples.h"
#include "SPI_Demux.h"
#include "Router.h"
#include "Loader.h"
#include "Safety.h"

void ping() {
  Router::info("pong");
}

void help() {
  Router::print_all_cmds();
}

void setup() {
  Router::begin();
  Router::info("Controller started.");

  Router::add({ping, "ping"}); // example registration
  Router::add({help, "help"});

  Safety::begin();       // prints safety info
  Loader::begin();       // registers data loader functions with the router
  SPI_Demux::begin();    // initializes the SPI backplane
  PT::begin();           // initializes the PT Boards
  TC::begin();           // initializes the TC Boards
  AutoSequence::begin(); // creates seq following commands
}

void loop() {
  Router::run(); // loop only runs once, since there is an internal loop in Router::run()
}