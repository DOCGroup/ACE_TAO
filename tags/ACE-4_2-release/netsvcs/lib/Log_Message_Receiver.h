/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Log_Message_Receiver
//
// = AUTHOR
//    Per Andersson
// 
// ============================================================================

#if !defined (LOG_MESSAGE_RECEIVER_H)
#define LOG_MESSAGE_RECEIVER_H

#include "ace/Log_Record.h"
#include "ace/Synch.h"

// ==========================================================================//
//------------- General Requirements on a Log Message Receiver --------------//
// ==========================================================================//
//
//  The requiremnts on a log manager receiver, T, are quite simple.
//  1: There must exist one "log_record" member function with the following
//     prototype:
//        void log_record(char const* hostname,
//                        ACE_Log_Record &record);
//
//  2: There must exist a public destructor.
//  3: There must exist a public copy constructor.
//  4: There must exist a default constructor. (for now)
//
//
//  The semantics are also simple. A log message receiver should
//  behave as an accessor object (smart pointer or envelope class). 
//  It should be very cheap to copy and the should be no noticeable 
//  difference when using either the new copy or the old log message 
//  receiver.
//
//  Methods:
//    void log_record(char const* hostname,
//                    ACE_Log_Record& record)
//  Description: 
//    Processes the log record "record" from the host "hostname"
//  Precondition: 
//    hostname != 0;
//  Requirements:
//    Record must be a valid ACE_Log_Record.
//
// ==========================================================================//

// ==========================================================================//
// ------------ General Description of a Log Message Receiver -------------- //
// ==========================================================================//
//
//  Log Message Receivers, LRMs, are processing log records. It is the
//  LRM that writes a log message to stderr, stdout, a log file and maybee
//  converts some of the log messages to notifications, warnings, alarms
//  and forwards them to some operation and maintenance system (PATROL).
//
//  The client logging handler and server logging handler are responsible
//  for forwarding, receiving, framing, processing log records. 
//  That is a very usable service, but it should also be possible to change
//  how log records are processed without having to rewrite code in 
//  the server log handler. This code should instead be written as a  
//  separate entity, a Log Message Receiver. 
//
//  A simple LMR should be very easy to write but it should also 
//  be possible to write more complex LMRs, like one that creates
//  a new log file each day or keeps a fixed size, round robin,
//  log file. It should also be possible to have separate LMRs
//  of the same type that uses differnt log files.
//
// ==========================================================================//


// Type based log message receiver
template<ACE_SYNCH_1> 
class Static_Log_Message_Receiver
  // = TITLE
  //  Static_Log_Message_Receiver is a simple log message receiver. It
  //  has no instance data and only static member functions. Static/typed
  //  based receivers are best when all LMR should do exactly the same thing.
  //
  // = DESCRIPTION
  //  This class contains a static log_record member function that
  //  prints the content of log_records on stderr.
{
public:
  static void log_record(const char *hostname, 
                         ACE_Log_Record &record);
  // Prints the log_record to stderr using record.print (hostname, 0, stderr).
  // Serializes the output by using a ACE_SYNCH_MUTEX.
};

// Instance based log message receiver

// ------------------------ Log_Message_Receiver --------------------------- //
//
//  Log_Message_Receiver is little more complicated log message receiver.
//  It is instance based and have a reference counted implementation.
//  Log_Message_Receiver is the envelope class for Log_Message_Receiver_Impl.
//  
// ------------------------------------------------------------------------- //


//Forward declaration
template<ACE_SYNCH_1> class Log_Message_Receiver_Impl;

template<ACE_SYNCH_1> 
class Log_Message_Receiver
  // = TITLE
  //  Log_Message_Receiver is a little more complicated log message receiver.
  //  It is instance based and have a reference counted implementation.
  //  Log_Message_Receiver is the envelope class for Log_Message_Receiver_Impl.
  //  The difference between Static_Log_Message_Receiver and 
  //  Log_Message_Receiver is that is possible to have instance data
  //  in Log_Message_Receiver. 
  //
  //  Comment:
  //  The practical usage of this is limited with the current
  //  ACE_Server_Logging_Acceptor_T design. Since ACE_Server_Logging_Acceptor_T
  //  will create the Log_Message_Receiver using the default constructor.
  //  The main reason for inclusion right now is to ensure that the
  //  code in ACE_Server_Logging_Handler_T works both with type and instance
  //  based LMRs.
  //
  //
  // = DESCRIPTION
  //  This class contains a log_record member function that
  //  prints the content of log_records on stderr.
{
public:
  Log_Message_Receiver (void);
  // Creates a new Log_Message_Receiver
  Log_Message_Receiver(Log_Message_Receiver<ACE_SYNCH_2> const &rhs);
  ~Log_Message_Receiver (void);
  
  void log_record (const char *hostname,
                   ACE_Log_Record &record);  
private:
  void operator= (const Log_Message_Receiver<ACE_SYNCH_2> &rhs);
  
  // Attributes.
  Log_Message_Receiver_Impl<ACE_SYNCH_2> *receiver_impl_;
};

// Implementation with reference count.

template<ACE_SYNCH_1> 
class Log_Message_Receiver_Impl
{
  friend class ACE_Shutup_GPlusPlus;  // Turn off g++ warning
public:
  // Methods for handling reference count and instance lifetime
  static Log_Message_Receiver_Impl *create (void);
  static Log_Message_Receiver_Impl *attach (Log_Message_Receiver_Impl<ACE_SYNCH_2> *body);
  static void detach (Log_Message_Receiver_Impl<ACE_SYNCH_2> *body);
  
  void log_record (const char *hostname,
                   ACE_Log_Record &record);

private:
  Log_Message_Receiver_Impl (void);
  ~Log_Message_Receiver_Impl (void);
  
  // = Not implemeted
  Log_Message_Receiver_Impl (const Log_Message_Receiver_Impl<ACE_SYNCH_2> &rhs);
  void operator=(const Log_Message_Receiver_Impl<ACE_SYNCH_2> &rhs);
    
  typedef ACE_Guard<ACE_SYNCH_MUTEX_T> Guard;
  // Attributes
  int count_;
  ACE_SYNCH_MUTEX_T print_lock_;

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static ACE_SYNCH_MUTEX_T copy_lock_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Log_Message_Receiver.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Log_Message_Receiver.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* LOG_MESSAGE_RECEIVER_H */
