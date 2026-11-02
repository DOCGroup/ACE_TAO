#include "ClientInterceptor.h"
#include "tao/OctetSeqC.h"
#include "tao/PI/ClientRequestInfo.h"
#include "MessengerC.h"
#include <cstring>
#include <iostream>

constexpr CORBA::ULong service_ctx_id = 0xdeed;

ClientInterceptor::
ClientInterceptor (Messenger_ptr theMessenger,
                   PortableInterceptor::Current_ptr thePic,
                   PortableInterceptor::SlotId theSlot)
  : myname_ ("Client_Authentication_Interceptor"),
    messenger_ (Messenger::_duplicate (theMessenger)),
    current_ (PortableInterceptor::Current::_duplicate (thePic)),
    slot_ (theSlot)
{
  std::cout << "Calling ClientInterceptor constructor." << std::endl;
}

char *
ClientInterceptor::name ()
{
  std::cout << "Calling ClientInterceptor name() method" << std::endl;
  return CORBA::string_dup (this->myname_);
}

void
ClientInterceptor::destroy ()
{
}

void
ClientInterceptor::send_poll (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG (ri);
  std::cout << "Calling send_poll()." << std::endl;
}


void
ClientInterceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  std::cout << "Calling send_request()." << std::endl;

  IOP::ServiceContext sc;
  sc.context_id = service_ctx_id;

  char const user_name[] = "Ron Klein";
  std::cout << "User's Name: " << user_name << std::endl;
  CORBA::ULong const string_len = sizeof (user_name);
  sc.context_data.length (string_len);
  std::memcpy (sc.context_data.get_buffer (), user_name, string_len);

  // recursive call setup
  CORBA::Any *recurse = ri->get_slot (slot_);
  CORBA::Boolean x = false;
  CORBA::Any flag;
  if (*recurse >>= CORBA::Any::to_boolean (x)) {
    if (x == false)
      {
        x = true;
        flag <<= CORBA::Any::from_boolean(x);

        current_->set_slot (slot_, flag);

        // get server time
        CORBA::String_var server_time = messenger_->get_time ();
        std::cout << "Server Time = " << server_time.in () << std::endl;
      }
  }
  else
  {
    std::cerr << "Could not extract a boolean value from any" << std::endl;
  }

  // Add this context to the service context list.
  ri->add_request_service_context (sc, false);

  // reset recursion test
  x = false;
  flag <<= CORBA::Any::from_boolean(x);
  current_->set_slot (slot_, flag);
}

void
ClientInterceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG (ri);
  std::cout << "Calling receive_reply()." << std::endl;
}

void
ClientInterceptor::receive_other (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG (ri);
  std::cout << "Calling receive_other()." << std::endl;
}

void
ClientInterceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG (ri);
  std::cout << "Calling receive_exception()." << std::endl;
}
