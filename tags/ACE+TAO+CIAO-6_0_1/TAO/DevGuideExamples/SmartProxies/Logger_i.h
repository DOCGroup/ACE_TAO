// $Id$

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
  Logger_i (void);
  virtual ~Logger_i (void);
private:
  ofstream log_file;      // Output file stream to which messages are logged.
  time_t log_time;        // Needed for creating a time stamp.
  ACE_TString log_time_string;  // The time stamp string.

virtual CORBA::Boolean log_message (const char * message);
};
#endif /* LOGGERI_H_  */

