/* -*- C++ -*- */
// $Id$

// log_wrapper.h

#include "ace/Profile_Timer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"

#ifndef _LOG_WRAPPER_H
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
  enum Log_Priority
  {
    LM_MESSAGE,
    LM_DEBUG,
    LM_WARNING,
    LM_ERROR,
    LM_EMERG
  };

  int open (const int port, const char* mcast_addr);
  // Subscribe to a given UDP multicast group

  int log_message (Log_Priority type, char *message);
  // send a string to the logger

  // = Format of the logging record.
  struct Log_Record
  {
    u_long sequence_number;
    Log_Priority type;
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

  Log_Record log_msg_;
  // One record used for many log messages.

  ACE_SOCK_Dgram_Mcast logger_;
  // A logger object.
};

#endif /* _LOG_WRAPPER_H */
