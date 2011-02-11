// $Id$

#include "Messenger_i.h"
#include "ace/Thread.h"
#include <iostream>

// Implementation skeleton constructor
Messenger_i::Messenger_i(CORBA::ORB_ptr orb)
{
  CORBA::Object_var obj = orb->resolve_initial_references("RTCurrent");
  rt_current_ = RTCORBA::Current::_narrow(obj.in());
}

// Implementation skeleton destructor
Messenger_i::~Messenger_i()
{
}

CORBA::Boolean
Messenger_i::send_message (const char* user_name,
                           const char* subject,
                           char*& message)
{
  CORBA::Short priority = rt_current_->the_priority();

  ACE_hthread_t current;
  ACE_Thread::self (current);
  int native_priority;

  std::cout << "Message from:    " << user_name << std::endl
            << "Subject:         " << subject   << std::endl
            << "CORBA Priority:  " << priority  << std::endl;
  if (ACE_Thread::getprio (current, native_priority) != -1) {
    std::cout << "Native Priority: " << native_priority  << std::endl;
  }
  std::cout << "Message:         " << message   << std::endl << std::endl;
  return true;
}

