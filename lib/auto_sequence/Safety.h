#ifndef SAFETY_H
#define SAFETY_H

#define CHECK_SERIAL_KILL       // should check for 'k' on serial monitor to kill
#define ENABLE_WC_SAFETY_CHECKS // check window comparator values

#define DONT_KILL 0        // continue loop
#define KILLED_BY_SERIAL 1 // serial cmd caused kill
#define KILLED_BY_WC 2     // window comparator checks

namespace Safety {
void begin();
void kill_response(int kill_reason);
int check_for_kill(float time_seconds);
} // namespace Safety

#endif