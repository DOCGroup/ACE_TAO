#include "Event_Comm.hh"
// $Id$

#include "Notifier_Handler.h"
#include "Input_Handler.h"

#if defined (ACE_HAS_ORBIX) && (ACE_HAS_ORBIX != 0)

int
Input_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "closing down Supplier::Input_Handler\n"));

  Event_Comm::Notifier *notifier = this->notifier_->notifier ();
  ACE_ASSERT (notifier != 0);

  ACE_OS::fclose (this->fp_);

  TRY {
    // Disconnect all the consumers gracefully.
    notifier->send_disconnect ("quit", IT_X);
  } CATCHANY {
    cerr << IT_X << endl;
  } ENDTRY;

  // Don't execute a callback here otherwise we'll recurse indefinitely!
  if (ACE_Reactor::instance()->remove_handler 
      (this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "remove_handler"));

  // *Must* be allocated dyanmically!
  delete (void *) this;
  return 0;
}

Input_Handler::Input_Handler (Notifier_Handler *notifier,
			      ACE_HANDLE handle) // Use stdin by default.
  : notifier_ (notifier),
    handle_ (handle)
{
  // Register ourselves with the ACE_Reactor so that input events 
  // cause our handle_input() method to be dispatched automatically.

  if (ACE_Reactor::instance()->register_handler (this, 
						 ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));

  this->fp_ = ACE_OS::fdopen (handle, "r");

  if (this->fp_ == 0)
    ACE_ERROR ((LM_ERROR, "%p\n", "fdopen"));
}

Input_Handler::~Input_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG, "closing down Input_Handler::~Input_Handler\n"));
  this->handle_close ();
}

ACE_HANDLE
Input_Handler::get_handle (void) const
{
  return this->handle_;
}

// Frame input events and notify <Consumers>.

int
Input_Handler::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ];
  
  // Read up to BUFSIZ worth of data from ACE_HANDLE h.

  if (ACE_OS::fgets (buf, sizeof buf - 1, this->fp_) == 0)
    {
      ACE_OS::strcpy (buf, "quit");
      ACE_DEBUG ((LM_DEBUG, "shutting down Input_Handler\n"));
    }    
  else
    {
      size_t n = ACE_OS::strlen (buf);

      // Null terminate the buffer, replacing the '\n' with '\0'.
      if (buf[n - 1] == '\n' || buf[n - 1] == EOF)
	buf[n - 1] = '\0';
      else
	buf[n] = '\0';
      ACE_DEBUG ((LM_DEBUG, "notifying for event %s\n", buf));
    }

  Event_Comm::Notifier *notifier = this->notifier_->notifier ();
  ACE_ASSERT (notifier != 0);

  if (ACE_OS::strcmp (buf, "quit") == 0)
    // Tell the main event loop to shutdown. 
    ACE_Reactor::end_event_loop();
  else
    {
      // Use the notifier to notify Consumers.
      TRY {
	Event_Comm::Notification notification;

	// Pass the buf over in the tag field.
	notification.tag_ = ACE_OS::strdup (buf);

	// This is where the "any" value goes or the object reference...
	// notification.value_ = ...

	// Forward <Notification> to all <Notification_Receivers>.
	notifier->send_notification (notification, IT_X);
      }
      CATCHANY {
	cerr << "unexpected exception " << IT_X << endl;
      } ENDTRY;
    }
  return 0;
}

#endif /* ACE_HAS_ORBIX */
