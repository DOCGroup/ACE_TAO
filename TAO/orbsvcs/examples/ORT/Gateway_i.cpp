//$Id$

#include "Gateway_i.h"

#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/ExceptionA.h"

#include "tao/IFR_Client/IFR_BasicC.h"

#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/Unknown_User_Exception.h"

#include "tao/ORB.h"
#include "tao/LocalObject.h"

Gateway_i::
Gateway_i (CORBA::ORB_ptr orb,
           PortableServer::Current_ptr poa_current)
  : orb_ (orb),
    poa_current_ (poa_current)
{
  /// Constructor
}

void
Gateway_i::invoke (CORBA::ServerRequest_ptr request)
{
  PortableServer::ObjectId_var target_id =
    this->poa_current_->get_object_id ();

  CORBA::String_var stringified_object_id =
    PortableServer::ObjectId_to_string (target_id.in ());

  CORBA::Object_var target_object =
    this->orb_->string_to_object (stringified_object_id.in ());

  // Use the IfR interfaces to query the NVList for this object...
  CORBA::InterfaceDef_var target_interface =
    target_object->_get_interface ();

  if (CORBA::is_nil (target_interface.in ()))
    {
      ///
    }

  // This is the target operation...
  CORBA::String_var operation_name =
    request->operation ();

  CORBA::Contained_var contained_operation =
    target_interface->lookup (operation_name.in ());

  CORBA::OperationDef_var operation =
    CORBA::OperationDef::_narrow (contained_operation.in ());

  // Save the result typecode...
  CORBA::TypeCode_var result_typecode =
    operation.in ()->result ();

  CORBA::ParDescriptionSeq_var parameters =
    operation.in ()->params ();

  // Build the NVList based on the info from the IfR
  CORBA::NVList_ptr arguments;
  this->orb_->create_list (parameters->length (),
                           arguments);

  CORBA::Flags flags = 0;

  CORBA::ULong length = parameters->length ();

  CORBA::ULong i = 0;

  for (i = 0; i < length; ++i)
    {
      switch (parameters[i].mode)
        {
        case CORBA::PARAM_IN:
          flags = CORBA::ARG_IN;
          break;
        case CORBA::PARAM_OUT:
          flags = CORBA::ARG_OUT;
          break;
        case CORBA::PARAM_INOUT:
          flags = CORBA::ARG_INOUT;
          break;
        }
    }

  for (i = 0; i != length; ++i)
    {
      CORBA::Any any;
      any._tao_set_typecode (parameters[i].type.in ());

      arguments->add_value (parameters[i].name,
                            any,
                            flags);
    }

  // Extract the values of the arguments from the DSI ServerRequest
  request->arguments (arguments);

  // Use the NVList (with values) to create a DII Request...
  CORBA::Request_var dii_request;

  CORBA::NamedValue *named_value = 0;

  this->orb_->create_named_value (named_value);

  CORBA::ContextList *context_list = 0;
  CORBA::ExceptionList *exceptions = 0;

  target_object->_create_request (CORBA::Context::_nil (),
                                  operation_name.in (),
                                  arguments,
                                  named_value, /* Result */
                                  exceptions,
                                  context_list, /* Context List */
                                  dii_request.inout (),
                                  CORBA::Flags (0));

  // Set the return type...
  dii_request->set_return_type (result_typecode.in ());

  try
    {
      // Make the DII request
      dii_request->invoke ();

      // At this point the NVList contains all the out and inout
      // arguments, but we need to extract the return value...
    }
  catch (CORBA::UnknownUserException& user_ex)
    {
      // Pass the exception back to the server request...
      request->set_exception (user_ex.exception ());
      return;
    }
  catch (const CORBA::SystemException& sys_ex)
    {
      CORBA::Any any;
      any <<= sys_ex;
      // Pass the exception back to the server request...
      request->set_exception (any);
      return;
    }
  catch (const CORBA::Exception&)
    {
    }

  request->set_result (dii_request->return_value ());
  // Using the same NVList for both the DSI Server Request and the DII
  // Request takes care of the out and inout arguments (whew!)
}

CORBA::RepositoryId
Gateway_i::_primary_interface (const PortableServer::ObjectId &,
                               PortableServer::POA_ptr)
{
  return 0;
}
