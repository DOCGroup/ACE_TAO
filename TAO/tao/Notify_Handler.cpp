#include "Notify_Handler.h"
#include "Connection_Handler.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "ace/Malloc_T.h"
#include "Resume_Handle.h"

ACE_RCSID (tao,
           Notify_Handler,
           "$Id$")

TAO_Notify_Handler::TAO_Notify_Handler (TAO_Transport *t,
                                        ACE_HANDLE h,
                                        ACE_Allocator *alloc)
  : ACE_Event_Handler (t->orb_core ()->reactor ()),
    t_ (t),
    h_ (h),
    allocator_ (alloc)
{
  this->t_->add_reference ();
}

TAO_Notify_Handler::TAO_Notify_Handler (TAO_Connection_Handler *ch,
                                        ACE_Allocator *alloc)
 : ACE_Event_Handler (ch->transport ()->orb_core ()->reactor ()),
   t_ (ch->transport ()),
   h_ (ACE_INVALID_HANDLE),
   allocator_ (alloc)
{
  this->t_->add_reference ();
  // This constructor should *never* get called, it is just here to
  // for backward comptibility.
  ACE_ASSERT (0);
}

TAO_Notify_Handler::~TAO_Notify_Handler (void)
{
  this->t_->remove_reference ();;
}


/*static*/ TAO_Notify_Handler *
TAO_Notify_Handler::create_handler (TAO_Connection_Handler *,
                                    ACE_Allocator *)
{
  return 0;
}

/*static*/ TAO_Notify_Handler *
TAO_Notify_Handler::create_handler (TAO_Transport *t,
                                    ACE_HANDLE h,
                                    ACE_Allocator *alloc)
{
  TAO_Notify_Handler *nh = 0;

  if (alloc)
    {
      ACE_NEW_MALLOC_RETURN (nh,
                             ACE_static_cast (
                                 TAO_Notify_Handler *,
                                 alloc->malloc (sizeof (TAO_Notify_Handler))),
                             TAO_Notify_Handler (t,
                                                 h,
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
TAO_Notify_Handler::handle_input (ACE_HANDLE)
{
  // NOTE: We will do what the Connection_Handler will do with some
  // exceptions though.. Quite a few things done are not required
  // by the Notify_Handler at all.

  // Let the transport know that it is used
  (void) this->t_->update_transport ();

  TAO_Resume_Handle resume_handle (this->t_->orb_core (),
                                   this->h_);

  // Does return value matter? Not is my opinion.
  (void) this->t_->handle_input (resume_handle);

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
