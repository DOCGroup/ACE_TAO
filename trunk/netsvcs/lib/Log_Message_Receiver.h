/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Server_Logging_Handler.h 
//
// = AUTHOR
//    Per Andersson
// 
// ============================================================================

#if !defined (LOG_MESSAGE_RECEIVER_H)
#define LOG_MESSAGE_RECEIVER_H

#include "ace/Synch.h"
#include "ace/Log_Record.h"

// Type based log message receiver
template<ACE_SYNCH_1> 
class Static_Log_Message_Receiver
// = TITLE
{
public:
  static void log_record (char const *hostname,
			  ACE_Log_Record &record);
};

//Forward declaration.
template<ACE_SYNCH_1> 
class Log_Message_Receiver_Impl;

// Instance based log message receiver.
template<ACE_SYNCH_1> 
class Log_Message_Receiver
// = TITLE
{
public:
  Log_Message_Receiver (void);
  Log_Message_Receiver (const Log_Message_Receiver<ACE_SYNCH_2> &rhs);
  ~Log_Message_Receiver (void);
  
  void log_record (char const *hostname, 
		   ACE_Log_Record &record);
  
private:
  Log_Message_Receiver &operator= (const Log_Message_Receiver<ACE_SYNCH_2> &rhs);
  
  // = Attributes.
  ACE_SYNCH_MUTEX_T lock_;
  Log_Message_Receiver_Impl<ACE_SYNCH_2> *receiver_impl_;
};

template<ACE_SYNCH_1>
class Log_Message_Receiver_Impl
{
  friend class ACE_Shutup_GPlusPlus;  // Turn off g++ warning
public:
  static Log_Message_Receiver_Impl *create (void);
  static Log_Message_Receiver_Impl *attach (Log_Message_Receiver_Impl<ACE_SYNCH_2> *body);
  static void detach (Log_Message_Receiver_Impl<ACE_SYNCH_2> *body);
  
  void log_record (char const *hostname,
		   ACE_Log_Record &record);

private:
  Log_Message_Receiver_Impl (void);
  ~Log_Message_Receiver_Impl (void);
  
  // = Attributes.
  int count_;
  ACE_SYNCH_MUTEX_T print_lock_;
  static ACE_SYNCH_MUTEX_T copy_lock_;
  typedef ACE_Guard<ACE_SYNCH_MUTEX_T> Guard;

  // = Not implemeted
  Log_Message_Receiver_Impl (const Log_Message_Receiver_Impl<ACE_SYNCH_2> &lhs);
  Log_Message_Receiver_Impl &operator= (const Log_Message_Receiver_Impl<ACE_SYNCH_2> &lhs);
};  

#endif /* LOG_MESSAGE_RECEIVER_H */



