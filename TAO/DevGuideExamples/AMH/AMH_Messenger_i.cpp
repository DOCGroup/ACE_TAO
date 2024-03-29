#include "amh_pch.h"
/* -*- C++ -*-  */

// ******  Code generated by the The ACE ORB (TAO) IDL Compiler *******
// TAO and the TAO IDL Compiler have been developed by the Center for
// Distributed Object Computing at Washington University, St. Louis.
//
// Information about TAO is available at:
//                 http://www.dre.vanderbilt.edu/~schmidt/TAO.html

#include "AMH_Messenger_i.h"
#include <iostream>
// Implementation skeleton constructor
AMH_Messenger_i::AMH_Messenger_i ()
{
}

// Implementation skeleton destructor
AMH_Messenger_i::~AMH_Messenger_i ()
{
}

void
AMH_Messenger_i::send_message (
    DevGuide::AMH_MessengerResponseHandler_ptr _tao_rh,
    const char * user_name,
    const char * subject,
    const char * message
  )
{
  std::cout << "Message from: " << user_name << std::endl;
  std::cout << "Subject:      " << subject << std::endl;
  std::cout << "Message:      " << message << std::endl;
  CORBA::String_var inout_message_arg =
    CORBA::string_dup("Thanks for the message.");
  CORBA::Boolean result = true;

  _tao_rh->send_message(result,inout_message_arg.inout ());
}
