// $Id$

#include "Messenger_i.h"
#include <iostream>


CORBA::Boolean Messenger_i::send_message(
  const char * user_name,
  const char * subject,
  char *& message)
{
  std::cout << "Message from: " << user_name << std::endl
            << "Subject:      " << subject << std::endl
            << "Message:      " << message << std::endl;
  message = CORBA::string_dup("A reply.");
  return 1;
}

