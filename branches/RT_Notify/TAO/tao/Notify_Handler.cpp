#include "Notify_Handler.h"
#include "Connection_Handler.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "ace/Malloc_T.h"

ACE_RCSID (tao,
           Notify_Handler,
           "$Id$")

TAO_Notify_Handler::~TAO_Notify_Handler (void)
{
  this->ch_->decr_refcount ();
}


/*static*/ TAO_Notify_Handler *
TAO_Notify_Handler::create_handler (TAO_Connection_Handler *ch,
                                    ACE_Allocator *alloc)
{
  TAO_Notify_Handler *nh = 0;

  if (alloc)
    {
      ACE_NEW_MALLOC_RETURN (nh,
                             ACE_static_cast (
                                 TAO_Notify_Handler *,
                                 alloc->malloc (sizeof (TAO_Notify_Handler))),
                             TAO_Notify_Handler (ch,
                                                 alloc),
                             0);

      return nh;
    }

  return 0;
}

/*static*/ void
TAO_Notify_Handler::destroy_handler (TAO_Notify_Handler *nh)
{
  if (nh->allocator_)
    {
      ACE_DES_FREE (nh,
                    nh->allocator_->free,
                    TAO_Notify_Handler);
    }

  return;
}


int
TAO_Notify_Handler::handle_input (ACE_HANDLE fd)
{
  // Does return value matter? Not is my opinion.
  this->ch_->handle_input (fd);

  // Yes, we are wantedly returning this so that handle_close () would
  // be called
  return -1;
}


int
TAO_Notify_Handler::handle_close (ACE_HANDLE /*fd*/,
                                  ACE_Reactor_Mask /*close_mask*/)
{
  TAO_Notify_Handler::destroy_handler (this);
  return 0;
}


TAO_Notify_Handler::TAO_Notify_Handler (TAO_Connection_Handler *ch,
                                        ACE_Allocator *alloc)
  : ACE_Event_Handler (ch->transport ()->orb_core ()->reactor ()),
    ch_ (ch),
    allocator_ (alloc)
{
  this->ch_->incr_refcount ();
}
