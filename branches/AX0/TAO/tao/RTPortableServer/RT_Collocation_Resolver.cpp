// $Id$

#include "tao/RTPortableServer/RT_Collocation_Resolver.h"

ACE_RCSID(RTCORBA, RT_Collocation_Resolver, "$Id$")

#include "tao/ORB_Core.h"
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/PortableServer/POA.h"
#include "tao/RTCORBA/Thread_Pool.h"

#if !defined (__ACE_INLINE__)
# include "tao/RTPortableServer/RT_Collocation_Resolver.i"
#endif /* ! __ACE_INLINE__ */

CORBA::Boolean
TAO_RT_Collocation_Resolver::is_collocated (CORBA::Object_ptr object
                                            ACE_ENV_ARG_DECL) const
{
  // Make sure that the servant is in the same ORB that created this
  // object.
  if (!object->_is_collocated ())
    return 0;

  // Get the orb core.
  TAO_ORB_Core *orb_core =
    object->_stubobj ()->servant_orb_var ()->orb_core ();

  // Lookup the target POA.  Note that Object Adapter lock is held
  // until <servant_upcall> dies.
  TAO_Object_Adapter::Servant_Upcall servant_upcall (orb_core);
  TAO_POA *poa =
    servant_upcall.lookup_POA (object->_object_key ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Get the thread pool associated with this POA.
  TAO_Thread_Pool *target_thread_pool =
    (TAO_Thread_Pool *) poa->thread_pool ();

  /// Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources &tss =
    *orb_core->get_tss_resources ();

  // Get the lane for this thread.
  TAO_Thread_Lane *current_thread_lane =
    (TAO_Thread_Lane *) tss.lane_;

  TAO_Thread_Pool *current_thread_pool = 0;

  // If we don't have a lane, we don't have a pool.
  if (current_thread_lane)
    current_thread_pool =
      &current_thread_lane->pool ();

  // If the pools don't match, then the current thread belongs to a
  // different pool than POA.  Therefore, this object is not
  // collocated.
  if (current_thread_pool != target_thread_pool)
    return 0;

  // If the current thread and the POA are in the default thread pool,
  // then the object is collocated.
  if (current_thread_pool == 0)
    return 1;

  // If the current thread and the POA are in a thread pool without
  // lanes, then the object is collocated.
  if (!current_thread_pool->with_lanes ())
    return 1;

  // Grab the cached policies from the POA.
  TAO_POA_Cached_Policies &cached_policies =
    poa->cached_policies ();

  // Grab the priority model used by the POA.  Note that this cannot
  // be NOT_SPECIFIED because NOT_SPECIFIED is not allowed with thread
  // pool with lanes.
  TAO_POA_Cached_Policies::PriorityModel priority_model =
    cached_policies.priority_model ();

  // If the policy is CLIENT_PROPAGATED, then we are collocated
  // because the current thread is of the correct priority :-) and
  // we'll simple use the current thread to run the upcall.
  if (priority_model == TAO_POA_Cached_Policies::CLIENT_PROPAGATED)
    return 1;

  // Locate the target servant.  We are really not interested in the
  // servant itself but in the priority that this servant will run at.
  // Note that the operation name is bogus: it is not used because the
  // IMPLICIT_ACTIVATION policy is not allowed with SERVER_DECLARED
  // policy.
  poa->locate_servant_i ("operation not used",
                         servant_upcall.system_id_,
                         servant_upcall,
                         servant_upcall.current_context_
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Get the priority that the servant will run at.
  CORBA::Short target_priority =
    servant_upcall.priority ();

  // If it matches the current thread's priority, then we are
  // collocated.  Otherwise we are not.
  if (target_priority == current_thread_lane->lane_priority ())
    return 1;
  else
    return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_RT_Collocation_Resolver,
                       ACE_TEXT ("RT_Collocation_Resolver"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Collocation_Resolver),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_RT_Collocation_Resolver)
