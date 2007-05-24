//
// $Id$
//

#include "tao/RTCORBA/RT_Current.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Priority_Mapping.h"

#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Protocols_Hooks.h"
#include "tao/SystemException.h"

#include "ace/Thread.h"

ACE_RCSID (RTCORBA,
           RT_Current,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_Current::TAO_RT_Current (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

TAO_RT_Current::~TAO_RT_Current (void)
{
}

RTCORBA::Priority
TAO_RT_Current::the_priority (void)
{
  TAO_Protocols_Hooks *tph = this->orb_core_->get_protocols_hooks ();

  RTCORBA::Priority priority = 0;

  if (tph != 0)
    {
      if (tph->get_thread_CORBA_priority (priority) == -1)
        throw ::CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO);
    }

  return priority;
}

void
TAO_RT_Current::the_priority (RTCORBA::Priority the_priority)
{

  TAO_Protocols_Hooks *tph = this->orb_core_->get_protocols_hooks ();

  if (tph != 0)
    {
      if (tph->set_thread_CORBA_priority (the_priority) == -1)
        throw ::CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
