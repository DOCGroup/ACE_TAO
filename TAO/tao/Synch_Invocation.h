// -*- C++ -*-

//=============================================================================
/**
 *  @file    Synch_Invocation.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SYNCH_INVOCATION_H
#define TAO_SYNCH_INVOCATION_H
#include /**/ "ace/pre.h"

#include "tao/Remote_Invocation.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"

class TAO_Operation_Details;
class TAO_Synch_Reply_Dispatcher;
class TAO_InputCDR;
class ACE_Time_Value;
class TAO_Bind_Dispatcher_Guard;

namespace CORBA
{
  class SystemException;
}

namespace TAO
{
  class Profile_Transport_Resolver;

  /**
   * @class Synch_Twoway_Invocation
   *
   * @brief All the action for a synchronous twoway invocation happen
   * here.
   *
   * An object of this is type is created by TAO::Invocation_Adapter
   * and invokes a method on this class. The method takes care of
   * creating and sending a request, waiting for a reply and
   * demarshalling the reply for the client.
   *
   */
  class TAO_Export Synch_Twoway_Invocation : public Remote_Invocation
  {
  public:
    /// Constructor used by TAO::Invocation_Adapter
    /**
     * @param otarget, The original target on which this invocation
     * was started. This is there to be passed up to its parent
     * class.
     *
     * @param resolver, The profile and transport holder.
     *
     * @param op, operation details of the invocation on the target
     *
     * @param response_expected, flag to indicate whether the
     * operation encapsulated by @a op returns a response or not.
     */
    Synch_Twoway_Invocation (CORBA::Object_ptr otarget,
                             Profile_Transport_Resolver &resolver,
                             TAO_Operation_Details &detail,
                             bool response_expected = true);

    /// Method used by the adapter to kickstart an invocation to the
    /// remote object.
    /**
     * There is a exception declaration in this method which ensures
     * that the exceptions propogated by the remote objects are
     * converted a CORBA exceptions for the clients. This method does
     * a bunch of things necessary to create and send the
     * invocation. This method is also nerve centre for the
     * interceptor invocation points.
     */
    Invocation_Status remote_twoway (ACE_Time_Value *max_wait_time
                                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::Exception));

  protected:

    /**
     * This method is selectively made virtual, so that inherited
     * classes can overload the  user exception handling type. For
     * example the DII needs a totally different method of
     * user exception exception handling
     */
    virtual Invocation_Status handle_user_exception (TAO_InputCDR &cdr
                                                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::Exception));

    /// Helper method used to handle location forwarded replies.
    Invocation_Status location_forward (TAO_InputCDR &cdr
                                        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Helper method used to handle system exceptions from the remote
    /// objects.
    Invocation_Status handle_system_exception (TAO_InputCDR &cdr
                                               ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// As the name suggests waits for a reply from the remote ORB.
    Invocation_Status wait_for_reply (ACE_Time_Value *max_wait_time,
                                      TAO_Synch_Reply_Dispatcher &rd,
                                      TAO_Bind_Dispatcher_Guard &bd
                                      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    /// Helper method that checks the reply status of the
    /// replies and takes appropriate action.
    Invocation_Status check_reply_status (
        TAO_Synch_Reply_Dispatcher &rd
        ACE_ENV_ARG_DECL);

  };

  /**
   * @class Synch_Oneway_Invocation
   *
   * @brief All the action for a synchronous oneway invocation happen
   * here.
   *
   * This class inherits from twoway invocation for the following
   * reasons
   *
   * - We deal with oneway invocation in more or less the same way as
   *   two but for waiting for the reply
   *
   * - For some SYNC_SCOPE policies namely SYNC_WITH_TARGET and
   *   SYNC_WITH_SERVER the invocation classes have to treat the
   *   invocation as a twoway invocation (more or less)
   *
   */
  class TAO_Export Synch_Oneway_Invocation :
    public Synch_Twoway_Invocation
  {
  public:
    /// Constructor used by TAO::Invocation_Adapter
    /**
     * @param otarget, The original target on which this invocation
     * was started. This is there to be passed up to its parent
     * class.
     *
     * @param resolver, The profile and transport holder.
     *
     * @param op, operation details of the invocation on the target
     *
     * @param response_expected, flag to indicate whether the
     * operation encapsulated by @a op returns a response or not.
     */
    Synch_Oneway_Invocation (CORBA::Object_ptr otarget,
                             Profile_Transport_Resolver &resolver,
                             TAO_Operation_Details &detail);

    /// Method used by the adapter to kickstart an oneway invocation
    /// to the remote object.
    Invocation_Status remote_oneway (ACE_Time_Value *max_wait_time
                                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::Exception));
  };

  /**
   * @class Reply_Guard
   *
   * @brief A guard class used for storing and forwarding the reply
   *  status to the portable interceptors.
   */
  class TAO_Export Reply_Guard
  {
  public:
    Reply_Guard (Invocation_Base *s,
                 Invocation_Status is);

    /// The destructor calls Invocation_Base::reply_received with the
    /// right reply status, which is useful for PI's.
    ~Reply_Guard (void);

    /// Mutator to set the invocation status.
    void set_status (Invocation_Status s);

  private:
    Invocation_Base *invocation_;
    Invocation_Status status_;
  };

}


#if defined (__ACE_INLINE__)
# include "tao/Synch_Invocation.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_SYNCH_INVOCATION_H*/
