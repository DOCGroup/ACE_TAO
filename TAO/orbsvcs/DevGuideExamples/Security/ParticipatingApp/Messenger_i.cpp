/* -*- C++ -*-  */

#include "Messenger_i.h"
#include <iostream>

Messenger_i::Messenger_i (CORBA::ORB_ptr orb, SSLIOP::Current_ptr ssliop_current)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    ssliop_current_ (SSLIOP::Current::_duplicate (ssliop_current))
{
}

CORBA::Boolean
Messenger_i::send_message (char const *user_name,
                           char const *subject,
                           char *&message)
{
  if (ssliop_current_->no_context ())
    std::cout << "Message from:        " << user_name << std::endl;
  else
    std::cout << "SECURE message from: " << user_name << std::endl;

  std::cout << "Subject:             " << subject << std::endl;
  std::cout << "Message:             " << message << std::endl << std::endl;
  return true;
}

void
Messenger_i::shutdown (char const *user_name)
{
  if (!ssliop_current_->no_context ())
    {
      // Requestor is authentic, so shut the server down.
      std::cout << "Shutdown command from: " << user_name << std::endl;
      std::cout << "Status:                User authenticated." << std::endl;
      std::cout << "Action:                Server shutdown in progress..." << std::endl << std::endl;
      orb_->shutdown (false);
    }
  else
    {
      std::cout << "Shutdown command from: " << user_name << std::endl;
      std::cout << "Status:                User *NOT* authenticated." << std::endl;
      std::cout << "Action:                Ignored." << std::endl << std::endl;
    }
}
