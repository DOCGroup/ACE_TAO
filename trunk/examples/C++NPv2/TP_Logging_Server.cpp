/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS.h"
#include "ace/Message_Block.h"
#include "TP_Logging_Server.h"

int TP_Logging_Handler::handle_input (ACE_HANDLE) {
  ACE_Message_Block *mblk = 0;
  if (logging_handler_.recv_log_record (mblk) != -1) {
    ACE_Message_Block *log_blk = 0;
    ACE_NEW_RETURN
      (log_blk, ACE_Message_Block
                  (ACE_reinterpret_cast (char *, this)), -1);
    log_blk->cont (mblk);
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, -1);
    if (TP_LOGGING_TASK::instance ()->put (log_blk) == -1)
    { log_blk->release (); return -1; }
    ++queued_count_;
    return 0;
  } else return -1;
}


int
TP_Logging_Handler::handle_close (ACE_HANDLE handle,
                                  ACE_Reactor_Mask) {
  int close_now = 0;
  if (handle != ACE_INVALID_HANDLE) {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, -1);
    if (queued_count_ == 0)
      close_now = 1;
    else
      deferred_close_ = 1;
  } else {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, -1);
    queued_count_--;
    if (queued_count_ == 0) close_now = deferred_close_;
  }

  if (close_now)
    return Logging_Event_Handler::handle_close ();
  return 0;
}


int TP_Logging_Task::svc () {
  for (ACE_Message_Block *log_blk; getq (log_blk) != -1; ) {
    TP_Logging_Handler *tp_handler = ACE_reinterpret_cast
      (TP_Logging_Handler *, log_blk->rd_ptr ());
    Logging_Handler logging_handler (tp_handler->log_file ());
    logging_handler.write_log_record (log_blk->cont ());

    log_blk->release ();
    tp_handler->handle_close (ACE_INVALID_HANDLE, 0);
  }
  return 0;
}

ACE_FACTORY_DEFINE (TPLS, TP_Logging_Server)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Reactor_Logging_Server<TP_Logging_Acceptor>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Reactor_Logging_Server<TP_Logging_Acceptor>
#elif defined (__GNUC__) && defined (_AIX)
template ACE_Singleton<TP_Logging_Task, ACE_Null_Mutex> *
  ACE_Singleton<TP_Logging_Task, ACE_Null_Mutex>::singleton_;
template ACE_Unmanaged_Singleton<TP_Logging_Task, ACE_Null_Mutex> *
  ACE_Unmanaged_Singleton<TP_Logging_Task, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
