// $Id$

#include "tao/TSS_Resources.h"
#include "tao/GUIResource_Factory.h"
#include "tao/TAO_Singleton.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_TSS_Resources::TAO_TSS_Resources (void)
  : poa_current_impl_ (0)
  , rtscheduler_current_impl_ (0)
  , rtscheduler_previous_current_impl_ (0)
  , rtcorba_current_priority_ (-1) // RTCORBA::minPriority - 1
  , default_environment_ (&tss_environment_)

#if (TAO_HAS_CORBA_MESSAGING == 1)

  , policy_current_ (&initial_policy_current_)

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
  , gui_resource_factory_ (0)
#if (TAO_HAS_TRANSPORT_CURRENT == 1)
  , tsg_ (0)
#endif /* TAO_HAS_TRANSPORT_CURRENT */
{
}

TAO_TSS_Resources::~TAO_TSS_Resources (void)
{
  delete this->gui_resource_factory_;
}

TAO_TSS_Resources *
TAO_TSS_Resources::instance (void)
{
  // Hide the template instantiation to prevent multiple instances
  // from being created.
  return
    TAO_TSS_Singleton<TAO_TSS_Resources, TAO_SYNCH_MUTEX>::instance ();
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template
  TAO_TSS_Singleton<TAO_TSS_Resources, TAO_SYNCH_MUTEX> *
  TAO_TSS_Singleton<TAO_TSS_Resources, TAO_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

TAO_END_VERSIONED_NAMESPACE_DECL
