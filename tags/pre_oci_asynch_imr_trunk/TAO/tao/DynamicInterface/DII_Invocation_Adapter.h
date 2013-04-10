// -*- C++ -*-

//=============================================================================
/**
 *  @file    DII_Invocation_Adapter.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_DII_INVOCATION_ADAPTER_H
#define TAO_DII_INVOCATION_ADAPTER_H
#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/Invocation_Adapter.h"

#if defined (TAO_HAS_AMI)
#include "tao/Messaging/Messaging.h"
#endif /* TAO_HAS_AMI */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL


TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_Operation_Details;
class TAO_Stub;
class TAO_ORB_Core;
class TAO_DII_Deferred_Reply_Dispatcher;
class TAO_DII_Asynch_Reply_Dispatcher;
namespace  CORBA
{
  class Object;
  class ExceptionList;
  class Request;
}

namespace TAO
{
  class Argument;
  class Collocation_Proxy_Broker;
  struct Exception_Data;

  /**
   * @class DII_Invocation_Adapter
   *
   * @brief Generic interface for the DII invocation object visible
   */
  class TAO_DynamicInterface_Export DII_Invocation_Adapter :
    public Invocation_Adapter
  {
  public:
    /// The only constructor used by the IDL compiler and only way to
    /// create this adapter.
    /**
     *
     * @param target Points to the object on which this invocation is
     * being invoked.
     *
     * @param args Array of pointers to the argument list in the
     * operation declaration. this includes the return, inout and out
     * arguments.
     *
     * @param arg_number Number of arguments in the above array. This
     * is the number of elements in the above array.
     *
     * @param operation The name of the operation being invoked.
     *
     * @param op_len Number of characters in the operation name. This
     * is an optimization which helps us to avoid calling strlen ()
     * while creating a message format.
     *
     * @param mode Invocation mode. This information is also available
     * in the IDL file and in the generated code.
     */
    DII_Invocation_Adapter (CORBA::Object *target,
                            Argument **args,
                            int arg_number,
                            const char *operation,
                            size_t op_len,
                            CORBA::ExceptionList *exception,
                            CORBA::Request *r,
                            Invocation_Mode mode = TAO_DII_INVOCATION);

    virtual ~DII_Invocation_Adapter (void);

    /// Invoke the target
    virtual void invoke (TAO::Exception_Data *ex, unsigned long ex_count);

  protected:

    virtual Invocation_Status invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object_var &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time,
        Invocation_Retry_State *retry_state = 0);

    virtual Invocation_Status invoke_collocated_i (
        TAO_Stub *stub,
        TAO_Operation_Details &details,
        CORBA::Object_var &effective_target,
        Collocation_Strategy strat);

  private:

    CORBA::ExceptionList *exception_list_;

    CORBA::Request *request_;

    TAO::Exception_Data *ex_data_;
  private:

    /// Dont allow default initializations
    DII_Invocation_Adapter (void);

    DII_Invocation_Adapter (const DII_Invocation_Adapter &);
    DII_Invocation_Adapter & operator= (const DII_Invocation_Adapter &);
  };

  /**
   * @class  DII_Deferred_Invocation_Adapter
   *
   * @brief This class is for deferred DII invocation.
   */
  class TAO_DynamicInterface_Export DII_Deferred_Invocation_Adapter
    : protected Invocation_Adapter
  {
  public:
    DII_Deferred_Invocation_Adapter (
        CORBA::Object *target,
        Argument **args,
        int arg_number,
        const char *operation,
        size_t op_len,
        int collocation_opportunity,
        TAO_ORB_Core *oc,
        CORBA::Request *req,
        TAO::Invocation_Mode mode = TAO_DII_DEFERRED_INVOCATION);

    /// Invoke the target
    virtual void invoke (TAO::Exception_Data *ex, unsigned long ex_count);

  protected:
    virtual Invocation_Status invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object_var &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time,
        Invocation_Retry_State *retry_state = 0);

    virtual Invocation_Status invoke_collocated_i (
        TAO_Stub *stub,
        TAO_Operation_Details &details,
        CORBA::Object_var &effective_target,
        Collocation_Strategy strat);

  private:
    CORBA::Request *request_;

    /// Reply dispatcher for the current Invocation.
    TAO_DII_Deferred_Reply_Dispatcher *rd_;

    /// Cache the orb_core
    TAO_ORB_Core * const orb_core_;
  };

#if defined (TAO_HAS_AMI)
  /**
   * @class  DII_Asynch_Invocation_Adapter
   *
   * @brief This class is for asynchronous DII invocation.
   */
  class TAO_DynamicInterface_Export DII_Asynch_Invocation_Adapter
    : protected DII_Invocation_Adapter
  {
  public:
    DII_Asynch_Invocation_Adapter (
       CORBA::Object *target,
       Argument **args,
       int arg_number,
       const char *operation,
       int op_len,
       CORBA::Request *req,
       TAO::Invocation_Mode mode = TAO_DII_ASYNCH_INVOCATION);

    /// Invoke the target
    void invoke_reply_handler (Messaging::ReplyHandler_ptr reply_handler_ptr);

  protected:
    virtual Invocation_Status invoke_twoway (
                TAO_Operation_Details &op,
                CORBA::Object_var &effective_target,
                Profile_Transport_Resolver &r,
                ACE_Time_Value *&max_wait_time,
                Invocation_Retry_State *retry_state = 0);
  };
#endif /* TAO_HAS_AMI */

  /**
   * @class  DII_Oneway_Invocation_Adapter
   *
   * @brief This class is for oneway DII invocation.
   */
  class TAO_DynamicInterface_Export DII_Oneway_Invocation_Adapter
    : public Invocation_Adapter
  {
  public:
    DII_Oneway_Invocation_Adapter (
       CORBA::Object *target,
       Argument **args,
       int arg_number,
       const char *operation,
       int op_len,
       TAO::Invocation_Mode mode = TAO_SYNCHRONOUS_INVOCATION);

  protected:
    virtual Invocation_Status invoke_collocated_i (
        TAO_Stub *stub,
        TAO_Operation_Details &details,
        CORBA::Object_var &effective_target,
        Collocation_Strategy strat);
  };
} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_INVOCATION_ADAPTER_H*/
