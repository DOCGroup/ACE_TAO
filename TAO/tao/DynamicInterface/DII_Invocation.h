// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    DynamicInterface
//
// = DESCRIPTION
//    The DII invocation classes.
//
// = AUTHOR
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_DII_INVOCATION_H
#define TAO_DII_INVOCATION_H
#include /**/ "ace/pre.h"

#include "dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Synch_Invocation.h"
#include "tao/Messaging/Asynch_Invocation.h"

class TAO_DII_Deferred_Reply_Dispatcher;

namespace TAO
{
  /**
   */
  class TAO_DynamicInterface_Export DII_Invocation:
    public Synch_Twoway_Invocation
  {
  public:
    friend class DII_Invocation_Adapter;

    DII_Invocation (CORBA::Object_ptr otarget,
                    Profile_Transport_Resolver &resolver,
                    TAO_Operation_Details &detail,
                    CORBA::ExceptionList *excp,
                    CORBA::Request_ptr req,
                    bool response_expected = true);

#if TAO_HAS_INTERCEPTORS ==1
    virtual Dynamic::ParameterList *arguments (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
#endif /*TAO_HAS_INTERCEPTORS == 1*/

    Invocation_Status remote_invocation (
        ACE_Time_Value *max_wait_time
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((CORBA::Exception));

    virtual Invocation_Status handle_user_exception (
        TAO_InputCDR &cdr
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((CORBA::Exception));
  private:

    CORBA::ExceptionList *excp_list_;

    // Back pointer to the DII request that created us.
    CORBA::Request_ptr host_;

  };


  /*
   *
   */
  class TAO_DynamicInterface_Export DII_Deferred_Invocation :
    public Asynch_Remote_Invocation
  {
  public:
    friend class DII_Deferred_Invocation_Adapter;

    DII_Deferred_Invocation (CORBA::Object_ptr otarget,
                             Profile_Transport_Resolver &resolver,
                             TAO_Operation_Details &detail,
                             TAO_DII_Deferred_Reply_Dispatcher *rd,
                             CORBA::Request_ptr req,
                             bool response_expected = true);

#if TAO_HAS_INTERCEPTORS ==1
    virtual Dynamic::ParameterList *arguments (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
#endif /*TAO_HAS_INTERCEPTORS == 1*/

    Invocation_Status remote_invocation (
        ACE_Time_Value *max_wait_time
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((CORBA::Exception));

  private:

    // Back pointer to the DII request that created us.
    CORBA::Request_ptr host_;

  };
}

#if defined (__ACE_INLINE__)
#  include "DII_Invocation.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ASYNCH_INVOCATION_H */
