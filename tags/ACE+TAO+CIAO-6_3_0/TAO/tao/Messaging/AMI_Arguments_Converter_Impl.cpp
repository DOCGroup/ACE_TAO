// -*- C++ -*-
// $Id$

#include "tao/Messaging/AMI_Arguments_Converter_Impl.h"
#include "tao/operation_details.h"
#include "tao/SystemException.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/CDR.h"

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

  TAO_OutputCDR output;
  this->dsi_convert_request (server_request, output);

  TAO_InputCDR input (output);
  errno = 0;
  for (CORBA::ULong j = 1; j < nargs; ++j)
    {
      if (!(args[j]->demarshal (input)))
        {
          TAO_InputCDR::throw_skel_exception (errno);
        }
    }

  TAO_Operation_Details* details
    = const_cast <TAO_Operation_Details*> (server_request.operation_details ());

  details->use_stub_args (false);
}

void
TAO_AMI_Arguments_Converter_Impl::dsi_convert_request (
    TAO_ServerRequest & server_request,
    TAO_OutputCDR & output)
{
  // The AMI requests on client side just has the in and inout arguments
  CORBA::ULong const nrarg = server_request.operation_details ()->args_num ();

  for (CORBA::ULong i = 1; i < nrarg; ++i)
    {
    if (!(server_request.operation_details ()->args()[i])->marshal (output))
        {
          throw ::CORBA::BAD_PARAM ();
        }
    }
}

void
TAO_AMI_Arguments_Converter_Impl::convert_reply (
    TAO_ServerRequest & server_request,
    TAO::Argument * const args[],
    size_t nargs)
{
  if (server_request.operation_details ()->reply_dispatcher ())
    {
      TAO_OutputCDR output;
      errno = 0;
      for (CORBA::ULong j = 0; j < nargs; ++j)
        {
          if (!(args[j]->marshal (output)))
            {
              TAO_OutputCDR::throw_skel_exception (errno);
            }
        }
      TAO_InputCDR input (output);
      this->dsi_convert_reply (server_request, input);
    }
}

void
TAO_AMI_Arguments_Converter_Impl::dsi_convert_reply (
    TAO_ServerRequest & server_request,
    TAO_InputCDR & input)
{
  if (server_request.operation_details ()->reply_dispatcher ())
    {
      TAO_Pluggable_Reply_Params params (0);
      params.reply_status (GIOP::NO_EXCEPTION);
      params.input_cdr_ = &input;
      server_request.operation_details ()->
        reply_dispatcher ()->dispatch_reply (params);
    }
}

void
TAO_AMI_Arguments_Converter_Impl::handle_corba_exception (
  TAO_ServerRequest & server_request,
  CORBA::Exception *exception)
{
  TAO_OutputCDR output;
  TAO_Pluggable_Reply_Params params (0);
  exception->_tao_encode (output);
  if (CORBA::SystemException::_downcast (exception) != 0)
    {
      params.reply_status (GIOP::SYSTEM_EXCEPTION);
    }
  else
    {
      params.reply_status (GIOP::USER_EXCEPTION);
    }
  TAO_InputCDR input (output);
  params.input_cdr_ = &input;
  server_request.operation_details ()->
    reply_dispatcher ()->dispatch_reply (params);
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_AMI_Arguments_Converter_Impl::Initializer (void)
{
  return ACE_Service_Config::process_directive (
    ace_svc_desc_TAO_AMI_Arguments_Converter_Impl);
}



ACE_STATIC_SVC_DEFINE (
  TAO_AMI_Arguments_Converter_Impl,
  ACE_TEXT ("AMI_Arguments_Converter"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (TAO_AMI_Arguments_Converter_Impl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0
  )

ACE_FACTORY_DEFINE (TAO_Messaging, TAO_AMI_Arguments_Converter_Impl)

TAO_END_VERSIONED_NAMESPACE_DECL
