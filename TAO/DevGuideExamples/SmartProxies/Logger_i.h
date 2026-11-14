#ifndef LOGGERI_H_
#define LOGGERI_H_

#include "LoggerS.h"
#include "ace/String_Base.h"
#include <fstream>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Logger_i : public virtual POA_Logger
{
public:
  Logger_i ();
  ~Logger_i () override = default;

  Logger_i (Logger_i const &) = delete;
  Logger_i (Logger_i &&) = delete;
  Logger_i &operator= (Logger_i const &) = delete;
  Logger_i &operator= (Logger_i &&) = delete;

private:
  std::ofstream log_file_; // Output file stream to which messages are logged.
  time_t log_time;        // Needed for creating a time stamp.
  ACE_TString log_time_string;  // The time stamp string.

  CORBA::Boolean log_message (char const *message) override;
};
#endif /* LOGGERI_H_  */

