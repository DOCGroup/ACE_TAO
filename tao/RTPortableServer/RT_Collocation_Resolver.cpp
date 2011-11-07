// $Id$

#include "tao/RTPortableServer/RT_Collocation_Resolver.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "tao/Profile.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Boolean
TAO_RT_Collocation_Resolver::is_collocated (CORBA::Object_ptr object) const
{
  // Make sure that the servant is in the same ORB that created this
  // object.
  if (!object->_is_collocated ())
    return false;

  // Get the orb core.
  TAO_ORB_Core *orb_core = object->_stubobj ()->servant_orb_var ()->orb_core ();

  // Lookup the target POA.  Note that Object Adapter lock is held
  // until <servant_upcall> dies.
  TAO::Portable_Server::Servant_Upcall servant_upcall (orb_core);
  TAO_Root_POA *poa =
    servant_upcall.lookup_POA (object->_stubobj ()->object_key ());

  // Get the thread pool associated with this POA.
  TAO_Thread_Pool *target_thread_pool =
    static_cast <TAO_Thread_Pool *> (poa->thread_pool ());

  // If the target POA does not have a dedicated thread pool, then all
  // calls to it are collocated.
  if (target_thread_pool == 0)
    return true;

  /// Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources &tss =
    *orb_core->get_tss_resources ();

  // Get the lane for this thread.
  TAO_Thread_Lane *current_thread_lane =
    static_cast <TAO_Thread_Lane *> (tss.lane_);

  TAO_Thread_Pool *current_thread_pool = 0;

  // If we don't have a lane, we don't have a pool.
  if (current_thread_lane)
    current_thread_pool =
      &current_thread_lane->pool ();

  // If the pools don't match, then the current thread belongs to a
  // different pool than POA.  Therefore, this object is not
  // collocated.
  if (current_thread_pool != target_thread_pool)
    return false;

  // If the current thread and the POA are in the default thread pool,
  // then the object is collocated.
  if (current_thread_pool == 0)
    return true;

  // If the current thread and the POA are in a thread pool without
  // lanes, then the object is collocated.
  if (!current_thread_pool->with_lanes ())
    return true;

  // Grab the priority model used by the POA.  Note that this cannot
  // be NOT_SPECIFIED because NOT_SPECIFIED is not allowed with thread
  // pool with lanes.
  TAO::Portable_Server::Cached_Policies::PriorityModel priority_model =
    poa->priority_model ();

  // If the policy is CLIENT_PROPAGATED, then we are collocated
  // because the current thread is of the correct priority :-) and
  // we'll simple use the current thread to run the upcall.
  if (priority_model == TAO::Portable_Server::Cached_Policies::CLIENT_PROPAGATED)
    return true;

  // Find the target servant priority. We are really not interested in the
  // servant itself but in the priority that this servant will run at.
  CORBA::Short target_priority;

  if (-1 == poa->find_servant_priority (servant_upcall.system_id_,
                                        target_priority))
  {
    return false;
  }

  // If it matches the current thread's priority, then we are
  // collocated.  Otherwise we are not.
  if (target_priority == current_thread_lane->lane_priority ())
    return true;
  else
    return false;
}


ACE_STATIC_SVC_DEFINE (TAO_RT_Collocation_Resolver,
                       ACE_TEXT ("RT_Collocation_Resolver"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Collocation_Resolver),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTPortableServer, TAO_RT_Collocation_Resolver)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
