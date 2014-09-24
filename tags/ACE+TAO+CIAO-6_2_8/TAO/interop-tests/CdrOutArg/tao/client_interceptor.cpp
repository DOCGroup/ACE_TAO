// $Id$

#include "client_interceptor.h"
#include "tao/OctetSeqC.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

const IOP::ServiceId service_id = 0xdeadbeef;
const char *request_msg = "12345678";

Echo_Client_Request_Interceptor::
Echo_Client_Request_Interceptor (const char *id)
  : myname_ ("Echo_Client_Interceptor"),
    orb_id_ (CORBA::string_dup (id))
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor (void)
{
}

char *
Echo_Client_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy (void)
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{

  if (CORBA::is_nil (this->orb_.in ()))
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      this->orb_ = CORBA::ORB_init (argc, argv,
                                    this->orb_id_.in ());
    }

  CORBA::String_var operation = ri->operation ();

  CORBA::Object_var target = ri->target ();

  CORBA::String_var ior =
    this->orb_->object_to_string (target.in ());

#if 0
  ACE_DEBUG ((LM_DEBUG,
              "%C.send_request "
              "from \"%C\" on object: %C\n",
              this->myname_,
              operation.in (),
              ior.in ()));
#endif /*if 0*/



  // Populate target member of the ClientRequestInfo.

  // Make the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = ::service_id;

  CORBA::ULong string_len = ACE_OS::strlen (request_msg) + 1;
  CORBA::Octet *buf = CORBA::OctetSeq::allocbuf (string_len);
  ACE_OS::strcpy (reinterpret_cast<char *> (buf), request_msg);

  sc.context_data.replace (string_len, string_len, buf, 1);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, 0);

  // Check that the request service context can be retrieved.
  IOP::ServiceContext_var sc2 =
    ri->get_request_service_context (::service_id);

  const char *buf2 =
    reinterpret_cast<const char *> (sc2->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf2, request_msg) != 0)
    {
        ACE_ERROR ((LM_ERROR,
                    "ERROR: Expected request service context to be: %C.\n"
                    "  Got: %C\n",
                    request_msg,
                    buf2));
    }

}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr )
{
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr )
{
}

