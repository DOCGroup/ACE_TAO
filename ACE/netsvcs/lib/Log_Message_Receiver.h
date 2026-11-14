/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Log_Message_Receiver.h
 *
 *  @author Per Andersson
 */
//=============================================================================

#ifndef LOG_MESSAGE_RECEIVER_H
#define LOG_MESSAGE_RECEIVER_H

#include "ace/Log_Record.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_Traits.h"
#include "ace/Guard_T.h"
#if defined (ACE_HAS_THREADS)
#  include "ace/Thread_Mutex.h"
#else
#  include "ace/Null_Mutex.h"
#endif /* ACE_HAS_THREADS */

/**
 * @class Static_Log_Message_Receiver
 *
 * @brief Static_Log_Message_Receiver is a simple log message receiver. It
 * has no instance data and only static member
 * functions. Static/typed based receivers are best when all LMR
 * should do exactly the same thing.
 *
 * This class contains a static log_record member function that
 * prints the content of log_records on stderr.
 */
template<ACE_SYNCH_DECL>
class Static_Log_Message_Receiver
{
public:
  /// Prints the log_record to stderr using record.print (hostname, 0, stderr).
  /// Serializes the output by using a ACE_SYNCH_MUTEX.
  static void log_record(const ACE_TCHAR *hostname,
                         ACE_Log_Record &record);

  /// Prints the log_record to a user specified ostream.
  static void log_output(const ACE_TCHAR *hostname,
                         ACE_Log_Record &record,
                         ostream *output);
};

//Forward declaration
template<ACE_SYNCH_DECL> class Log_Message_Receiver_Impl;

/**
 * @class Log_Message_Receiver
 *
 * @brief Log_Message_Receiver is a little more complicated log message
 * receiver.  It is instance based and have a reference counted
 * implementation.  Log_Message_Receiver is the envelope class for
 * Log_Message_Receiver_Impl.  The difference between
 * Static_Log_Message_Receiver and Log_Message_Receiver is that is
 * possible to have instance data in Log_Message_Receiver.
 *
 * A receiver must provide a log_record method, a public destructor and copy
 * constructor, and a default constructor. It should act as a cheap-to-copy
 * envelope object. The log_record method processes a valid ACE_Log_Record
 * for a non-null hostname.
 *
 * Log message receivers process records after the client and server logging
 * handlers forward, receive, frame, and dispatch them. Keeping that work in
 * a separate receiver allows applications to change processing without
 * changing the server handler, for example to write daily or fixed-size
 * rotating log files or forward selected records to a monitoring system.
 * Multiple receivers of the same type can use different output files.
 *
 * @note
 * The practical usage of this is limited with the current
 * ACE_Server_Logging_Acceptor_T design. Since
 * ACE_Server_Logging_Acceptor_T will create the
 * Log_Message_Receiver using the default constructor.  The main
 * reason for inclusion right now is to ensure that the code in
 * ACE_Server_Logging_Handler_T works both with type and instance
 * based LMRs.
 *
 * This class contains a log_record member function that prints the
 * content of log_records on stderr.
 */
template<ACE_SYNCH_DECL>
class Log_Message_Receiver
{
public:
  /// Creates a new Log_Message_Receiver
  Log_Message_Receiver ();
  Log_Message_Receiver(Log_Message_Receiver<ACE_SYNCH_USE> const &rhs);
  ~Log_Message_Receiver ();

  void log_record (const ACE_TCHAR *hostname,
                   ACE_Log_Record &record);

  void log_output(const ACE_TCHAR *hostname,
                  ACE_Log_Record &record,
                  ostream *output);
private:
  void operator= (const Log_Message_Receiver<ACE_SYNCH_USE> &rhs) = delete;

  /// Attributes
  Log_Message_Receiver_Impl<ACE_SYNCH_USE> *receiver_impl_;
};

/**
 * @class Log_Message_Receiver_Impl
 *
 * @brief Implementation with reference count.
 */
template<ACE_SYNCH_DECL>
class Log_Message_Receiver_Impl
{
public:
  // Methods for handling reference count and instance lifetime
  static Log_Message_Receiver_Impl *create ();
  static Log_Message_Receiver_Impl *attach (Log_Message_Receiver_Impl<ACE_SYNCH_USE> *body);
  static void detach (Log_Message_Receiver_Impl<ACE_SYNCH_USE> *body);

  void log_record (const ACE_TCHAR *hostname, ACE_Log_Record &record);

  void log_output(const ACE_TCHAR *hostname,
                  ACE_Log_Record &record,
                  ostream *output);

protected:
  Log_Message_Receiver_Impl ();
  ~Log_Message_Receiver_Impl ();

  /// Attributes
  int count_;
  ACE_SYNCH_MUTEX_T print_lock_;

private:
  static ACE_SYNCH_MUTEX_T copy_lock_;

  void operator= (const Log_Message_Receiver_Impl<ACE_SYNCH_USE> &) = delete;
  Log_Message_Receiver_Impl (const Log_Message_Receiver_Impl<ACE_SYNCH_USE> &) = delete;
};

#include "Log_Message_Receiver.cpp"

#endif /* LOG_MESSAGE_RECEIVER_H */
