/* -*- C++ -*- */
// @(#)Log_Wrapper.h	1.1	10/18/96


// log_wrapper.h
// wrapper around sending log messages via multicast

#include "ace/Profile_Timer.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

#if !defined (_LM_WRAPPER_H)
#define _LM_WRAPPER_H

class Log_Wrapper 
{
public:
  Log_Wrapper (void);
  ~Log_Wrapper (void);

  // = Types of logging messages.
  enum ACE_Log_Priority 
  {
    LM_MESSAGE,
    LM_DEBUG, 
    LM_WARNING,
    LM_ERROR,
    LM_EMERG
  };

  int open (const int port, const char* mcast_addr);
  // get an object reference from an orbixd
  
  int log_message (ACE_Log_Priority type, char *message);
  // send a string to the logger

  // = Format of the logging record.
  struct ACE_Log_Record 
  {
    unsigned long sequence_number;
    ACE_Log_Priority type;
    long  host;
    long  time;
    long  app_id;
    long  msg_length;
  };

private:
  ACE_INET_Addr server_;
  // Server address where records are logged.

  ACE_Log_Record log_msg_;
  // One record used for many log messages.

  ACE_SOCK_Dgram logger_;
  // A logger object.
};

#endif /* _LM_WRAPPER_H */
