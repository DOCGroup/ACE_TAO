// $Id$
#include "tao/DynamicInterface/DII_Arguments_Converter_Impl.h"
#include "tao/DynamicInterface/DII_Arguments.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/operation_details.h"
#include "tao/SystemException.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_DII_Arguments_Converter_Impl::convert_request (
    TAO_ServerRequest & server_request,
    TAO::Argument * const args[],
    size_t nargs)
{
  // The DII requests on client side always have two arguments
  // - one is the return argument and the other is NVList_Argument.
  // Since the argument list in the client side is used by server side
  // in collocation case and the server expects the list of arguments
  // and not the NVList_Argument, we need expand the NVList_Argument
  // to be list of Arguments.

  // Before expanding NVList_Argument logic was added, the
  // $TAO_ROOT/tests/DII_Collocated_Tests/run_test.pl should fail.
  // The servant will get incorrect "IN" parameter from the oneway
  // operation with single "IN" parameter and get access violation on
  // get_in_arg () from the oneway operation with multiple "IN"
  // parameters.
  CORBA::NVList_ptr lst
    = static_cast<TAO::NVList_Argument *> (
        server_request.operation_details ()->args()[1])->arg ();

  CORBA::ULong const sz = lst->count ();

  if (sz != nargs - 1)
    {
      throw ::CORBA::BAD_PARAM ();
    }

  // To avoid the use of extraction operators on CORBA::Any, we will
  // marshal the arguments in the NVList into an output cdr and then
  // demarshal them into the TAO::Argument array.
  TAO_OutputCDR output;
  for (CORBA::ULong i = 0; i < sz; ++i)
    {
      CORBA::NamedValue_ptr theitem = lst->item (i);

      if (!(theitem->value ()->impl ()->marshal_value (output)))
        {
          throw ::CORBA::BAD_PARAM ();
        }
    }

  TAO_InputCDR input (output);
  for (CORBA::ULong j = 0; j < sz; ++j)
    {
      if (!(args[j + 1]->demarshal (input)))
        {
          throw ::CORBA::BAD_PARAM ();
        }
    }

  TAO_Operation_Details* details
    = const_cast <TAO_Operation_Details*> (server_request.operation_details ());

  // The NVList_Argument in operation details is converted to skel
  // args, the invocation of the dii collocation requests should use
  // the skel args and not use the stub args.
  details->use_stub_args (false);
}

void
TAO_DII_Arguments_Converter_Impl::dsi_convert_request (
    TAO_ServerRequest & server_request,
    TAO_OutputCDR & output)
{
  // The DII requests on client side always have two arguments
  // - one is the return argument and the other is NVList_Argument.
  CORBA::NVList_ptr lst
    = static_cast<TAO::NVList_Argument *> (
        server_request.operation_details ()->args()[1])->arg ();

  // Only marshal the in(out) arguments since we use this to demarshal
  // these values in the serverside argument list.
  lst->_tao_encode (output, CORBA::ARG_IN | CORBA::ARG_INOUT);
}

void
TAO_DII_Arguments_Converter_Impl::convert_reply (
    TAO_ServerRequest & server_request,
    TAO::Argument * const args[],
    size_t nargs)
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

void
TAO_DII_Arguments_Converter_Impl::dsi_convert_reply (
    TAO_ServerRequest & server_request,
    TAO_InputCDR & input)
{
  TAO::NamedValue_Argument * _ret_val
    = static_cast<TAO::NamedValue_Argument *> (
        server_request.operation_details ()->args()[0]);

  _ret_val->demarshal (input);

  CORBA::NVList_ptr lst
    = static_cast<TAO::NVList_Argument *> (
        server_request.operation_details ()->args()[1])->arg ();

  lst->_tao_decode (input,
                    CORBA::ARG_INOUT | CORBA::ARG_OUT);
}

void
TAO_DII_Arguments_Converter_Impl::handle_corba_exception (
  TAO_ServerRequest & /*server_request*/,
  CORBA::Exception *exception)
{
  exception->_raise ();
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_DII_Arguments_Converter_Impl::Initializer (void)
{
  return ACE_Service_Config::process_directive (
    ace_svc_desc_TAO_DII_Arguments_Converter_Impl);
}


TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (
  TAO_DII_Arguments_Converter_Impl,
  ACE_TEXT ("DII_Arguments_Converter"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (TAO_DII_Arguments_Converter_Impl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0
  )

ACE_FACTORY_DEFINE (TAO_DynamicInterface, TAO_DII_Arguments_Converter_Impl)

