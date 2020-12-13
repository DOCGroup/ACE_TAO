#include "Thread_Per_Connection_Handler.h"
#include "Connection_Handler.h"
#include "debug.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "ace/Flag_Manip.h"

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
