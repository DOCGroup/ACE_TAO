//
// $Id$
//

#include "tao/RTCORBA/RT_Current.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Priority_Mapping.h"

#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Protocols_Hooks.h"

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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Protocols_Hooks *tph = this->orb_core_->get_protocols_hooks ();

  RTCORBA::Priority priority;

  int const result = tph->get_thread_CORBA_priority (priority);

  if (result == -1)
    ACE_THROW_RETURN (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO), -1);

  return priority;
}

void
TAO_RT_Current::the_priority (RTCORBA::Priority the_priority)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  TAO_Protocols_Hooks *tph = this->orb_core_->get_protocols_hooks ();

  int const result = tph->set_thread_CORBA_priority (the_priority);

  if (result == -1)
    throw ::CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
