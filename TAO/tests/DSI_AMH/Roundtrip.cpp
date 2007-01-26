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
                   TAO_AMH_DSI_Response_Handler_ptr rh
                   ACE_ENV_ARG_DECL)
{
  if (ACE_OS::strcmp ("shutdown", request->operation ()) == 0)
    {
      this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  else if (ACE_OS::strcmp ("_is_a", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Any type_id;
      type_id._tao_set_typecode (CORBA::_tc_string);
      list->add_value ("type_id", type_id, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::NamedValue_ptr nv = list->item (0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      const char *arg;
      *(nv->value ()) >>= arg;

      CORBA::Boolean type_matches = 0;
      if (ACE_OS::strcmp (arg, "IDL:Test/Roundtrip:1.0") == 0
          || ACE_OS::strcmp (arg, "IDL:omg.org/CORBA/Object:1.0") == 0
          || ACE_OS::strcmp (arg, "") == 0)
        type_matches = 1;

      CORBA::Any result;
      result <<= CORBA::Any::from_boolean (type_matches);

      request->set_result (result ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // AMH way of replying
      rh->invoke_reply (list, nv);
      return;
    }

  else if (ACE_OS::strcmp ("test_method", request->operation ()) == 0)
    {
      CORBA::NVList_ptr list;
      this->orb_->create_list (0, list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Any send_time;
      send_time._tao_set_typecode (CORBA::_tc_ulonglong);
      list->add_value ("send_time", send_time, CORBA::ARG_IN
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->arguments (list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::NamedValue_ptr nv = list->item (0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->set_result (*(nv->value ()) ACE_ENV_ARG_PARAMETER);

      // AMH way of replying
      rh->invoke_reply (list, nv);

      return;
    }

  CORBA::Any bad_operation;
  CORBA::BAD_OPERATION exception;
  bad_operation <<= exception;
  request->set_exception (bad_operation ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::RepositoryId
Roundtrip::_primary_interface (const PortableServer::ObjectId &,
                               PortableServer::POA_ptr
                               ACE_ENV_ARG_DECL_NOT_USED)
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

  ACE_TRY
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
                    rh.in()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::Exception, ex)

    {
      // Only if the client is waiting.
      if (request.response_expected () && !request.sync_with_server ())
        {
          request.tao_send_reply_exception (ex);
        }
    }
  ACE_ENDTRY;

  CORBA::release (dsi_request);
}

void
Roundtrip::invoke (CORBA::ServerRequest_ptr request
                      ACE_ENV_ARG_NOT_USED)
  throw (CORBA::SystemException)
{
  ACE_UNUSED_ARG (request);
  throw CORBA::NO_IMPLEMENT();
}
