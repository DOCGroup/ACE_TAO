#if !defined (LOG_MESSAGE_RECEIVER_CPP)
#define LOG_MESSAGE_RECEIVER_CPP

#include "ace/Synch.h"
#include "ace/Synch_T.h"
#include "Log_Message_Receiver.h"

// Type based log message receiver
template<ACE_SYNCH_1> void
Static_Log_Message_Receiver<ACE_SYNCH_2>::log_record (char const *hostname,
						      ACE_Log_Record &record)
{
#if defined (ACE_HAS_THREADS)
    static ACE_SYNCH_MUTEX_T lock_;
    ACE_Guard<ACE_SYNCH_MUTEX_T> guard (lock_);
#endif /* ACE_HAS_THREADS */
   
  record.print (hostname, 0, stderr);
}

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
template<ACE_SYNCH_1> 
ACE_SYNCH_MUTEX_T Log_Message_Receiver_Impl<ACE_SYNCH_2>::copy_lock_;
#else
static ACE_SYNCH_MUTEX global_copy_lock_;
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

template<ACE_SYNCH_1> inline 
Log_Message_Receiver_Impl<ACE_SYNCH_2>::Log_Message_Receiver_Impl (void) 
  : count_ (0)
{
}

template<ACE_SYNCH_1> inline
Log_Message_Receiver_Impl<ACE_SYNCH_2> *
Log_Message_Receiver_Impl<ACE_SYNCH_2>::create (void)
{
  return new Log_Message_Receiver_Impl<ACE_SYNCH_2>;
}

template<ACE_SYNCH_1> inline
Log_Message_Receiver_Impl<ACE_SYNCH_2> *
Log_Message_Receiver_Impl<ACE_SYNCH_2>::attach (Log_Message_Receiver_Impl<ACE_SYNCH_2> *body)
{
  assert(body != 0);

#if defined (ACE_HAS_THREADS)  
  #if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  Guard guard (copy_lock_);
  #else
  // Use the "body"s print lock as copy lock.
  ACE_Guard<ACE_SYNCH_MUTEX> guard (global_copy_lock_);  
  #endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
#endif
  ++body->count_;
  return body;
}

template<ACE_SYNCH_1> inline void
Log_Message_Receiver_Impl<ACE_SYNCH_2>::detach(Log_Message_Receiver_Impl<ACE_SYNCH_2> *body)
{
  assert (body != 0);
  
#if defined (ACE_HAS_THREADS)  
  #if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  Guard guard (copy_lock_);
  #else
  // Use the "body"s print lock as copy lock.
  ACE_Guard<ACE_SYNCH_MUTEX> guard (global_copy_lock_);  
  #endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
#endif
  if (body->count_-- == 0)
    delete body;
}

// Type based log message receiver
template<ACE_SYNCH_1> inline void
Log_Message_Receiver_Impl<ACE_SYNCH_2>::log_record (char const *hostname,
						    ACE_Log_Record &record)
{
  ACE_MT (Guard guard (print_lock_));
  record.print (hostname, 0, stderr);
}

template<ACE_SYNCH_1> inline 
Log_Message_Receiver_Impl<ACE_SYNCH_2>::~Log_Message_Receiver_Impl (void)
{
  assert (count_ == 0 - 1);
}

template<ACE_SYNCH_1>
Log_Message_Receiver<ACE_SYNCH_2>::Log_Message_Receiver() 
  : receiver_impl_ (Log_Message_Receiver_Impl<ACE_SYNCH_2>::create ())
{
  assert (receiver_impl_ != 0);  
}

template<ACE_SYNCH_1>
Log_Message_Receiver<ACE_SYNCH_2>::Log_Message_Receiver 
  (Log_Message_Receiver<ACE_SYNCH_2> const &rhs)
    : receiver_impl_ (Log_Message_Receiver_Impl<ACE_SYNCH_2>::attach (rhs.receiver_impl_))
{
  assert (receiver_impl_ != 0);  
}

// Type based log message receiver
template<ACE_SYNCH_1> void
Log_Message_Receiver<ACE_SYNCH_2>::log_record(char const *hostname,
                                              ACE_Log_Record &record)
{
  assert (receiver_impl_ != 0);  
  receiver_impl_->log_record (hostname, record);
}

template<ACE_SYNCH_1>
Log_Message_Receiver<ACE_SYNCH_2>::~Log_Message_Receiver (void)
{
  assert (receiver_impl_ != 0);  
  Log_Message_Receiver_Impl<ACE_SYNCH_2>::detach (receiver_impl_);
}

#endif /* LOG_MESSAGE_RECEIVER_CPP */
