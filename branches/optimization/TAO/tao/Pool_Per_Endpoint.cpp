// $Id$

#include "tao/Pool_Per_Endpoint.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Pluggable.h"
#include "tao/Priority_Mapping_Manager.h"
#include "tao/debug.h"
#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
# include "tao/Pool_Per_Endpoint.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Pool_Per_Endpoint, "$Id$")

TAO_Pool_Per_Endpoint::TAO_Pool_Per_Endpoint (CORBA::ORB_ptr orb,
                                              int policy,
                                              int poolsize,
                                              int flags)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     policy_ (policy),
     poolsize_ (poolsize),
     flags_ (flags)
{
  this->thr_mgr (&this->thread_manager_);
}

TAO_Pool_Per_Endpoint::~TAO_Pool_Per_Endpoint (void)
{
}

void
TAO_Pool_Per_Endpoint::run (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Acceptor_Registry *ac =
    this->orb_->orb_core ()->acceptor_registry ();

  for (TAO_AcceptorSetIterator i = ac->begin (); i != ac->end (); ++i)
    {
      int priority =
        ACE_Sched_Params::priority_min (this->policy_);

#if (TAO_HAS_RT_CORBA == 1)

      CORBA::Object_var obj =
        this->orb_->orb_core ()->priority_mapping_manager ();
      
      TAO_Priority_Mapping_Manager_var mapping_manager = 
        TAO_Priority_Mapping_Manager::_narrow (obj.in (),
                                               ACE_TRY_ENV);

      RTCORBA::PriorityMapping *pm =
        mapping_manager.in ()->mapping ();

      const CORBA::Short corba_priority = (*i)->priority ();
      CORBA::Short native_priority;
      if (pm->to_native (corba_priority, native_priority) == 1)
        priority = native_priority;
      else
        ACE_THROW (CORBA::DATA_CONVERSION ());

      if (TAO_debug_level > 3)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - creating thread at priority %d:%d\n"),
                    priority, corba_priority));
#endif /* TAO_HAS_RT_CORBA == 1 */
      if (this->activate (this->flags_,
                          this->poolsize_, /* number of threads */
                          1, /* force active */
                          priority) == -1)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }
    }
  this->thread_manager_.wait ();
}

int
TAO_Pool_Per_Endpoint::svc (void)
{
  if (TAO_debug_level > 3)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - TAO_Pool_Per_Endpoint::svc: ")
                ACE_TEXT (" using reactor <%x> in this thread\n"),
                this->orb_->orb_core ()->reactor ()));

  this->orb_->run ();

  if (TAO_debug_level > 3)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - TAO_Pool_Per_Endpoint::svc: ")
                ACE_TEXT (" ORB::run() finished\n")));
 return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
