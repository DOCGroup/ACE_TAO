//
// $Id$
//

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecSchedulerC.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Sequence<CosNaming::NameComponent>;
template class TAO_Unbounded_Sequence<CosNaming::Binding>;
template class TAO_Unbounded_Sequence<RtecEventComm::Event>;
template class TAO_Unbounded_Sequence<RtecEventChannelAdmin::Dependency>;
template class TAO_Unbounded_Sequence<RtecEventChannelAdmin::Publication>;
template class TAO_Unbounded_Sequence<RtecScheduler::Dependency_Info>;
template class TAO_Unbounded_Sequence<RtecScheduler::RT_Info>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Sequence<CosNaming::NameComponent>
#pragma instantiate TAO_Unbounded_Sequence<CosNaming::Binding>
#pragma instantiate TAO_Unbounded_Sequence<RtecEventComm::Event>
#pragma instantiate TAO_Unbounded_Sequence<RtecEventChannelAdmin::Dependency>
#pragma instantiate TAO_Unbounded_Sequence<RtecEventChannelAdmin::Publication>
#pragma instantiate TAO_Unbounded_Sequence<RtecScheduler::Dependency_Info>
#pragma instantiate TAO_Unbounded_Sequence<RtecScheduler::RT_Info>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
