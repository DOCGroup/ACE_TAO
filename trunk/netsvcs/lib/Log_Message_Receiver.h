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

// Type based log message receiver
template<ACE_SYNCH_1> 
class Static_Log_Message_Receiver
{
public:
  static void log_record (char const *hostname, 
			  ACE_Log_Record &record);
};

//Forward declaration
template<ACE_SYNCH_1> class Log_Message_Receiver_Impl;

// Instance based log message receiver
template<ACE_SYNCH_1> 
class Log_Message_Receiver
{
public:
  Log_Message_Receiver (void);
  Log_Message_Receiver(Log_Message_Receiver<ACE_SYNCH_2> const &rhs);
  ~Log_Message_Receiver (void);
  
  void log_record (const char *hostname,
		   ACE_Log_Record &record);
  
private:
  void operator= (const Log_Message_Receiver<ACE_SYNCH_2> &rhs);
  
  Log_Message_Receiver_Impl<ACE_SYNCH_2> *receiver_impl_;
  // Attributes.
};

template<ACE_SYNCH_1> 
class Log_Message_Receiver_Impl
{
  friend class ACE_Shutup_GPlusPlus;  // Turn off g++ warning
public:
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
    
  int count_;
  ACE_SYNCH_MUTEX print_lock_;

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
