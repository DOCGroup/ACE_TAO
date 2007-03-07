//
// $Id$
//
#include "Roundtrip.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/SystemExceptionA.h"
#include "tao/DynamicInterface/Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Roundtrip::invoke (CORBA::ServerRequest_ptr request,
                   TAO_AMH_DSI_Response_Handler_ptr rh)
{
  if (ACE_OS::strcmp ("shutdown", request->operation ()) == 0)
    {
      this->orb_->shutdown (0);

      return;
    }

  else if (ACE_OS::strcmp ("_is_a", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list);

      CORBA::Any type_id;
      type_id._tao_set_typecode (CORBA::_tc_string);
      list->add_value ("type_id", type_id, CORBA::ARG_IN);

      request->arguments (list);

      CORBA::NamedValue_ptr nv = list->item (0);

      const char *arg;
      *(nv->value ()) >>= arg;

      CORBA::Boolean type_matches = false;
      if (ACE_OS::strcmp (arg, "IDL:Test/Roundtrip:1.0") == 0
          || ACE_OS::strcmp (arg, "IDL:omg.org/CORBA/Object:1.0") == 0
          || ACE_OS::strcmp (arg, "") == 0)
        type_matches = true;

      CORBA::Any result_any;
      result_any <<= CORBA::Any::from_boolean (type_matches);

      //request->set_result (result);

      // AMH way of replying
      CORBA::NamedValue_ptr result = 0;
      this->orb_->create_named_value (result);
      *(result->value()) = result_any;
      rh->invoke_reply (list, result);
      return;
    }

  else if (ACE_OS::strcmp ("test_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list);

      CORBA::Any send_time;
      send_time._tao_set_typecode (CORBA::_tc_ulonglong);
      list->add_value ("send_time", send_time, CORBA::ARG_IN);

      request->arguments (list);

      CORBA::NamedValue_ptr nv = list->item (0);

      request->set_result (*(nv->value ()));

      // AMH way of replying
      rh->invoke_reply (list, nv);

      return;
    }

  CORBA::Any bad_operation;
  CORBA::BAD_OPERATION exception;
  bad_operation <<= exception;
  request->set_exception (bad_operation);
}

CORBA::RepositoryId
Roundtrip::_primary_interface (const PortableServer::ObjectId &,
                               PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:Test/Roundtrip:1.0");
}

void
Roundtrip::_dispatch (TAO_ServerRequest &request,
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
      TAO_ORB_Core *orbcore = request.orb()->orb_core ();
      TAO_AMH_BUFFER_ALLOCATOR* amh_allocator =
          orbcore->lane_resources().amh_response_handler_allocator();
      rh->init (request, amh_allocator);
      // Delegate to user.
      this->invoke (dsi_request,
                    rh.in());
    }
  catch (const CORBA::Exception& ex)

    {
      // Only if the client is waiting.
      if (request.response_expected () && !request.sync_with_server ())
        {
          request.tao_send_reply_exception (ex);
        }
    }

  CORBA::release (dsi_request);
}

void
Roundtrip::invoke (CORBA::ServerRequest_ptr request)
{
  ACE_UNUSED_ARG (request);
  throw CORBA::NO_IMPLEMENT();
}
