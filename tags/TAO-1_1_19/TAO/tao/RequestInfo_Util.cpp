// -*- C++ -*-
//
// $Id$

#include "RequestInfo_Util.h"
#include "Any.h"
#include "StringSeqC.h"

ACE_RCSID (tao, RequestInfo_Util, "$Id$")

Dynamic::ParameterList *
TAO_RequestInfo_Util::make_parameter_list (CORBA::Environment &ACE_TRY_ENV)
{
  Dynamic::ParameterList *parameter_list = 0;
  ACE_NEW_THROW_EX (parameter_list,
                    Dynamic::ParameterList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return parameter_list;
}

Dynamic::ExceptionList *
TAO_RequestInfo_Util::make_exception_list (CORBA::Environment &ACE_TRY_ENV)
{
  Dynamic::ExceptionList *exception_list = 0;
  ACE_NEW_THROW_EX (exception_list,
                    Dynamic::ExceptionList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return exception_list;
}

Dynamic::ContextList *
TAO_RequestInfo_Util::make_context_list (CORBA::Environment &ACE_TRY_ENV)
{
  Dynamic::ContextList *context_list = 0;
  ACE_NEW_THROW_EX (context_list,
                    Dynamic::ContextList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return context_list;
}

Dynamic::RequestContext *
TAO_RequestInfo_Util::make_request_context (CORBA::Environment &ACE_TRY_ENV)
{
  Dynamic::RequestContext *request_context = 0;
  ACE_NEW_THROW_EX (request_context,
                    Dynamic::RequestContext,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return request_context;
}

CORBA::Any *
TAO_RequestInfo_Util::make_any (CORBA::Boolean tk_void_any,
                                CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Any *any = 0;

  if (tk_void_any)
    {
      CORBA::TypeCode *tc = 0;
      ACE_NEW_THROW_EX (tc,
                        CORBA::TypeCode (CORBA::tk_void),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      CORBA::TypeCode_var safe_tc = tc;

      ACE_NEW_THROW_EX (any,
                        CORBA::Any (tc),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      // The Any duplicates the TypeCode so there is no problem in
      // allowing the TypeCode_var to decrement the reference count on
      // the tk_void TypeCode.
    }
  else
    {
      ACE_NEW_THROW_EX (any,
                        CORBA::Any,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
    }

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return any;
}
