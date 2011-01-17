/* -*- C++ -*- $Id$ */

#include "Messenger_i.h"
#include <iostream>
// Implementation skeleton constructor
Messenger_i::Messenger_i (void)
  {
  }

// Implementation skeleton destructor
Messenger_i::~Messenger_i (void)
  {
  }

CORBA::Boolean Messenger_i::send_message (
    const char * user_name,
    const char * subject,
    char *& message
  )
  {
    std::cout << "Message from: " << user_name << std::endl;
    std::cout << "Subject:      " << subject << std::endl;
    std::cout << "Message:      " << message << std::endl;
    return 1;
  }
