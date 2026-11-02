#include "Logger_i.h"
#include "ace/OS_NS_time.h"
#include <iostream>

Logger_i::Logger_i ()
{
  this->log_file_.open ("Logger.txt");
}

CORBA::Boolean Logger_i::log_message (char const *message)
{
  ACE_OS::time(&log_time);
  log_time_string = ACE_OS::ctime(&log_time);
  // Replace carriage return with string delimiter.
  log_time_string[24] = '\0';
  this->log_file_ << ACE_TEXT_ALWAYS_CHAR (log_time_string.c_str ()) << " "
                  << message << std::endl;
  return true;
}

