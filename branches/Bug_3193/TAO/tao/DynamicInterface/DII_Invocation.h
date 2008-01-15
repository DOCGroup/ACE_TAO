// -*- C++ -*-

//=============================================================================
/**
 *  @file    DII_Invocation.h
 *
 *  $Id$
 *
 *  @brief The DII invocation classes.
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu>
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_DII_INVOCATION_H
#define TAO_DII_INVOCATION_H

#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Synch_Invocation.h"
#include "tao/Messaging/Asynch_Invocation.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DII_Deferred_Reply_Dispatcher;
class TAO_DII_Asynch_Reply_Dispatcher;

namespace Dynamic
{
  class ParameterList;
}

namespace TAO
{
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
    virtual Dynamic::ParameterList *arguments (void);
#endif /*TAO_HAS_INTERCEPTORS == 1*/

    Invocation_Status remote_invocation (ACE_Time_Value *max_wait_time);

    virtual Invocation_Status handle_user_exception (TAO_InputCDR &cdr);

  private:
    CORBA::ExceptionList *excp_list_;

    /// Back pointer to the DII request that created us.
    CORBA::Request_ptr host_;

  };

  class TAO_DynamicInterface_Export DII_Deferred_Invocation
    : public Asynch_Remote_Invocation
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
    virtual Dynamic::ParameterList *arguments (void);
#endif /*TAO_HAS_INTERCEPTORS == 1*/

    Invocation_Status remote_invocation (ACE_Time_Value *max_wait_time);

  private:
    /// Back pointer to the DII request that created us.
    CORBA::Request_ptr host_;
  };

  /**
   * @class TAO_GIOP_DII_Asynch_Invocation
   *
   * @brief This class is responsible to send the asynchronous
   * invocation.
   *
   * This class is responsible to send the asynchronous
   * invocation. This class connects (or looks up a connection from the cache)
   * to the remote server, builds the CDR stream for the Request, send
   * the CDR stream and returns.
   *
   */
  class TAO_DynamicInterface_Export TAO_GIOP_DII_Asynch_Invocation
    : public TAO::Asynch_Remote_Invocation
  {
  public:
    TAO_GIOP_DII_Asynch_Invocation (TAO_Stub *data,
                                    TAO_ORB_Core* orb_core,
                                    CORBA::Boolean argument_flag,
                                    const CORBA::Request_ptr req,
                                    CORBA::Object_ptr reply_handler,
                                    int byte_order = TAO_ENCAP_BYTE_ORDER);

    int invoke (void);

  private:
    /// Implementation of the invoke() methods, handles the basic
    /// send/reply code and the system exceptions.
    int invoke_i (void);


    /// Reply dispatcher for the current synchronous Asynch_Invocation.
    TAO_DII_Asynch_Reply_Dispatcher *rd_;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_DII_INVOCATION_H */
