// $Id$

#include "Event_Comm_i.h"
#include "Notifier_Handler.h"
#include "Supplier_Input_Handler.h"
#include "tao/ORB_Core.h"

Supplier_Input_Handler::Supplier_Input_Handler ()
  : notifier_ (0)
{
  // No-Op.
}

Supplier_Input_Handler::~Supplier_Input_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "closing down Supplier_Input_Handler::~Supplier_Input_Handler\n"));
}

int
Supplier_Input_Handler::close (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "closing down Supplier::Supplier_Input_Handler\n"));

  // Make sure to cleanup the STDIN handler.
   if (ACE_Event_Handler::remove_stdin_handler
      (
       TAO_ORB_Core_instance ()->reactor (),
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
Supplier_Input_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  // Read up to BUFSIZ worth of data from ACE_HANDLE h.

  if (ACE_OS::fgets (buf,
                     sizeof buf - 1,
                     stdin) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "shutting down Supplier_Input_Handler\n"));
      return 0;
    }
  else
    {
      size_t n = ACE_OS::strlen (buf);

      // Null terminate the buffer, replacing the '\n' with '\0'.
      if (buf[n - 1] == '\n')
        buf[n - 1] = '\0';
      else
        buf[n] = '\0';
      ACE_DEBUG ((LM_DEBUG,
                  "notifying for event %s\n",
                  buf));
    }



  if (ACE_OS::strncmp (buf, "quit", 4) == 0)
    // Tell the main event loop to shutdown.
    this->notifier_->shutdown ();
  else
    {
      Event_Comm::Notifier *notifier = this->notifier_->notifier ();
      ACE_ASSERT (notifier != 0);

      // Use the notifier to notify Consumers.
      try
        {
          Event_Comm::Event event;

          // Pass the buf over in the tag field.
          event.tag_ = ACE_OS::strdup (buf);

          // This is where the "any" value goes or the object
          // reference...  event.value_ = ...

          // Forward <Event> to all <Consumers>.
          notifier->push (event);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Unexpected Error\n");
        }
    }
  return 0;
}
