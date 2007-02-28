//$Id$

#include "ProxyServant.h"

TAO_ProxyServant::TAO_ProxyServant ()
{
}

TAO_ProxyServant::~TAO_ProxyServant ()
{
}

void
TAO_ProxyServant::_dispatch (TAO_ServerRequest &request,
                         void * //context
                        )
{
  // No need to do any of this if the client isn't waiting.
  if (request.response_expected ())
    {
      if (!CORBA::is_nil (request.forward_location ()))
        {
          request.init_reply ();
          request.tao_send_reply ();

          // No need to invoke in this case.
          return;
        }
      else if (request.sync_with_server ())
        {
          // The last line before the call to this function
          // was an ACE_CHECK_RETURN, so if we're here, we
          // know there is no exception so far, and that's all
          // a SYNC_WITH_SERVER client request cares about.
          request.send_no_exception_reply ();
        }
    }

  // Create DSI request object.
  CORBA::ServerRequest *dsi_request = 0;
  ACE_NEW (dsi_request,
           CORBA::ServerRequest (request));
    try
    {
      TAO_AMH_DSI_Response_Handler_ptr rh_ptr = 0;
      ACE_NEW (rh_ptr, TAO_AMH_DSI_Response_Handler(request));

      TAO_AMH_DSI_Response_Handler_var rh = rh_ptr;

      // init the handler
      TAO_AMH_BUFFER_ALLOCATOR* amh_allocator =
              request.orb()->orb_core ()->lane_resources().amh_response_handler_allocator();
      rh->init (request, amh_allocator);
      // Delegate to user.
      this->invoke (dsi_request,
                    rh.in());
    }
  catch (CORBA::Exception, ex)
    {
      // Only if the client is waiting.
      if (request.response_expected () && !request.sync_with_server ())
        {
          request.tao_send_reply_exception (ex);
        }
    }
  CORBA::release (dsi_request);
}

TAO_ProxyServant::invoke(CORBA::ServerRequest_ptr request,
                     TAO_AMH_DSI_Response_Handler_ptr rh)
  throw (CORBA::SystemException)
{
  if ( ACE_OS::strcmp ("_get_interface", request->operation ()) == 0)
    this->invoke_get_interface (request);
  else if ( ACE_OS::strcmp ("_primary_interface", request->operation ()) == 0)
    this->invoke_primary_interface (request);
  else
    this->invoke_i (request, rh);
}

void
TAO_ProxyServant::invoke_get_interface (CORBA::ServerRequest_ptr request)
{
  CORBA::NVList_ptr args;
  this->orb_->create_list (0, args );
  request->arguments (args );

  CORBA::InterfaceDef_var theIntDef = this->_get_interface ();
  CORBA::Any theResult;
  theResult.type (CORBA::_tc_Object);
  theResult <<= theIntDef.in ();
  request->set_result (theResult);
}

void
TAO_ProxyServant::invoke_primary_interface (CORBA::ServerRequest_ptr request)
{
  CORBA::NVList_ptr args;
  this->orb_->create_list (0, args);
  request->arguments (args);
  CORBA::String_var rep_id =
    this->interface_repository_id();

  CORBA::Any a;
  a.type(CORBA::_tc_string);
  a <<= rep_id.in ();
  request->set_result (a);
}
