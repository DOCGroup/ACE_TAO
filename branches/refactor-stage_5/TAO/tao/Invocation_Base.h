// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation_Base.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_INVOCATION_BASE_H
#define TAO_INVOCATION_BASE_H
#include "ace/pre.h"

#include "tao/Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Invocation_Utils.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "tao/ClientRequestInfo_i.h"
#include "tao/ClientRequestInterceptor_Adapter.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

namespace Dynamic
{
  class ExceptionList;
  class ParameterList;
}

class TAO_Service_Context;
class TAO_Operation_Details;
class TAO_Stub;

namespace TAO
{
  class Invocation_Adapter;

  /**
   * @class Invocation_Base
   *
   * @brief
   *
   */
  class TAO_Export Invocation_Base
  {
  public:
    friend class Invocation_Adapter;

    virtual ~Invocation_Base (void);
    TAO_ORB_Core *orb_core (void) const;
    TAO_Stub *stub (void) const;

    CORBA::Object_ptr forwarded_reference (void);

    TAO_Service_Context &request_service_context (void);
    TAO_Service_Context &reply_service_context (void);

    CORBA::Octet sync_scope (void) const;
    CORBA::Object_ptr steal_forwarded_reference (void);

    bool is_forwarded (void) const;

    void reply_received (Invocation_Status s);

  protected:

    Invocation_Base (CORBA::Object_ptr target,
                     TAO_Operation_Details &op,
                     bool response_expected = true);

    void forwarded_reference (CORBA::Object_ptr o);

  protected:

    TAO_Operation_Details &details_;
    CORBA::Object_var forwarded_to_;

  private:

    ACE_UNIMPLEMENTED_FUNC (Invocation_Base & operator= (const Invocation_Base &));


  private:

    CORBA::Object_ptr target_;

#if TAO_HAS_INTERCEPTORS == 1
  public:

    char *operation_name (void);

    Dynamic::ParameterList *arguments (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    Dynamic::ExceptionList *exceptions (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CORBA::Any * result (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:

    Invocation_Status send_request_interception (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    Invocation_Status receive_reply_interception (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    Invocation_Status receive_other_interception (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    PortableInterceptor::ReplyStatus
        handle_any_exception (CORBA::Exception *e
                              ACE_ENV_ARG_DECL);

    PortableInterceptor::ReplyStatus
        handle_all_exception (ACE_ENV_SINGLE_ARG_DECL);


  protected:

    ClientRequestInterceptor_Adapter adapter_;

    TAO_ClientRequestInfo_i req_info_;
#endif /*TAO_HAS_INTERCEPTORS*/
  };
}

#include "ace/post.h"
#endif /*TAO_INVOCATION_BASE_H*/
