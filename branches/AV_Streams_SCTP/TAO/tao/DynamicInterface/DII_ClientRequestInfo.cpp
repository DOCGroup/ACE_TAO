#include "DII_ClientRequestInfo.h"


#if TAO_HAS_INTERCEPTORS == 1

#include "tao/RequestInfo_Util.h"

ACE_RCSID (DynamicInterface,
           DII_ClientRequestInfo,
           "$Id$")


TAO_DII_ClientRequestInfo::TAO_DII_ClientRequestInfo (
  TAO_GIOP_Invocation *_tao_invocation,
  CORBA::Object_ptr _tao_target,
  CORBA::Request_ptr request)
  : TAO_ClientRequestInfo_i (_tao_invocation, _tao_target),
    request_ (request),
    result_ (0)
{
}

Dynamic::ParameterList *
TAO_DII_ClientRequestInfo::arguments (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Generate the argument list on demand.
  Dynamic::ParameterList *parameter_list =
    TAO_RequestInfo_Util::make_parameter_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Dynamic::ParameterList_var safe_parameter_list = parameter_list;

  const CORBA::ULong len = this->request_->arguments ()->count ();
  parameter_list->length (len);

  for (CORBA::ULong i = 0; i < len; ++i)
  {
    (*parameter_list)[len].argument <<=
      *this->request_->arguments ()->item (len)->value ();

    switch(this->request_->arguments ()->item (len)->flags ())
      {
      case CORBA::ARG_IN:
        {
          (*parameter_list)[len].mode = CORBA::PARAM_IN;
          break;
        }
      case CORBA::ARG_INOUT:
        {
          (*parameter_list)[len].mode = CORBA::PARAM_INOUT;
          break;
        }
      case CORBA::ARG_OUT:
        {
          (*parameter_list)[len].mode = CORBA::PARAM_OUT;
          break;
        }
      default:
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
      }
  }

  return safe_parameter_list._retn ();
}

Dynamic::ExceptionList *
TAO_DII_ClientRequestInfo::exceptions (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Generate the exception list on demand.
  Dynamic::ExceptionList *exception_list =
    TAO_RequestInfo_Util::make_exception_list (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Dynamic::ExceptionList_var safe_exception_list = exception_list;

  CORBA::ExceptionList_var ex_list = this->request_->exceptions ();

  const CORBA::ULong count = ex_list->count ();
  exception_list->length (count);

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      (*exception_list)[i] = ex_list->item (i);
    }

  return safe_exception_list._retn ();
}

CORBA::Any *
TAO_DII_ClientRequestInfo::result (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Generate the result on demand.
  const CORBA::Boolean tk_void_any = 0;
  CORBA::Any *result_any =
    TAO_RequestInfo_Util::make_any (tk_void_any
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Any_var safe_result_any = result_any;

  (*result_any) <<= this->result_;
  return safe_result_any._retn ();
}

void
TAO_DII_ClientRequestInfo::result (CORBA::Long result)
{
  // update the result
  this->result_ = result;
}

char *
TAO_DII_ClientRequestInfo::operation (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->request_->operation ());
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
