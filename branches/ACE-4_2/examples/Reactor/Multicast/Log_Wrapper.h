/* -*- C++ -*- */
// $Id$

// log_wrapper.h

#include "ace/Profile_Timer.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"

#if !defined (_LOG_WRAPPER_H)
#define _LOG_WRAPPER_H

class Log_Wrapper 
  // = TITLE
  //      Provide a wrapper around sending log messages via IP
  //      multicast. 
{
public:
  Log_Wrapper (void);
  ~Log_Wrapper (void);

  // = Types of logging messages.
  enum ACE_Log_Priority 
  {
    LOG_MESSAGE,
    LOG_DEBUG, 
    LOG_WARNING,
    LOG_ERROR,
    LOG_EMERG
  };

  int open (const int port, const char* mcast_addr);
  // get an object reference from an orbixd
  
  int log_message (ACE_Log_Priority type, char *message);
  // send a string to the logger

  // = Format of the logging record.
  struct ACE_Log_Record 
  {
    u_long sequence_number;
    ACE_Log_Priority type;
    long  host;
    long  time;
    long  app_id;
    long  msg_length;
  };

private:
  ACE_INET_Addr server_;
  // Server address where records are logged.

  u_long sequence_number_;
  // Keep track of the sequence.

  ACE_Log_Record log_msg_;
  // One record used for many log messages.

  ACE_SOCK_Dgram_Mcast logger_;
  // A logger object.
};

#endif /* _LOG_WRAPPER_H */
