// $Id$

#include "TSS_Resources.h"
#include "GUIResource_Factory.h"

ACE_RCSID (tao,
           TSS_Resources,
           "$Id$")

TAO_TSS_Resources::TAO_TSS_Resources (void)
  :  poa_current_impl_ (0)
     , rtscheduler_current_impl_ (0)
     , rtscheduler_previous_current_impl_ (0)
     , default_environment_ (&this->tss_environment_)

#if (TAO_HAS_CORBA_MESSAGING == 1)

  , policy_current_ (&this->initial_policy_current_)

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
     , gui_resource_factory_ (0)

{
}

TAO_TSS_Resources::~TAO_TSS_Resources (void)
{
  if (gui_resource_factory_)
    delete gui_resource_factory_;
  gui_resource_factory_ = 0;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_TSS_Singleton<TAO_TSS_Resources, TAO_SYNCH_MUTEX>;
template class ACE_TSS<TAO_TSS_Resources>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_TSS_Singleton<TAO_TSS_Resources, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_TSS_Resources>

#elif defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)

template
  TAO_TSS_Singleton<TAO_TSS_Resources, TAO_SYNCH_MUTEX> *
  TAO_TSS_Singleton<TAO_TSS_Resources, TAO_SYNCH_MUTEX>::singleton_;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
