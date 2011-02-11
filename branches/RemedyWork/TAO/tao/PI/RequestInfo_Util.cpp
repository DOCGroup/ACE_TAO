// -*- C++ -*-
// $Id$

#include "tao/PI/RequestInfo_Util.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/ORB_Constants.h"
#include "tao/StringSeqC.h"
#include "tao/SystemException.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Dynamic::ParameterList *
TAO_RequestInfo_Util::make_parameter_list (void)
{
  Dynamic::ParameterList *parameter_list = 0;
  ACE_NEW_THROW_EX (parameter_list,
                    Dynamic::ParameterList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return parameter_list;
}

Dynamic::ExceptionList *
TAO_RequestInfo_Util::make_exception_list (void)
{
  Dynamic::ExceptionList *exception_list = 0;
  ACE_NEW_THROW_EX (exception_list,
                    Dynamic::ExceptionList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return exception_list;
}

Dynamic::ContextList *
TAO_RequestInfo_Util::make_context_list (void)
{
  Dynamic::ContextList *context_list = 0;
  ACE_NEW_THROW_EX (context_list,
                    Dynamic::ContextList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return context_list;
}

Dynamic::RequestContext *
TAO_RequestInfo_Util::make_request_context (void)
{
  Dynamic::RequestContext *request_context = 0;
  ACE_NEW_THROW_EX (request_context,
                    Dynamic::RequestContext,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // No need to do an ACE_CHECK_RETURN.  The caller should do that.

  return request_context;
}

CORBA::Any *
TAO_RequestInfo_Util::make_any (CORBA::Boolean tk_void_any)
{
  CORBA::Any *any = 0;
  ACE_NEW_THROW_EX (any,
                    CORBA::Any,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  if (tk_void_any)
    {
      any->_tao_set_typecode (CORBA::_tc_void);
    }

  return any;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
