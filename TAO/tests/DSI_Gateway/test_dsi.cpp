#include "test_dsi.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/Unknown_User_Exception.h"
#include <cstring>

#if !defined(__ACE_INLINE__)
#include "test_dsi.inl"
#endif /* __ACE_INLINE__ */

void
DSI_Simple_Server::invoke (CORBA::ServerRequest_ptr request)
{
  CORBA::NVList_ptr list {};
  this->orb_->create_list (0, list);

  request->arguments (list);

  CORBA::Request_var target_request {};

  this->target_->_create_request (nullptr, // ctx
                                  request->operation (),
                                  list,
                                  nullptr, // result
                                  nullptr, // exception_list,
                                  nullptr, // context_list,
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
  catch (CORBA::UNKNOWN const &)
    {
      // Outgoing reply must have the same byte order as the incoming one.
      request->_tao_reply_byte_order (target_request->_tao_byte_order ());

      request->gateway_exception_reply (target_request->raw_user_exception ());

      return;
    }

  // Outgoing reply must have the same byte order as the incoming one.
  request->_tao_reply_byte_order (target_request->_tao_byte_order ());

  if (std::strcmp ("shutdown", request->operation ()) == 0)
    {
      this->orb_->shutdown (false);
    }
}

CORBA::RepositoryId
DSI_Simple_Server::_primary_interface (const PortableServer::ObjectId &,
                                       PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:Simple_Server:1.0");
}

PortableServer::POA_ptr
DSI_Simple_Server::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
