#ifndef AUTO_SEQ_LOGGER_H
#define AUTO_SEQ_LOGGER_H

namespace AutoSeqLogger {
void create_seq_log(const char *filename);
void log_seq_state();
void close_seq_log();

}; // namespace AutoSeqLogger

#endif
