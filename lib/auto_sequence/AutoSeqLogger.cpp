#include "AutoSeqLogger.h"

#include "Router.h"
#include "SDCard.h"

namespace AutoSeqLogger {

File seqlogfile;

void log_seq_state() {
  // seqlogfile.println(); // TODO - log stuff here
  seqlogfile.flush();
  // TODO - prints?
}

// creates a log file for the current sequence
void create_seq_log(const char *filename) {
  seqlogfile = SDCard::open(filename, FILE_WRITE);
}

// close and flush the log file
void close_seq_log() {
  seqlogfile.flush();
  seqlogfile.close();
}

} // namespace AutoSeqLogger
