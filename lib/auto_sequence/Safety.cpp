#include "Safety.h"

#include "WindowComparator.h"
#include "Router.h"

// prints which safety features are active
void Safety::begin() {
#ifndef ENABLE_WC_SAFETY_CHECKS
  Router::info("WARNING! Running without wc safety.");
#endif
}

// prints debug information after a kill
void Safety::kill_response(int kill_reason) {
  Router::info("Fault detected! Curve following terminated, odrives disabled, fault signal sent to Zucrow.");
  Router::info_no_newline("Fault cause: ");

  if (kill_reason == KILLED_BY_SERIAL) {
    Router::info("serial abort triggered");
  }
  if (kill_reason == KILLED_BY_WC) {
    Router::info_no_newline("Window comparator ");
    Router::info_no_newline(WindowComparators::WC_ERROR.causeID);
    if (WindowComparators::WC_ERROR.causeReason == WC_CAUSE_OVERFLOW) {
      Router::info_no_newline(" overflow ");
      Router::info_no_newline(WindowComparators::WC_ERROR.causeValue);
      Router::info_no_newline(" > ");
      Router::info(WindowComparators::WC_ERROR.compValue);
    } else {
      Router::info_no_newline(" underflow ");
      Router::info_no_newline(WindowComparators::WC_ERROR.causeValue);
      Router::info_no_newline(" < ");
      Router::info(WindowComparators::WC_ERROR.compValue);
    }
  }
}

// checks various kill conditions, returns the first one found, or DONT_KILL
int Safety::check_for_kill(float time_seconds) {
#ifdef CHECK_SERIAL_KILL
  if (COMMS_SERIAL.available() && COMMS_SERIAL.read() == 'k') {
    return KILLED_BY_SERIAL;
  }
#endif

#ifdef ENABLE_WC_SAFETY_CHECKS
  if (WindowComparators::WC_ERROR.isError) {
    return KILLED_BY_WC;
  }
#endif

  return DONT_KILL;
}
