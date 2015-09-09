/* -*- C++ -*-  */

#include "Messenger_i.h"
#include "Client_Task.h"

// Implementation skeleton constructor
Messenger_i::Messenger_i (CORBA::ORB_var orb)
  : orb_ (orb)
{
}

// Implementation skeleton destructor
Messenger_i::~Messenger_i (void)
{
}

CORBA::Boolean
Messenger_i::send_message (const char *, const char *, char *&)
{
  ACE_DEBUG((LM_DEBUG, "Inside send_message()....\n"));
  const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
  CORBA::Object_var obj =
    orb_->string_to_object (ior);

  // Narrow the Messenger object reference
  Messenger_var messenger = Messenger::_narrow(obj.in());
  if (CORBA::is_nil(messenger.in()))
  {
    ACE_ERROR((LM_ERROR, "ERROR: Not a Messenger reference\n"));
    return 1;
  }

  Client_Task client_task (orb_.in (),
                           messenger.in (),
                           ACE_Thread_Manager::instance ());

  if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                            1,
                            1) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
    }

  // Wait for the client and server to finish
  ACE_Thread_Manager::instance ()->wait ();

  return client_task.result_;
}

CORBA::Boolean
Messenger_i::call_message (const char * user_name)
{
    ACE_DEBUG((LM_DEBUG, "Inside call_message()....\n"));
    ACE_DEBUG((LM_DEBUG, "Message from: %s\n", user_name));
    return true;
}

void
Messenger_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
