// $Id$

#include "Consumer_Handler.h"

ACE_RCSID(Consumer, Consumer_Handler, "$Id$")

#if defined (ACE_HAS_ORBIX) && (ACE_HAS_ORBIX != 0)

#if defined (ACE_HAS_MT_ORBIX) && (ACE_HAS_MT_ORBIX != 0)
typedef ACE_MT_CORBA_Handler CORBA_HANDLER;
#else
typedef ACE_ST_CORBA_Handler CORBA_HANDLER;
#endif /* ACE_HAS_MT_ORBIX */

int
Consumer_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
// ACE_ST_CORBA_Handler::remove_service (Event_Comm_Consumer_IMPL);

  if (this->receiver_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "closing down Consumer_Handler\n"));
      CORBA_HANDLER::instance ()->deactivate_service (Event_Comm_Consumer_IMPL, 
						      this->receiver_->_marker ());
      CORBA::release (this->receiver_);
      this->receiver_ = 0;
      CORBA::release (this->notifier_);
      this->notifier_ = 0;
      // *Must* be allocated dynamically in order to delete this!
      delete this;
    }
  return 0;
}

Consumer_Handler::Consumer_Handler (int argc, char *argv[])
  : notifier_ (0), 
    receiver_ (0)
{
  const char *server_name =
    Event_Comm_Consumer_IMPL;
  char buf[BUFSIZ];
  char *receiver_marker = buf;
  char *filtering_criteria;
  char *host;
  char *notifier_marker;
  char *service_location = argv[0];

  // First see if we have any environment variables.
  filtering_criteria = ACE_OS::getenv ("FILTERING_CRITERIA");
  host = ACE_OS::getenv ("HOST");
  notifier_marker = ACE_OS::getenv ("NOTIFIER_MARKER");

  // Then override these variables with command-line arguments.
  filtering_criteria = argc > 1 ? argv[1] : "";
  host = argc > 2 ? argv[2] : "tango.cs";
  notifier_marker = argc > 3 ? argv[3] : "notifier:" Event_Comm_Notifier_IR;

  CORBA::Orbix.setDiagnostics (0);

  utsname name;

  // Make the marker name be the "/hostname/processid"
  ACE_OS::uname (&name);
  sprintf (buf, "/%s/%d", name.nodename, ACE_OS::getpid ());

  CORBA_HANDLER::instance ()->activate_service (Event_Comm_Consumer_IMPL, 
						receiver_marker,
                                                service_location);

  // Create the receiver object.
  ACE_NEW (this->receiver_,
           TIE_Event_Comm_Consumer (Consumer_i) 
           (new Consumer_i));
  
  this->receiver_->_marker (receiver_marker);

  ACE_ASSERT (this->receiver_);

  TRY 
    {
      // Get a binding to the notifier.
      this->notifier_ = Event_Comm::Notifier::_bind (notifier_marker, host, IT_X);

      if (this->notifier_ != CORBA::OBJECT_NIL)
        // Subscribe ourselves with the notifier's broker.
        this->notifier_->subscribe (this->receiver_, 
                                    filtering_criteria, IT_X);
    } 
  CATCHANY 
    {
      cerr << "Unexpected exception " << IT_X << endl;
      ACE_OS::exit (1);
    }
  ENDTRY;
  // Print out context.

  receiver_marker = (char *) this->receiver_->_marker ();
  CORBA::BOA::activationMode mode = CORBA::Orbix.myActivationMode ();
  ACE_DEBUG ((LM_DEBUG,
              "starting up a %spersistent server in mode %d with marker name %s\n",
              mode == CORBA::BOA::persistentActivationMode ? "" : "non-", 
              mode,
              receiver_marker));
}

Event_Comm::Consumer *
Consumer_Handler::receiver (void)
{
  return this->receiver_;
}

Event_Comm::Notifier *
Consumer_Handler::notifier (void)
{
  return this->notifier_;
}

// Destroy a Receiver target object.

Consumer_Handler::~Consumer_Handler (void)
{
  this->handle_close (-1,
                      ACE_Event_Handler::ALL_EVENTS_MASK);
}

#endif /* ACE_HAS_ORBIX */
