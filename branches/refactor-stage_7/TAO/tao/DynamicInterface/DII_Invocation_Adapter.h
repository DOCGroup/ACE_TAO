// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation_Adapter.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_DII_INVOCATION_ADAPTER_H
#define TAO_DII_INVOCATION_ADAPTER_H
#include "ace/pre.h"

#include "tao/Invocation_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "dynamicinterface_export.h"

struct TAO_Exception_Data;
class TAO_Operation_Details;
class TAO_Stub;
class ACE_Time_Value;
class TAO_ORB_Core;
class TAO_DII_Deferred_Reply_Dispatcher;
namespace  CORBA
{
  class Object;
  class Environment;
  class SystemException;
  class ExceptionList;
  class Request;
}

namespace TAO
{
  class Argument;
  class Collocation_Proxy_Broker;

  /**
   * @class DII_Invocation_Adapter
   *
   * @brief Generic interface for the DII invocation object visible
   *
   *
   */
  class TAO_DynamicInterface_Export DII_Invocation_Adapter :
    public Invocation_Adapter
  {
  public:
    /// The only constructor used by the IDL compiler and onlly way to
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
     * @param ope_len Number of charecters in the operation name. This
     * is an optimization which helps us to avoid calling strlen ()
     * while creating a message format.
     *
     * @param cpb The collocation proxy broker for the target if one
     * exists.
     *
     * @param type The operation type which could be a oneway or two
     * way operation. this information is availbe in the IDL file.
     *
     * @param mode Invocation mode. This information is also available
     * in the IDL file and in the generated code.
     */
    DII_Invocation_Adapter (CORBA::Object *target,
                            Argument **args,
                            int arg_number,
                            char *operation,
                            int op_len,
                            CORBA::ExceptionList *exception,
                            CORBA::Request *r,
                            Invocation_Mode m = TAO_DII_INVOCATION);


    virtual ~DII_Invocation_Adapter (void) {}

  protected:

    virtual Invocation_Status invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object *&effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time
        ACE_ENV_ARG_DECL);

  private:

    CORBA::ExceptionList *exception_list_;

    CORBA::Request *request_;
  private:

    /// Dont allow default initializations
    ACE_UNIMPLEMENTED_FUNC (DII_Invocation_Adapter (void));

    ACE_UNIMPLEMENTED_FUNC (
        DII_Invocation_Adapter & operator= (const DII_Invocation_Adapter &));
  };

  /**
   *
   */
  class TAO_DynamicInterface_Export DII_Deferred_Invocation_Adapter
    : protected Invocation_Adapter
  {
  public:
    DII_Deferred_Invocation_Adapter (
        CORBA::Object *target,
        Argument **args,
        int arg_number,
        char *operation,
        int op_len,
        Collocation_Proxy_Broker *b,
        TAO_ORB_Core *oc,
        CORBA::Request *req,
        TAO::Invocation_Mode mode = TAO_DII_DEFERRED_INVOCATION);

    /// Invoke the target
    virtual void invoke (TAO_Exception_Data *ex,
                         unsigned long ex_count
                         ACE_ENV_ARG_DECL);


  protected:
    virtual Invocation_Status invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object *&effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time
        ACE_ENV_ARG_DECL);

  private:
    CORBA::Request *request_;

    /// Reply dispatcher for the current Invocation.
    TAO_DII_Deferred_Reply_Dispatcher *rd_;

    /// Cache the orb_core
    TAO_ORB_Core *orb_core_;
  };
} // End namespace TAO


#include "ace/post.h"
#endif /*TAO_INVOCATION_ADAPTER_H*/
