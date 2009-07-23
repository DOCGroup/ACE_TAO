// $Id$

#include "MessengerHandler.h"
#include "ace/OS_NS_time.h"
#include <iostream>

// Implementation skeleton constructor
MessengerHandler::MessengerHandler (CORBA::ORB_ptr orb)
: time_(0)
, orb_(CORBA::ORB::_duplicate(orb))
{
}

// Implementation skeleton destructor
MessengerHandler::~MessengerHandler (void)
{
}

void MessengerHandler::send_message (
   CORBA::Boolean ami_return_val,
   const char * message,
   CORBA::Long time
   )
{
  if (ami_return_val)
  {
    time_ = time;
    time_t t = time_;
    const ACE_TCHAR * time_str = ACE_OS::ctime(&t);
    if (time_str != 0) {
      std::cout << std::endl << "Message sent at " <<
        ACE_TEXT_ALWAYS_CHAR (time_str) << std::endl;
    }
    std::cout << "Content of message: " << message << std::endl;
  }
  else
  {
    std::cerr << "Error: Message was not sent." << std::endl;
  }
  // Our simple test just shuts down after sending one message.
  orb_->shutdown(false);
}

void MessengerHandler::send_message_excep (Messaging::ExceptionHolder* excep_holder)
{
  // We'll print an error message and shut down the orb
  try
  {
    excep_holder->raise_exception();
  }
  catch(const CORBA::Exception& ex)
  {
    std::cerr << "A CORBA Exception was thrown: " << ex << std::endl;
  }
  orb_->shutdown(false);
}
