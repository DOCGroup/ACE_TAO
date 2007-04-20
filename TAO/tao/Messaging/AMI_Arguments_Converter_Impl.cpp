// $Id$
#include "tao/Messaging/AMI_Arguments_Converter_Impl.h"
#include "tao/operation_details.h"
#include "tao/SystemException.h"

ACE_RCSID (Messaging,
           AMI_Arguments_Converter_Impl,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_AMI_Arguments_Converter_Impl::convert_request (
    TAO_ServerRequest & server_request,
    TAO::Argument * const args[],
    size_t nargs)
{
  // The AMI requests on client side just has the in and inout argumenst,
  // Since the argument list in the client side is used by server side
  // in collocation case and the server expects the full list of arguments
  // and not just the inout arguments we need to expand the client arguments
  // to be list of Arguments.

  CORBA::ULong const nrarg = server_request.operation_details ()->args_num ();

  TAO_OutputCDR output;
  for (CORBA::ULong i = 1; i < nrarg; ++i)
    {
    if (!(server_request.operation_details ()->args()[i])->marshal (output))
        {
          throw ::CORBA::BAD_PARAM ();
        }
    }

  TAO_InputCDR input (output);
  for (CORBA::ULong j = 1; j < nargs; ++j)
    {
      if (!(args[j]->demarshal (input)))
        {
          throw ::CORBA::BAD_PARAM ();
        }
    }

  TAO_Operation_Details* details
    = const_cast <TAO_Operation_Details*> (server_request.operation_details ());

  details->use_stub_args (false);
}

void
TAO_AMI_Arguments_Converter_Impl::convert_reply (
    TAO_ServerRequest & server_request,
    TAO::Argument * const args[],
    size_t nargs)
{
  TAO_OutputCDR output;
  for (CORBA::ULong j = 1; j < nargs; ++j)
    {
      if (!(args[j]->marshal (output)))
        {
          throw ::CORBA::BAD_PARAM ();
        }
    }
  CORBA::ULong const nrarg = server_request.operation_details ()->args_num ();
  TAO_InputCDR input (output);
  for (CORBA::ULong i = 1; i < nrarg; ++i)
    {
    if (!(server_request.operation_details ()->args()[i])->demarshal (input))
        {
          throw ::CORBA::BAD_PARAM ();
        }
    }
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_AMI_Arguments_Converter_Impl::Initializer (void)
{
  return ACE_Service_Config::process_directive (
    ace_svc_desc_TAO_AMI_Arguments_Converter_Impl);
}


TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (
  TAO_AMI_Arguments_Converter_Impl,
  ACE_TEXT ("AMI_Arguments_Converter"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (TAO_AMI_Arguments_Converter_Impl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0
  )

ACE_FACTORY_DEFINE (TAO_Messaging, TAO_AMI_Arguments_Converter_Impl)

