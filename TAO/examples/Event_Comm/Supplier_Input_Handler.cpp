// $Id$

#include "Event_Comm_i.h"
#include "Notifier_Handler.h"
#include "Supplier_Input_Handler.h"

ACE_RCSID(Supplier, Supplier_Input_Handler, "$Id$")

Supplier_Input_Handler::Supplier_Input_Handler ()
  : handle_ (0),
    notifier_ (0)
{
  // No-Op.
}

Supplier_Input_Handler::~Supplier_Input_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "closing down Supplier_Input_Handler::~Supplier_Input_Handler\n"));
}

int
Supplier_Input_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              "closing down Supplier::Supplier_Input_Handler\n"));

  Event_Comm::Notifier *notifier = this->notifier_->notifier ();
  ACE_ASSERT (notifier != 0);

  ACE_OS::fclose (this->fp_);

  TAO_TRY
    {
      // Disconnect all the consumers gracefully.
      notifier->disconnect ("quit",
			    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Error:Supplier_Input_Handler::handle_close\n");
    }
  TAO_ENDTRY;

  if (ACE_Reactor::instance ()->remove_handler
      (this,
       // Don't execute a callback here otherwise we'll recurse
       // indefinitely!
       ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "remove_handler"));
  return 0;
}

int
Supplier_Input_Handler::initialize (Notifier_Handler *notifier,
				    ACE_HANDLE handle) // Use stdin by default.
{
  notifier_ = notifier;
  handle_ = handle;

  // Register ourselves with the ACE_Reactor so that input events
  // cause our handle_input() method to be dispatched automatically.

  if (ACE_Reactor::instance ()->register_handler
      (this,
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                "%p\n",
                "register_handler"), -1);

  this->fp_ = ACE_OS::fdopen (handle, "r");

  if (this->fp_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "fdopen"), -1);
  return 0;
}

ACE_HANDLE
Supplier_Input_Handler::get_handle (void) const
{
  return this->handle_;
}

// Frame input events and notify <Consumers>.

int
Supplier_Input_Handler::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ];

  // Read up to BUFSIZ worth of data from ACE_HANDLE h.

  if (ACE_OS::fgets (buf,
		     sizeof buf - 1,
		     this->fp_) == 0)
    {
      ACE_OS::strcpy (buf,
		      "quit");
      ACE_DEBUG ((LM_DEBUG,
                  "shutting down Supplier_Input_Handler\n"));
    }
  else
    {
      size_t n = ACE_OS::strlen (buf);

      // Null terminate the buffer, replacing the '\n' with '\0'.
      if (buf[n - 1] == '\n' || buf[n - 1] == EOF)
	buf[n - 1] = '\0';
      else
	buf[n] = '\0';
      ACE_DEBUG ((LM_DEBUG,
		  "notifying for event %s\n",
		  buf));
    }

  Event_Comm::Notifier *notifier = this->notifier_->notifier ();
  ACE_ASSERT (notifier != 0);

  if (ACE_OS::strcmp (buf, "quit") == 0)
    // Tell the main event loop to shutdown.
    ACE_Reactor::end_event_loop ();
  else
    {
      // Use the notifier to notify Consumers.
      TAO_TRY
        {
          Event_Comm::Event event;

          // Pass the buf over in the tag field.
          event.tag_ = ACE_OS::strdup (buf);

          // This is where the "any" value goes or the object
          // reference...  event.value_ = ...

          // Forward <Event> to all <Consumers>.
          notifier->push (event, TAO_TRY_ENV);
	  TAO_CHECK_ENV;
        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("Unexpected Error\n");
        }
      TAO_ENDTRY;
    }
  return 0;
}
