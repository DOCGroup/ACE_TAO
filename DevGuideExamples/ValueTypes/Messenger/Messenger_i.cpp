// $Id$

#include "_pch.h"

#include "Messenger_i.h"

#include <sstream>


CORBA::Boolean Messenger_i::send_message (Message*& msg)
{
  msg->print();

  msg->user("Son");
  msg->addAddress("Mom");
  msg->addAddress("Dad");

  std::ostringstream out;
  CORBA::String_var sub = msg->subject();
  out << "RE: " << sub.in();
  msg->subject(out.str().c_str());
  msg->text("Ok. I'm on my way.");

  return true;
}

