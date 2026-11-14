/* -*- C++ -*-  */

#include "Messenger_i.h"
#include <iostream>

CORBA::Boolean
Messenger_i::send_message (char const *user_name,
                           char const *subject,
                           char *&message)
{
  std::cout << "Message from: " << user_name << std::endl;
  std::cout << "Subject:      " << subject << std::endl;
  std::cout << "Message:      " << message << std::endl;
  return true;
}
