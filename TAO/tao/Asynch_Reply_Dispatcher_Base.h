// -*- C++ -*-

//=============================================================================
/**
 *  @file Asynch_Reply_Dispatcher_Base.h
 *
 *  $Id$
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ASYNCH_REPLY_DISPATCHER_BASE_H
#define TAO_ASYNCH_REPLY_DISPATCHER_BASE_H

#include /**/ "ace/pre.h"

#include "tao/Reply_Dispatcher.h"
#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IOPC.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
class ACE_Lock;
class ACE_Allocator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Pluggable_Reply_Params;
class TAO_ORB_Core;
class TAO_Transport;
/**
 * @class TAO_Asynch_Reply_Dispatcher_Base
 *
 * @brief Base class for TAO_Asynch_Reply_Dispatcher and
 *  TAO_DII_Deferred_Reply_Dispatcher
 */
class TAO_Export TAO_Asynch_Reply_Dispatcher_Base
  : public TAO_Reply_Dispatcher
{
public:
  /// Default constructor.
  TAO_Asynch_Reply_Dispatcher_Base (TAO_ORB_Core *orb_core,
                                    ACE_Allocator *allocator = 0);

  /// Sets the transport for this invocation.
  void transport (TAO_Transport *t);

  /// Install the timeout handler
  virtual long schedule_timer (CORBA::ULong , const ACE_Time_Value &) = 0;


  /// A helper method that can be used by the subclasses
  /**
   * The semantics of this helper method needs careful attention. A
   * call to this method will do the following
   *
   *   - If the reply has already been dispatched, the return value
   *     will be false to signify not to try.
   *
   *   - If the reply has not been dispatched, this method will set
   *     the flag to be true and return a true value to signify that
   *     the caller thread can go ahead and dispatch reply.
   *
   * Why are we clumping everything in one method. Answer is we need
   * atomicity?
   */
  bool try_dispatch_reply (void);

protected:

  /// Destructor.
  virtual ~TAO_Asynch_Reply_Dispatcher_Base (void);

private:
  void operator= (const TAO_Asynch_Reply_Dispatcher_Base &);
  TAO_Asynch_Reply_Dispatcher_Base (const TAO_Asynch_Reply_Dispatcher_Base &);

protected:
  /// The service context list.
  /**
   * Note, that this is not a reference as in
   * the synchronous case. We own the reply_service_info
   * because our TAO_Asynch_Invocation or TAO_DII_Deferred_Invocation
   * will go out of scope before we are done.
   */
  IOP::ServiceContextList reply_service_info_;

  /// The buffer that is used to initialise the data block
  char buf_[ACE_CDR::DEFAULT_BUFSIZE];

  /// Datablock that is created on the stack to initialise the CDR
  /// stream underneath.
  ACE_Data_Block db_;

  /// CDR stream which has the reply information that needs to be
  /// demarshalled by the stubs
  TAO_InputCDR reply_cdr_;

  /// This invocation is using this transport, may change...
  TAO_Transport *transport_;

private:
  /// Lock to protect @c is_reply_dispatched_ flag.
  ACE_Lock *lock_;

  /// Has the reply been dispatched?
  bool is_reply_dispatched_;
};

namespace TAO
{
  /**
   * @class ARDB_Refcount_Functor
   *
   * @brief Functor for refcounting of Asynch_Reply_Dispatcher_Base
   *
   * This is used to safely handle the destruction of
   * Asynch_Reply_Dispatcher_Base objects which are created on the
   * heap. We cannot use auto_ptr <> since it calls delete on the
   * pointer, and calling delete on Asynch_Reply_Dispatcher_Base *
   * will not work. Hence this functor will be used with Auto_Functor
   * class to handle the memory safely.
   *
   * @todo Ideally, this class can be a generic class. But that
   * requires quite a bit of cleanup within TAO to be more useful.
   */
  class TAO_Export ARDB_Refcount_Functor
  {
  public:
    void operator() (TAO_Asynch_Reply_Dispatcher_Base *ardb) throw ();
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Asynch_Reply_Dispatcher_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ASYNCH_REPLY_DISPATCHER_BASE_H */
