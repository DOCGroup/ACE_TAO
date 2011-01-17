// $Id$

#include "ClientInterceptor.h"
#include "tao/OctetSeqC.h"
#include "tao/PI/ClientRequestInfo.h"
#include "MessengerC.h"
#include "ace/OS_NS_string.h"
#include <iostream>

const CORBA::ULong service_ctx_id = 0xdeed;

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

ClientInterceptor::~ClientInterceptor (void)
{
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
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling send_poll()." << std::endl;
}


void
ClientInterceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  std::cout << "Calling send_request()." << std::endl;

  IOP::ServiceContext sc;
  sc.context_id = service_ctx_id;

  const char user_name[] = "Ron Klein";
  std::cout << "User's Name: " << user_name << std::endl;
  CORBA::ULong string_len = sizeof (user_name) + 1;
  CORBA::Octet *buf = 0;
  buf = new CORBA::Octet [string_len];

  ACE_OS::strcpy (reinterpret_cast<char*> (buf), user_name);

  sc.context_data.replace (string_len, string_len, buf, true);

  // recursive call setup
  CORBA::Any *recurse = ri->get_slot(slot);
  CORBA::Boolean x;
  CORBA::Any flag;
  if (*recurse >>= CORBA::Any::to_boolean(x)) {
    if (x == false)
      {
        x = true;
        flag <<= CORBA::Any::from_boolean(x);

        pic->set_slot(slot, flag);

        // get server time
        std::cout << "Server Time = " << messenger->get_time() << std::endl;
      }
  } else {
    std::cerr << "Could not extract a boolean value from any" << std::endl;
  }

  // Add this context to the service context list.
  ri->add_request_service_context (sc, false);

  // reset recursion test
  x = false;
  flag <<= CORBA::Any::from_boolean(x);
  pic->set_slot(slot,flag);

}

void
ClientInterceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_reply()." << std::endl;
}

void
ClientInterceptor::receive_other (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_other()." << std::endl;
}

void
ClientInterceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_exception()." << std::endl;
}
