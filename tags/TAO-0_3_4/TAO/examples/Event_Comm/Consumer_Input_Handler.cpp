// $Id$

#include "Consumer_Input_Handler.h"
#include "Consumer_Handler.h"

ACE_RCSID(Consumer, Consumer_Input_Handler, "$Id$")

Consumer_Input_Handler::Consumer_Input_Handler (void)
  : receiver_handler_ (0),
    consumer_initiated_shutdown_ (0)
{
  // No-Op.
}

Consumer_Input_Handler::~Consumer_Input_Handler (void)
{
  // No-Op.
}

int
Consumer_Input_Handler::consumer_initiated_shutdown (void)
{
  return this->consumer_initiated_shutdown_;
}

void
Consumer_Input_Handler::consumer_initiated_shutdown (int c)
{
  this->consumer_initiated_shutdown_ = c;
}

int
Consumer_Input_Handler::close (void)
{
  ACE_DEBUG ((LM_DEBUG,
	      "closing down Consumer::Input_Handler\n"));

  Event_Comm::Consumer *receiver =
    this->receiver_handler_->receiver ();
  Event_Comm::Notifier *notifier =
    this->receiver_handler_->notifier ();

  if (this->consumer_initiated_shutdown ())
    {
      // Only try to unsubscribe if the Consumer initiated the
      // shutdown.  Otherwise, the Notifier initiated it and it has
      // probably gone away by now!
      ACE_TRY_NEW_ENV
        {
          // Gracefully shutdown the Receiver by removing it from the
          // Notifier's internal map.

	  if (notifier != 0)
	    notifier->unsubscribe (receiver,
				   "",
				   ACE_TRY_ENV);
	  ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_TRY_ENV.print_exception ("Consumer_Input_Handler::handle_close\n");
        }
      ACE_ENDTRY;
    }

  // Make sure to cleanup the STDIN handler.
  if (ACE_Event_Handler::remove_stdin_handler
      (TAO_ORB_Core_instance ()->reactor (),
       TAO_ORB_Core_instance ()->thr_mgr ()) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "remove_stdin_handler"));

  return 0;
}

int Consumer_Input_Handler::initialize (Consumer_Handler *ch)
{
  receiver_handler_ = ch;

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

int
Consumer_Input_Handler::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ];

  // Read up to BUFSIZ worth of data from ACE_HANDLE h.
  ssize_t n = ACE_OS::read (h, buf, sizeof buf - 1);

  if (n > 0)
    {
      // Null terminate the buffer, replacing the '\n' with '\0'.
      if (buf[n - 1] == '\n' || buf[n - 1] == EOF)
	buf[n - 1] = '\0';
      else
	buf[n] = '\0';
      ACE_DEBUG ((LM_DEBUG,
                  "notifying for event %s\n",
                  buf));
    }
  else
    {
      // If nothing is read, do nothing.
      return 0;
    }

  Event_Comm::Notifier *notifier =
    this->receiver_handler_->notifier ();

  ACE_ASSERT (notifier != 0);

  if (ACE_OS::strncmp (buf, "quit", 4) == 0)
    {
      // Consumer wants to shutdown.
      this->consumer_initiated_shutdown (1);

      // Tell the main event loop to shutdown.
       this->receiver_handler_->shutdown ();
    }
  else
    {
      ACE_TRY_NEW_ENV
        {
          Event_Comm::Event event;

          event.tag_ = ACE_OS::strdup (buf);

          notifier->push (event, ACE_TRY_ENV);
	  ACE_TRY_CHECK;
	}
      ACE_CATCHANY
        {
          ACE_TRY_ENV.print_exception("Unexpected exception\n");
        }
      ACE_ENDTRY;
    }

  /* NOTREACHED */
  return 0;
}
