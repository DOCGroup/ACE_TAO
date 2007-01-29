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

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL


TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_Operation_Details;
class TAO_Stub;
class TAO_ORB_Core;
class TAO_DII_Deferred_Reply_Dispatcher;
namespace  CORBA
{
  class Object;
  class SystemException;
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


    virtual ~DII_Invocation_Adapter (void) {}

  protected:

    virtual Invocation_Status invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object_var &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time
       );

  private:

    CORBA::ExceptionList *exception_list_;

    CORBA::Request *request_;
  private:

    /// Dont allow default initializations
    DII_Invocation_Adapter (void);

    DII_Invocation_Adapter (const DII_Invocation_Adapter &);
    DII_Invocation_Adapter & operator= (const DII_Invocation_Adapter &);
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
        const char *operation,
        size_t op_len,
        Collocation_Proxy_Broker *b,
        TAO_ORB_Core *oc,
        CORBA::Request *req,
        TAO::Invocation_Mode mode = TAO_DII_DEFERRED_INVOCATION);

    /// Invoke the target
    virtual void invoke (TAO::Exception_Data *ex,
                         unsigned long ex_count
                        );


  protected:
    virtual Invocation_Status invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object_var &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time
       );

  private:
    CORBA::Request *request_;

    /// Reply dispatcher for the current Invocation.
    TAO_DII_Deferred_Reply_Dispatcher *rd_;

    /// Cache the orb_core
    TAO_ORB_Core * const orb_core_;
  };
} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_INVOCATION_ADAPTER_H*/
