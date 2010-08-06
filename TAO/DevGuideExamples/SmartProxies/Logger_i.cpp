// $Id$

#include "Logger_i.h"
#include "ace/OS_NS_time.h"
#include <iostream>

Logger_i::Logger_i (void)
{
  log_file.open("Logger.txt");
}

Logger_i::~Logger_i (void)
{
  log_file.close();
}

CORBA::Boolean Logger_i::log_message (const char * message)
{
  ACE_OS::time(&log_time);
  log_time_string = ACE_OS::ctime(&log_time);
  // Replace carriage return with string delimiter.
  log_time_string[24] = '\0';
  log_file << ACE_TEXT_ALWAYS_CHAR(log_time_string.c_str()) << " "
           << message << std::endl;
  return 1;
}

