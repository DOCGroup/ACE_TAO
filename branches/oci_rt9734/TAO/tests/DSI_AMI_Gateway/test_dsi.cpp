// $Id$

#include "test_dsi.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/Unknown_User_Exception.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"

#if !defined(__ACE_INLINE__)
#include "test_dsi.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(DSI_Gateway, test_dsi, "$Id$")

void
DSI_Simple_Server::invoke (CORBA::ServerRequest_ptr request)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  //marshal_demarshal_=true;
  if(marshal_demarshal_)
  {
  CORBA::NVList_ptr list;
  this->orb_->create_list (0, list);

  request->arguments (list);

  CORBA::Request_var target_request;

  this->target_->_create_request (0, // ctx
                                  request->operation (),
                                  list,
                                  0, // result
                                  0, // exception_list,
                                  0, // context_list,
                                  target_request.inout (),
                                  0);

  target_request->_tao_lazy_evaluation (1);

  // Outgoing request must have the same byte order as the incoming one.
  target_request->_tao_byte_order (request->_tao_incoming_byte_order ());

  try
    {
      // Updates the byte order state, if necessary.
      target_request->invoke ();
    }
  catch (const CORBA::UNKNOWN& ex)
    {
      ACE_UNUSED_ARG (ex);

      // Outgoing reply must have the same byte order as the incoming one.
      request->_tao_reply_byte_order (target_request->_tao_byte_order ());

      request->gateway_exception_reply (target_request->raw_user_exception ());

      return;
    }

  // Outgoing reply must have the same byte order as the incoming one.
  request->_tao_reply_byte_order (target_request->_tao_byte_order ());

  }
  else
  {
    // forward the request without marshalling and demarshalling.
    TAO_ServerRequest & tao_server_request = request->_tao_server_request ();
    TAO_Transport* request_ptr = tao_server_request.transport ();
    TAO_Stub *stubobj = target_ ->_stubobj ();

    // the following code is doing the copy but with memory leak
    TAO_OutputCDR outcdr (tao_server_request.incoming ()->start ()->duplicate ());
    outcdr.write_octet_array_mb(tao_server_request.incoming ()->start ());

      ACE_DEBUG((LM_DEBUG,"the length of incoming cdr = %d\n"
                        "the length of outgoing cdr = %d\n"
                        "the length of my outcdr = %d\n",
                        tao_server_request.incoming()->length(),
                        tao_server_request.outgoing()->length(),
                        outcdr.length ()));
    request_ptr->send_message ( outcdr,
                                stubobj,
                                TAO_Transport::TAO_ONEWAY_REQUEST);

  }
  if (ACE_OS::strcmp ("shutdown", request->operation ()) == 0)
    {
      this->orb_->shutdown (0);
    }
}

CORBA::RepositoryId
DSI_Simple_Server::_primary_interface (const PortableServer::ObjectId &,
                                       PortableServer::POA_ptr)
    ACE_THROW_SPEC (())
{
  return CORBA::string_dup ("IDL:Simple_Server:1.0");
}

PortableServer::POA_ptr
DSI_Simple_Server::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
