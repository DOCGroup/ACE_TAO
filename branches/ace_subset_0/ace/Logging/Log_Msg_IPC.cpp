// $Id$

#include "ace/Logging/Log_Msg_IPC.h"
#include "ace/Logging/Log_Record.h"

ACE_RCSID(ace, Log_Msg_IPC, "$Id$")

ACE_Log_Msg_IPC::ACE_Log_Msg_IPC (void)
{
}

ACE_Log_Msg_IPC::~ACE_Log_Msg_IPC (void)
{
  (void) this->close ();
}

int
ACE_Log_Msg_IPC::open (const ACE_TCHAR *logger_key)
{
  ACE_LOG_MSG_IPC_CONNECTOR con;
  return con.connect (this->message_queue_,
                      ACE_LOG_MSG_IPC_ADDR (logger_key));
}

int
ACE_Log_Msg_IPC::reset (void)
{
  if (this->message_queue_.get_handle () != ACE_INVALID_HANDLE)
    {
      // If we don't do this, handles aren't reused on Win32 and the
      // server eventually crashes!
#if defined (ACE_WIN32)
      ACE_INT32 dummy = ~0;
      this->message_queue_.send_n ((const void *) &dummy,
                                    sizeof (ACE_INT32));
#endif /* ACE_WIN32 */
      return this->close ();
    }
  return 0;
}

int
ACE_Log_Msg_IPC::close (void)
{
  return this->message_queue_.close ();
}

int
ACE_Log_Msg_IPC::log (ACE_Log_Record &log_record)
{
#if defined (ACE_HAS_STREAM_PIPES)
  ACE_Str_Buf log_msg (ACE_static_cast (void *,
                                        &log_record),
                       ACE_static_cast (int,
                                        log_record.length ()));

  // Try to use the <putpmsg> API if possible in order to
  // ensure correct message queueing according to priority.
  return
    this->message_queue_.send
      (ACE_static_cast (const ACE_Str_Buf *, 0),
       &log_msg,
       ACE_static_cast (int, log_record.priority ()),
       MSG_BAND);
#else
  // We're running over sockets, so we'll need to indicate the
  // number of bytes to send.
  return
    this->message_queue_.send_n ((void *) &log_record,
                                  log_record.length ());
#endif /* ACE_HAS_STREAM_PIPES */
}
