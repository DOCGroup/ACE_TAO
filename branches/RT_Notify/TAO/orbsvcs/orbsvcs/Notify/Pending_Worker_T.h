/* -*- C++ -*- */
/**
 *  @file Pending_Worker_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PENDING_WORKER_T_H
#define TAO_NS_PENDING_WORKER_T_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Types.h"
#include "ace/Task.h"
#include "orbsvcs/ESF/ESF_Worker.h"

template <class PEER> class TAO_NS_Dispatch_Observer_T;

/**
 * @class TAO_NS_Dispatch_Pending_Worker_T
 *
 * @brief Ask each PEER to dispatch pending updates.
 *
 */
template <class PEER>
class TAO_Notify_Export TAO_NS_Dispatch_Pending_Worker_T : public TAO_ESF_Worker<PEER>
{
protected:
  ///= TAO_ESF_Worker method
  void work (PEER* peer ACE_ENV_ARG_DECL);
};

/*******************************************************************************************/

/**
 * @class TAO_NS_Pending_Worker_T
 *
 * @brief Periodic Task that obtains the list of pending PEER's from the dispatch_observer and dispatches pending events/updates.
 *
 */
template <class PEER>
class TAO_Notify_Export TAO_NS_Pending_Worker_T : public ACE_Task<ACE_SYNCH>
{
public:
  /// Constuctor
  TAO_NS_Pending_Worker_T (void);

  /// Destructor
  ~TAO_NS_Pending_Worker_T ();

  /// Init
  void init (TAO_NS_Dispatch_Observer_T<PEER>* dispatch_observer, ACE_Time_Value& update_period ACE_ENV_ARG_DECL);

  /// Shutdown
  void shutdown (void);

  /// Suspend
  void worker_suspend (void);

  /// Resume
  void worker_resume (void);

protected:
  /// task svc
  virtual int svc (void);

  /// The observer that logs PEERs that need updates.
  TAO_NS_Dispatch_Observer_T<PEER>* dispatch_observer_;

  /// The period at which updates are send.
  ACE_Time_Value update_period_;

  /// Flag that we should shutdown.
  CORBA::Boolean done_;

  /// Flag that we are suspended.
  CORBA::Boolean suspended_;
};

#if defined (__ACE_INLINE__)
#include "Pending_Worker_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Pending_Worker_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Pending_Worker_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_NS_PENDING_WORKER_T_H */
