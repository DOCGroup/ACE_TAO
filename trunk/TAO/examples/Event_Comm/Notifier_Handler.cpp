// $Id$

#include "Notifier_Handler.h"

ACE_RCSID(Supplier, Notifier_Handler, "$Id$")

int
Notifier_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  if (this->notifier_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "closing down Notifier_Handler\n"));
      CORBA::release (this->notifier_);
      this->notifier_ = 0;
      // *Must* be allocated dyanmically!
      ::operator delete ((void *) this);
    }

  return 0;
}

Event_Comm::Notifier *
Notifier_Handler::notifier (void)
{
  return this->notifier_;
}

void
Notifier_Handler::notifier (Event_Comm::Notifier *notifier)
{
  if (this->notifier_ != notifier)
    {
      CORBA::release (this->notifier_);
      this->notifier_ = notifier;
    }
}

// Create and initialize a Notifier target object.

Notifier_Handler::Notifier_Handler (const char *service_location,
				    const char *marker,
				    int putit)
{
  // Create a notifier object using the implementation class
  // Notifier_i.
  ACE_NEW (this->notifier_,
           TIE_Event_Comm_Notifier (Notifier_i) (new Notifier_i,
                                                 marker));
}

// Destroy a Notifier target object.

Notifier_Handler::~Notifier_Handler (void)
{
  this->handle_close ();
}
