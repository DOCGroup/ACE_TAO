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

namespace Dynamic
{
  class ExceptionList;
  class ParameterList;
}

class TAO_Service_Context;
class TAO_Operation_Details;

namespace TAO
{
  class Profile_Transport_Resolver;
  class Invocation_Adapter;

  enum Invocation_Status
    {
      TAO_INVOKE_START,
      /// The request must be restarted, a temporary failure has ocurred.
      TAO_INVOKE_RESTART,
      /// invoke() call successful.
      TAO_INVOKE_SUCCESS,
      TAO_INVOKE_FAILURE
    };

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

    CORBA::Object_ptr forwarded_reference (void);

    char *operation_name (void);
    Dynamic::ParameterList *arguments (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    Dynamic::ExceptionList *exceptions (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    TAO_Service_Context &request_service_context (void);
    TAO_Service_Context &reply_service_context (void);

    CORBA::Octet sync_scope (void) const;
    CORBA::Object_ptr steal_forwarded_reference (void);

    bool is_forwarded (void) const;

  protected:

    Invocation_Base (TAO_Stub *stub,
                     TAO_Operation_Details &op);

    void forwarded_reference (CORBA::Object_ptr o);

  protected:

    TAO_Operation_Details &details_;
    CORBA::Object_var forwarded_to_;

  private:

    ACE_UNIMPLEMENTED_FUNC (Invocation_Base & operator= (const Invocation_Base &));


  private:
    TAO_Stub *stub_;
  };
}
#include "ace/post.h"
#endif /*TAO_INVOCATION_BASE_H*/
