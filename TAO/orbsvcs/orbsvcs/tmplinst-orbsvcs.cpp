// $Id$

#include "orbsvcs/CosLifeCycleC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosPropertyServiceC.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecSchedulerC.h"

ACE_RCSID(orbsvcs, tmplinst_orbsvcs, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Object_Manager<CORBA_Object>;
template class TAO_Object_Manager<CORBA_TypeCode>;
template class TAO_Unbounded_Object_Sequence<CORBA_Object>;
template class TAO_Unbounded_Pseudo_Sequence<CORBA_TypeCode>;

#if defined(TAO_ORBSVCS_HAS_LifeCycle)
template class TAO_Unbounded_Sequence<CosLifeCycle::NVP>;
#endif /* TAO_ORBSVCS_HAS_LifeCycle */

#if defined(TAO_ORBSVCS_HAS_Naming)
template class TAO_Unbounded_Sequence<CosNaming::NameComponent>;
template class TAO_Unbounded_Sequence<CosNaming::Binding>;
#endif /* TAO_ORBSVCS_HAS_Naming */

#if defined(TAO_ORBSVCS_HAS_Property)
template class TAO_Unbounded_Sequence<CosPropertyService::Property>;
template class TAO_Unbounded_Sequence<CosPropertyService::PropertyDef>;
template class TAO_Unbounded_Sequence<CosPropertyService::PropertyException>;
template class TAO_Unbounded_Sequence<CosPropertyService::PropertyMode>;
#endif /* TAO_ORBSVCS_HAS_Property */

#if defined(TAO_ORBSVCS_HAS_Event)
template class TAO_Unbounded_Sequence<RtecEventComm::Event>;
template class TAO_Unbounded_Sequence<RtecEventChannelAdmin::Dependency>;
template class TAO_Unbounded_Sequence<RtecEventChannelAdmin::Publication>;
#endif /* TAO_ORBSVCS_HAS_Event */

#if defined(TAO_ORBSVCS_HAS_Sched)
template class TAO_Unbounded_Sequence<RtecScheduler::Dependency_Info>;
template class TAO_Unbounded_Sequence<RtecScheduler::RT_Info>;
#endif /* TAO_ORBSVCS_HAS_Sched */

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Object_Manager<CORBA_Object>
#pragma instantiate TAO_Object_Manager<CORBA_TypeCode>
#pragma instantiate TAO_Unbounded_Object_Sequence<CORBA_Object>
#pragma instantiate TAO_Unbounded_Pseudo_Sequence<CORBA_TypeCode>

#if defined(TAO_ORBSVCS_HAS_LifeCycle)
#pragma instantiate TAO_Unbounded_Sequence<CosLifeCycle::NVP>
#endif /* TAO_ORBSVCS_HAS_LifeCycle */

#if defined(TAO_ORBSVCS_HAS_Naming)
#pragma instantiate TAO_Unbounded_Sequence<CosNaming::NameComponent>
#pragma instantiate TAO_Unbounded_Sequence<CosNaming::Binding>
#endif /* TAO_ORBSVCS_HAS_Naming */

#if defined(TAO_ORBSVCS_HAS_Property)
#pragma instantiate TAO_Unbounded_Sequence<CosPropertyService::Property>
#pragma instantiate TAO_Unbounded_Sequence<CosPropertyService::PropertyDef>
#pragma instantiate TAO_Unbounded_Sequence<CosPropertyService::PropertyException>
#pragma instantiate TAO_Unbounded_Sequence<CosPropertyService::PropertyMode>
#endif /* TAO_ORBSVCS_HAS_Property */

#if defined(TAO_ORBSVCS_HAS_Event)
#pragma instantiate TAO_Unbounded_Sequence<RtecEventComm::Event>
#pragma instantiate TAO_Unbounded_Sequence<RtecEventChannelAdmin::Dependency>
#pragma instantiate TAO_Unbounded_Sequence<RtecEventChannelAdmin::Publication>
#endif /* TAO_ORBSVCS_HAS_Event */

#if defined(TAO_ORBSVCS_HAS_Sched)
#pragma instantiate TAO_Unbounded_Sequence<RtecScheduler::Dependency_Info>
#pragma instantiate TAO_Unbounded_Sequence<RtecScheduler::RT_Info>
#endif /* TAO_ORBSVCS_HAS_Sched */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
