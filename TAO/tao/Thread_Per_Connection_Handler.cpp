#include "tao/Thread_Per_Connection_Handler.h"
#include "tao/Connection_Handler.h"
#include "tao/debug.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "ace/Flag_Manip.h"
#include "ace/Vector_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Thread_Per_Connection_Handler::TAO_Thread_Per_Connection_Handler (
    TAO_Connection_Handler *ch,
    TAO_ORB_Core *oc)
  : ACE_Task_Base (oc->thr_mgr ())
  , ch_ (ch)
{
  this->ch_->transport ()->add_reference ();
}

TAO_Thread_Per_Connection_Handler::~TAO_Thread_Per_Connection_Handler ()
{
  this->ch_->close_connection ();
  this->ch_->transport ()->remove_reference ();
}

int
TAO_Thread_Per_Connection_Handler::activate_ch (long flags,
                                                int n_threads)
{
  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Thread_Per_Connection_Handler::")
                  ACE_TEXT ("activate %d threads, flags = %q\n"),
                  n_threads,
                  static_cast<ACE_INT64> (flags)));
    }

  ACE_Vector<ACE_CString> names (n_threads);
  for (int i = 0; i < n_threads; ++i)
    {
      ACE_CString buffer = "          ";
      ACE_OS::itoa (static_cast<ACE_INT32> (i), &buffer[0], 10);
      names.push_back ("TAO_Thread_Per_Connection_Handler " + buffer);
    }

  ACE_Vector<const char *> thread_names (n_threads);
  for (int i = 0; i < n_threads; ++i)
    {
      thread_names.push_back (names[static_cast<size_t> (i)].c_str ());
    }

  return this->activate (flags,
                         n_threads,
                         0,
                         ACE_DEFAULT_THREAD_PRIORITY,
                         -1,
                         0,
                         0,
                         0,
                         0,
                         0,
                         &thread_names[0]);
}

int
TAO_Thread_Per_Connection_Handler::svc ()
{
  ACE::clr_flags (this->ch_->transport ()->event_handler_i ()->get_handle (),
                  ACE_NONBLOCK);

  // Call the implementation here
  return this->ch_->svc_i ();
}

int
TAO_Thread_Per_Connection_Handler::open (void*v)
{
  return this->ch_->open_handler (v);
}

int
TAO_Thread_Per_Connection_Handler::close (u_long)
{
  delete this;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
