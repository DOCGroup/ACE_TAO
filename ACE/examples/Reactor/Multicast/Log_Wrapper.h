/* -*- C++ -*- */
// log_wrapper.h

#include "ace/Profile_Timer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"

#ifndef _LOG_WRAPPER_H
#define _LOG_WRAPPER_H

/// Wraps the sending of log messages over IP multicast.
class Log_Wrapper
{
public:
  Log_Wrapper () = default;
  ~Log_Wrapper () = default;
  Log_Wrapper (Log_Wrapper const &) = delete;
  Log_Wrapper (Log_Wrapper &&) = delete;
  Log_Wrapper &operator= (Log_Wrapper const &) = delete;
  Log_Wrapper &operator= (Log_Wrapper &&) = delete;

  /// @name Logging message types
  /// @{
  enum Log_Priority
  {
    LM_MESSAGE,
    LM_DEBUG,
    LM_WARNING,
    LM_ERROR,
    LM_EMERG
  };
  /// @}

  /// Subscribes to a UDP multicast group.
  int open (int port, char const *mcast_addr);

  /// Sends a string to the logger.
  int log_message (Log_Priority type, char *message);

  /// @name Logging record format
  /// @{
  struct Log_Record
  {
    u_long sequence_number;
    Log_Priority type;
    long  host;
    long  time;
    long  app_id;
    long  msg_length;
  };
  /// @}

private:
  /// Server address where records are logged.
  ACE_INET_Addr server_;

  /// Sequence number for log records.
  u_long sequence_number_ {};

  /// One record reused for multiple log messages.
  Log_Record log_msg_ {};

  /// Multicast logger socket.
  ACE_SOCK_Dgram_Mcast logger_;
};

#endif /* _LOG_WRAPPER_H */
