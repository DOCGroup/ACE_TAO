// $Id$

#include "Event_Comm_i.h"
#include "Notifier_Handler.h"
#include "Supplier_Input_Handler.h"

ACE_RCSID(Supplier, Supplier_Input_Handler, "$Id$")

Supplier_Input_Handler::Supplier_Input_Handler ()
  : //handle_ (0),
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
Supplier_Input_Handler::handle_close (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "closing down Supplier::Supplier_Input_Handler\n"));

  // Make sure to cleanup the STDIN handler.
   if (ACE_Event_Handler::remove_stdin_handler
      (TAO_ORB_Core_instance ()->reactor (),
       TAO_ORB_Core_instance ()->thr_mgr ()) == -1)
    ACE_ERROR ((LM_ERROR,
		       "%p\n",
		       "remove_stdin_handler"));

   return 0;
}

int
Supplier_Input_Handler::initialize (Notifier_Handler *notifier)			  
{
  notifier_ = notifier;
  // Register our <Input_Handler> to handle STDIN events, which will
  // trigger the <handle_input> method to process these events.

  if (ACE_Event_Handler::register_stdin_handler
      (this,
       TAO_ORB_Core_instance ()->reactor (),
       TAO_ORB_Core_instance ()->thr_mgr ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%p\n",
		       "register_stdin_handler"),
		      -1);

  
  return 0;
}

// Frame input events and notify <Consumers>.

int
Supplier_Input_Handler::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ];

  // Read up to BUFSIZ worth of data from ACE_HANDLE h.

  if (ACE_OS::fgets (buf,
		     sizeof buf - 1,
			 stdin) == 0)
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
