#include "ClientInterceptor.h"
#include "tao/OctetSeqC.h"
#include "tao/PI/ClientRequestInfo.h"
#include "MessengerC.h"

#include <cstring>
#include <iostream>
#include <memory>

constexpr CORBA::ULong service_ctx_id = 0xdeed;

ClientInterceptor::
ClientInterceptor (Messenger_var theMessenger,
                   PortableInterceptor::Current_ptr thePic,
                   PortableInterceptor::SlotId theSlot)
  : myname_ ("Client_Authentication_Interceptor")
{
  std::cout << "Calling ClientInterceptor constructor." << std::endl;
  this->messenger = theMessenger;
  this->pic = thePic;
  this->slot = theSlot;
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
ClientInterceptor::send_poll (
                              PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling send_poll()." << std::endl;
}


void
ClientInterceptor::send_request (
                                 PortableInterceptor::ClientRequestInfo_ptr ri)
{
  std::cout << "Calling send_request()." << std::endl;

  IOP::ServiceContext sc;
  sc.context_id = service_ctx_id;

  char const user_name[] = "Ron Klein";
  std::cout << "User's Name: " << user_name << std::endl;
  CORBA::ULong const string_len = sizeof (user_name);
  std::unique_ptr<CORBA::Octet[]> buf =
    std::make_unique<CORBA::Octet[]> (string_len);

  std::strcpy (reinterpret_cast<char *> (buf.get ()), user_name);

  sc.context_data.replace (string_len, string_len, buf.get (), true);
  buf.release ();

  // recursive call setup
  CORBA::Any * const recurse = ri->get_slot (slot);
  CORBA::Long x {};
  *recurse >>= x;

  CORBA::Any flag;
  if (x == 0)
    {
      flag <<= 1;

      pic->set_slot(slot, flag);

      // get server time
      CORBA::String_var server_time = messenger->get_time ();
      std::cout << "Server Time = " << server_time.in () << std::endl;
    }
  // Add this context to the service context list.
  ri->add_request_service_context (sc, false);

  // reset recursion test
  flag <<= 0;
  pic->set_slot(slot,flag);
}

void
ClientInterceptor::receive_reply (
                                  PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_reply()." << std::endl;
}

void
ClientInterceptor::receive_other (
                                  PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_other()." << std::endl;
}

void
ClientInterceptor::receive_exception (
                                      PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_exception()." << std::endl;
}


