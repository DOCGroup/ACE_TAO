#include "ClientInterceptor.h"
#include "tao/OctetSeqC.h"
#include "tao/PI/ClientRequestInfo.h"
#include "MessengerC.h"

#include <cstring>
#include <iostream>
#include <memory>

constexpr CORBA::ULong service_ctx_id = 0xdeed;

ClientInterceptor::
ClientInterceptor (Messenger_var messenger,
                   PortableInterceptor::Current_ptr current,
                   PortableInterceptor::SlotId slot)
  : myname_ ("Client_Authentication_Interceptor"),
    messenger_ (messenger),
    current_ (PortableInterceptor::Current::_duplicate (current)),
    slot_ (slot)
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

  IOP::ServiceContext sc {};
  sc.context_id = service_ctx_id;

  char const user_name[] = "Ron Klein";
  std::cout << "User's Name: " << user_name << std::endl;
  CORBA::ULong const string_len = sizeof (user_name);
  std::unique_ptr<CORBA::Octet[]> buffer =
    std::make_unique<CORBA::Octet[]> (string_len);

  std::memcpy (buffer.get (), user_name, string_len);

  sc.context_data.replace (string_len, string_len, buffer.get (), true);
  buffer.release ();

  CORBA::Any * const recurse = ri->get_slot (slot_);
  CORBA::Long x {};
  *recurse >>= x;

  CORBA::Any flag {};
  if (x == 0)
    {
      flag <<= 1;

      current_->set_slot (slot_, flag);

      CORBA::String_var const server_time = messenger_->get_time ();
      std::cout << "Server Time = " << server_time.in () << std::endl;
    }
  ri->add_request_service_context (sc, false);

  flag <<= 0;
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
