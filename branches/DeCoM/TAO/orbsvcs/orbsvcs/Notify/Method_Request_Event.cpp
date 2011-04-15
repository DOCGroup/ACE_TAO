// $Id$

#include "orbsvcs/Notify/Method_Request_Event.h"
#include "orbsvcs/Notify/Delivery_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Method_Request_Event::TAO_Notify_Method_Request_Event (
      const TAO_Notify_Event * event)
  : event_ (event)
{
}

TAO_Notify_Method_Request_Event::TAO_Notify_Method_Request_Event (
      const TAO_Notify::Delivery_Request_Ptr & request)
  : event_ (request->event ().get ())
  , delivery_request_ (request)
{
}

TAO_Notify_Method_Request_Event::TAO_Notify_Method_Request_Event (
      const TAO_Notify_Method_Request_Event & rhs,
      const TAO_Notify_Event * event)
  : event_ (event)
  , delivery_request_ (rhs.delivery_request_)
{
}

TAO_Notify_Method_Request_Event::~TAO_Notify_Method_Request_Event()
{
}

void
TAO_Notify_Method_Request_Event::complete ()
{
  if (this->delivery_request_.get () != 0)
  {
    this->delivery_request_->complete ();
  }
}


unsigned long
TAO_Notify_Method_Request_Event::sequence ()
{
  if (this->delivery_request_.get () != 0)
  {
    return this->delivery_request_->sequence ();
  }
  return 0;
}

bool
TAO_Notify_Method_Request_Event::should_retry ()
{
  if (this->delivery_request_.get () != 0)
  {
    return this->delivery_request_->should_retry ();
  }
  return false;
}

/**********************************************************/


TAO_Notify_Method_Request_Event_Queueable::TAO_Notify_Method_Request_Event_Queueable (
      const TAO_Notify_Method_Request_Event & prev_request,
      const TAO_Notify_Event::Ptr& event)
  : TAO_Notify_Method_Request_Queueable (event.get ())
  , TAO_Notify_Method_Request_Event (prev_request, event.get ())
  , event_var_ (event)
{
}

TAO_Notify_Method_Request_Event_Queueable::TAO_Notify_Method_Request_Event_Queueable (
    TAO_Notify::Delivery_Request_Ptr & request)
  : TAO_Notify_Method_Request_Queueable (request->event ().get ())
  , TAO_Notify_Method_Request_Event (request, request->event ().get ())
  , event_var_ (request->event ())
{
}

TAO_Notify_Method_Request_Event_Queueable::~TAO_Notify_Method_Request_Event_Queueable ()
{
}

int
TAO_Notify_Method_Request_Event_Queueable::execute (void)
{
  ACE_ASSERT (false);
  return -1;
}

const TAO_Notify_Event *
TAO_Notify_Method_Request_Event::event() const
{
  return this->event_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
