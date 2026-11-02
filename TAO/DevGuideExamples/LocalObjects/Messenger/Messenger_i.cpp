#include "Messenger_i.h"
#include <iostream>

// Implementation skeleton constructor
Messenger_i::Messenger_i ()
{
}

CORBA::Boolean Messenger_i::send_message (
  char const *user_name,
  char const *subject,
  char *& message)
{
  std::cout << "Message from: " << user_name << std::endl;
  std::cout << "Subject:      " << subject << std::endl;
  std::cout << "Message:      " << message << std::endl;
  CORBA::string_free(message);
  message = CORBA::string_dup("Thanks for the message.");
  return true;
}

