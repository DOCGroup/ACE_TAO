#include "Notifier_Handler.h"
// $Id$


#if defined (ACE_HAS_ORBIX)

#if defined (ACE_HAS_MT_ORBIX)
typedef ACE_MT_CORBA_Handler CORBA_HANDLER;
#else
typedef ACE_ST_CORBA_Handler CORBA_HANDLER;
#endif /* ACE_HAS_MT_ORBIX */

int
Notifier_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  if (this->notifier_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "closing down Notifier_Handler\n"));
      CORBA_HANDLER::instance ()->deactivate_service (Event_Comm_Notifier_IMPL, 
						      this->notifier_->_marker ());
      CORBA::release (this->notifier_);
      this->notifier_ = 0;
      // *Must* be allocated dyanmically!
      delete this;
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
  CORBA_HANDLER::instance ()->activate_service (Event_Comm_Notifier_IMPL, 
						putit ? marker : 0, service_location);

  // Create a notifier object using the implementation class Notifier_i.
  this->notifier_ = 
    new TIE_Event_Comm_Notifier (Notifier_i) (new Notifier_i, marker);
}

// Destroy a Notifier target object.

Notifier_Handler::~Notifier_Handler (void)
{
  this->handle_close ();
}

#endif /* ACE_HAS_ORBIX */
