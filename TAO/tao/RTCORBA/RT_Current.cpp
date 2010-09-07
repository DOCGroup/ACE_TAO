// $Id$

#include "tao/RTCORBA/RT_Current.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Priority_Mapping.h"

#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Protocols_Hooks.h"
#include "tao/SystemException.h"

#include "ace/Thread.h"

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

  int result =
      tph->get_thread_CORBA_priority (priority);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "ERROR: TAO_RT_Current::the_priority. "
                    "RT CORBA Priority accessed in a thread where it has not been set.\n"));
      // Spec does not define a minor code
      throw CORBA::INITIALIZE ();
    }

  return priority;
}

void
TAO_RT_Current::the_priority (RTCORBA::Priority the_priority)
{

  TAO_Protocols_Hooks *tph = this->orb_core_->get_protocols_hooks ();

  if (tph->set_thread_CORBA_priority (the_priority) == -1)
    {
      // Note this check is required by "2.6 Real-time Current" which states:
      // "A BAD_PARAM system exception shall be thrown if an attempt is made to set the
      // priority to a value outside the range 0 to 32767."
      // Note that it is not enough to assume that the mapping's rejection will deal with
      // this as it also says: "If the to_native call returns FALSE ... then a Real-time ORB
      // shall raise a DATA_CONVERSION system exception" i.e different exception.

      if (the_priority < 0) // short
        throw CORBA::BAD_PARAM (
                  CORBA::SystemException::_tao_minor_code (
                  0,
                  EINVAL),
                  CORBA::COMPLETED_NO);

      throw CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
