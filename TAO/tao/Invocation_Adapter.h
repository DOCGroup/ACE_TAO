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
#ifndef TAO_INVOCATION_ADAPTER_H
#define TAO_INVOCATION_ADAPTER_H
#include /**/ "ace/pre.h"

#include "ace/Global_Macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @NOTE: Do not include any headers unncessarily here.
#include "ace/CORBA_macros.h"

#include "tao/TAO_Export.h"
#include "tao/Invocation_Utils.h"

struct TAO_Exception_Data;
class TAO_Operation_Details;
class TAO_Stub;
class ACE_Time_Value;

namespace  CORBA
{
  class Object;
  typedef Object *Object_ptr;

  class Environment;
}

namespace TAO
{
  class Argument;
  struct Exception_Data;
  class Collocation_Proxy_Broker;
  class Profile_Transport_Resolver;

  /**
   * @class Invocation_Adapter
   *
   * @brief Generic interface for the invocation object visible to the
   * IDL compiler.
   *
   * The main objective of this class is to adapt the type and
   * invocation specific information declared in the IDL by the
   * application and convert them as CORBA invocations to the target
   * object. Implementation of this class knows how to make
   * invocations on a collocated or a remote object.
   *
   * This adapter class serves as the base class for various types of
   * invocations like AMI, DII, DSI etc. Adapter classes for AMI, DII,
   * DSI inherit from this class and their local behavioural
   * information before kicking off an invocation.
   *
   * @@ More info..
   * Wafer thin inclusions
   * All stuff created on stack
   * Only handles starts and restarts
   *
   */
  class TAO_Export Invocation_Adapter
  {
  public:
    /// The only constructor used by the IDL compiler, and only way to
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
     * exists. This is useful especially to route the call to the
     * collocated target.
     *
     * @param type The operation type which could be a oneway or two
     * way operation. this information is available in the IDL file.
     *
     * @param mode Invocation mode. This information is also available
     * in the IDL file and in the generated code.
     */
    Invocation_Adapter (CORBA::Object_ptr target,
                        Argument **args,
                        int arg_number,
                        char *operation,
                        int op_len,
                        Collocation_Proxy_Broker *cpb,
                        TAO::Invocation_Type type = TAO_TWOWAY_INVOCATION,
                        TAO::Invocation_Mode mode = TAO_SYNCHRONOUS_INVOCATION);

    /// Invoke the target
    /**
     * The implementation decides whether the target is remote or
     * collocated and takes the right decision.
     *
     * @param ex Array of exception data declared by the application
     * in their IDL.
     *
     * @param ex_count Number of elements in the array.
     */
    virtual void invoke (TAO::Exception_Data *ex,
                         unsigned long ex_count
                         ACE_ENV_ARG_DECL);
  protected:

    /// Make a collocated call.
    /**
     * This method checks whether the target is indeed collocated and
     * if so, creates an object that takes care of making collocated
     * invocations  and calls invoke () on it. If the invoke ()
     * returns with a location forwarded reply, this method takes care
     * of forwarding the request to the new target.
     *
     * @NOTE: At this point of time the object that is created for
     * making collocated invocations is too coarse grained to handle
     * different messaging and invocation policies. This need to
     * change in the future. Please take a look at the documentation
     * in Collocated_Invocation.h for details.
     *
     * @param stub The stub object on which the invocation is made.
     *
     * @param op The operations details of the operation that is being
     * invoked.
     */
    virtual void invoke_collocated (TAO_Stub *stub,
                                    TAO_Operation_Details &op
                                    ACE_ENV_ARG_DECL);

    /// Makes a remote calls to the target.
    /**
     * The stub pointer passed to this call has all the details about
     * the remote object to which the invocation needs to be routed
     * to. The implementation of this method takes care of reinvoking
     * the target if it receives forwarding information or if the
     * first invocation fails for some reason, like a loss of
     * connection during send () etc.
     */
    virtual void invoke_remote (TAO_Stub *,
                                TAO_Operation_Details &op
                                ACE_ENV_ARG_DECL);

    /**
     * @name Helper methods for making different types of invocations.
     *
     * These methods useful for various types of invocations like
     * SII, AMI, DII and DSI. All the subclasses implement these
     * methods to get the right behaviour at their level.
     */
    //@{

    /// Helper method that prepares the necessary stuff for a remote
    /// invocation.

    /* This method does the following necessary activities needed for
     * a remote  invocation.
     *
     * - Extracts the roundtrip timeout policies set in the ORB or
     *   Object or at the thread level
     * - Uses the target information to pick a profile and a transport
     *   object on which the invocation needs to be sent
     *
     * - Checks whether the operation is a oneway or twoway and
     *   delegates the call.
     */
    virtual Invocation_Status invoke_remote_i (
        TAO_Stub *,
        TAO_Operation_Details &op,
        CORBA::Object_ptr &effective_target,
        ACE_Time_Value *&max_wait_time
        ACE_ENV_ARG_DECL);


    virtual Invocation_Status invoke_twoway (
        TAO_Operation_Details &op,
        CORBA::Object_ptr &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time
        ACE_ENV_ARG_DECL);

    virtual Invocation_Status invoke_oneway (
        TAO_Operation_Details &op,
        CORBA::Object_ptr &effective_target,
        Profile_Transport_Resolver &r,
        ACE_Time_Value *&max_wait_time
        ACE_ENV_ARG_DECL);

    //@}
    /// Helper function that extracts the roundtrip timeout policies
    /// set in the ORB.
    bool get_timeout (TAO_Stub *stub,
                      ACE_Time_Value &val);

    /// Helper method that extracts TAO_Stub from the target object.
    TAO_Stub *get_stub (ACE_ENV_SINGLE_ARG_DECL) const;

    /// Helper method that takes care of setting the profiles within
    /// the stub object if the target gets forwarded
    void object_forwarded (CORBA::Object *&effective_target,
                           TAO_Stub *stub
                           ACE_ENV_ARG_DECL);
  protected:

    /// The target object on which this invocation is carried out.
    CORBA::Object_ptr target_;

    /// Array of arguments for this operation
    Argument **args_;

    /// Number of arguments for this operation.
    /**
     * This includes the return values too
     */
    int number_args_;

    /// Name of the operation.
    const char *operation_;

    /// String length of the operation name.
    int op_len_;

    /// Collocation proxy broker for this operation.
    Collocation_Proxy_Broker *cpb_;

    /// The invocation type and mode.
    Invocation_Type type_;
    Invocation_Mode mode_;

  private:
    /// Dont allow default initializations
    ACE_UNIMPLEMENTED_FUNC (Invocation_Adapter (void));

    ACE_UNIMPLEMENTED_FUNC (Invocation_Adapter & operator= (const Invocation_Adapter &));
  };
} // End namespace TAO


#if defined (__ACE_INLINE__)
# include "tao/Invocation_Adapter.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_INVOCATION_ADAPTER_H*/
