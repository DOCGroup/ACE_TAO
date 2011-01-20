// $Id$

// Smart_Messenger_Proxy.cpp

#include "Smart_Messenger_Proxy.h"
#include <iostream>

Smart_Messenger_Proxy_Factory::Smart_Messenger_Proxy_Factory(
  CORBA::ORB_ptr orb)
{
  std::cout << "Creating smart proxy factory" << std::endl;
  // Convert the contents of the Logger.ior file to an object reference.
  CORBA::Object_var obj = orb->string_to_object("file://Logger.ior");
  if (CORBA::is_nil(obj.in())) {
    std::cerr << "Nil Logger reference" << std::endl;
    throw 0;
  }

  // Narrow the object reference to a Logger object reference.
  logger_ = Logger::_narrow(obj.in());
  if (CORBA::is_nil(logger_.in ())) {
    std::cerr << "Not a Logger object reference" << std::endl;
    throw 0;
  }
}

Messenger_ptr
Smart_Messenger_Proxy_Factory::create_proxy (
  Messenger_ptr proxy)
{

  Messenger_ptr smart_proxy = 0;
  if (CORBA::is_nil(proxy) == 0)
    smart_proxy = new Smart_Messenger_Proxy(proxy, logger_.in());
  return smart_proxy;
}


Smart_Messenger_Proxy::Smart_Messenger_Proxy(
  Messenger_ptr proxy, Logger_ptr logger)
    : TAO_Smart_Proxy_Base(proxy),
      logger_(Logger::_duplicate(logger))
{
  std::cout << "Creating smart proxy" << std::endl;
}

CORBA::Boolean
Smart_Messenger_Proxy::send_message (
  const char * user_name,
  const char * subject,
  char *& message)
{
  logger_->log_message("Before send_message()");
  CORBA::Boolean ret_val =
    TAO_Messenger_Smart_Proxy_Base::send_message(user_name, subject, message);
  logger_->log_message("After send_message()");
  return ret_val;
}

