// -*- C++ -*-

#include "Messenger_i.h"
#include <iostream>

CORBA::Boolean
Messenger_i::send_message (char const *user_name,
                           char const *subject,
                           char *&message)
{
  std::cerr << "Message from: " << user_name << std::endl;
  std::cerr << "Subject:      " << subject << std::endl;
  std::cerr << "Message:      " << message << std::endl;
  return true;
}

