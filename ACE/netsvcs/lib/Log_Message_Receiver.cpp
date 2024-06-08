#if !defined (LOG_MESSAGE_RECEIVER_CPP)
#define LOG_MESSAGE_RECEIVER_CPP

#include "ace/Log_Msg.h"
#include "Log_Message_Receiver.h"

// Type based log message receiver
template<ACE_SYNCH_DECL> void
Static_Log_Message_Receiver<ACE_SYNCH_USE>::log_record (const ACE_TCHAR *hostname,
                                                        ACE_Log_Record &record)
{
#if defined (ACE_HAS_THREADS)
  static ACE_SYNCH_MUTEX_T lock_;
  ACE_GUARD (ACE_SYNCH_MUTEX_T, guard, lock_);
#endif /* ACE_HAS_THREADS */

  record.print (hostname,
                ACE_Log_Msg::instance ()->flags (),
                stderr);
}

template<ACE_SYNCH_DECL> void
Static_Log_Message_Receiver<ACE_SYNCH_USE>::log_output (const ACE_TCHAR *hostname,
                                                        ACE_Log_Record &record,
                                                        ostream *outputfile)
{
  if (outputfile != 0)
    {
#if defined (ACE_HAS_THREADS)
      static ACE_SYNCH_MUTEX_T lock_;
      ACE_GUARD (ACE_SYNCH_MUTEX_T, guard, lock_);
#endif /* ACE_HAS_THREADS */
      record.print (hostname,
                    ACE_Log_Msg::instance ()->flags (),
                    *outputfile);
    }
}

template<ACE_SYNCH_DECL>
ACE_SYNCH_MUTEX_T Log_Message_Receiver_Impl<ACE_SYNCH_USE>::copy_lock_;

template<ACE_SYNCH_DECL>
Log_Message_Receiver_Impl<ACE_SYNCH_USE>::Log_Message_Receiver_Impl ()
  : count_ (0)
{
}

template<ACE_SYNCH_DECL>
Log_Message_Receiver_Impl<ACE_SYNCH_USE> *
Log_Message_Receiver_Impl<ACE_SYNCH_USE>::create ()
{
  return new Log_Message_Receiver_Impl<ACE_SYNCH_USE>;
}

template<ACE_SYNCH_DECL>
Log_Message_Receiver_Impl<ACE_SYNCH_USE> *
Log_Message_Receiver_Impl<ACE_SYNCH_USE>::attach (Log_Message_Receiver_Impl<ACE_SYNCH_USE> *body)
{
  ACE_ASSERT (body != 0);

#if defined (ACE_HAS_THREADS)
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX_T, guard, copy_lock_, 0);
#endif /* ACE_HAS_THREADS */
  ++body->count_;
  return body;
}

template<ACE_SYNCH_DECL> void
Log_Message_Receiver_Impl<ACE_SYNCH_USE>::detach (Log_Message_Receiver_Impl<ACE_SYNCH_USE> *body)
{
  ACE_ASSERT (body != 0);

#if defined (ACE_HAS_THREADS)
  ACE_GUARD (ACE_SYNCH_MUTEX_T, guard, copy_lock_);
#endif /* ACE_HAS_THREADS */
  if (body->count_-- == 0)
    delete body;
}

// Type based log message receiver
template<ACE_SYNCH_DECL> void
Log_Message_Receiver_Impl<ACE_SYNCH_USE>::log_record (const ACE_TCHAR *hostname,
                                                      ACE_Log_Record &record)
{
  ACE_GUARD (ACE_SYNCH_MUTEX_T, guard, print_lock_);
  record.print (hostname,
                ACE_Log_Msg::instance ()->flags (),
                stderr);
}

template<ACE_SYNCH_DECL> void
Log_Message_Receiver_Impl<ACE_SYNCH_USE>::log_output (const ACE_TCHAR *hostname,
                                                      ACE_Log_Record &record,
                                                      ostream *outputfile)
{
  if (outputfile != 0)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX_T, guard, print_lock_);
      record.print (hostname,
                    ACE_Log_Msg::instance ()->flags (),
                    *outputfile);
    }
}

template<ACE_SYNCH_DECL>
Log_Message_Receiver_Impl<ACE_SYNCH_USE>::~Log_Message_Receiver_Impl ()
{
  ACE_ASSERT (count_ == 0 - 1);
}

template<ACE_SYNCH_DECL>
Log_Message_Receiver<ACE_SYNCH_USE>::Log_Message_Receiver ()
  : receiver_impl_ (Log_Message_Receiver_Impl<ACE_SYNCH_USE>::create ())
{
  ACE_ASSERT (receiver_impl_ != 0);
}

template<ACE_SYNCH_DECL>
Log_Message_Receiver<ACE_SYNCH_USE>::Log_Message_Receiver
  (Log_Message_Receiver<ACE_SYNCH_USE> const &rhs)
    : receiver_impl_ (Log_Message_Receiver_Impl<ACE_SYNCH_USE>::attach (rhs.receiver_impl_))
{
  ACE_ASSERT (receiver_impl_ != 0);
}

// Type based log message receiver
template<ACE_SYNCH_DECL> void
Log_Message_Receiver<ACE_SYNCH_USE>::log_record(const ACE_TCHAR *hostname,
                                                ACE_Log_Record &record)
{
  ACE_ASSERT (receiver_impl_ != 0);
  receiver_impl_->log_record (hostname, record);
}

template<ACE_SYNCH_DECL> void
Log_Message_Receiver<ACE_SYNCH_USE>::log_output(const ACE_TCHAR *hostname,
                                                ACE_Log_Record &record,
                                                ostream *outputfile)
{
  ACE_ASSERT (receiver_impl_ != 0);
  receiver_impl_->log_output (hostname, record, outputfile);
}

template<ACE_SYNCH_DECL>
Log_Message_Receiver<ACE_SYNCH_USE>::~Log_Message_Receiver ()
{
  ACE_ASSERT (receiver_impl_ != 0);
  Log_Message_Receiver_Impl<ACE_SYNCH_USE>::detach (receiver_impl_);
}

#endif /* LOG_MESSAGE_RECEIVER_CPP */
