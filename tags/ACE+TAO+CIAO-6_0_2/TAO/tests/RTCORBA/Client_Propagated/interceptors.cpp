// $Id$

#include "interceptors.h"
#include "tao/OctetSeqC.h"
#include "ace/Log_Msg.h"

Client_Request_Interceptor::
Client_Request_Interceptor (const char *id)
  : myname_ ("Client_Request_Interceptor"),
    orb_id_ (CORBA::string_dup (id))
{
}

Client_Request_Interceptor::~Client_Request_Interceptor (void)
{
}

char *
Client_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Client_Request_Interceptor::destroy (void)
{
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do Nothing
}

void
Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do Nothing
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{

  try
    {
      // Try to get the RTCorbaPriority context.  If its not there,
      // report the fact via an exception.
      IOP::ServiceContext_var sc =
        ri->get_reply_service_context (IOP::RTCorbaPriority);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");

      ACE_DEBUG ((LM_DEBUG, "EXCEPTION was most likely caused by the lack "
                            "of an IOP::RTCorbaPriority context in the reply.\n"));
      throw;
    }

}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::receive_exception (
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

  ACE_DEBUG ((LM_DEBUG,
              "%s.received_exception "
              "from \"%s\" on object: %s\n",
              this->myname_,
              operation.in (),
              ior.in ()));

  // Try to get the RTCorbaPriority context.  If its not there,
  // report the fact via an exception.
  IOP::ServiceContext_var sc =
    ri->get_reply_service_context (IOP::RTCorbaPriority);
}
