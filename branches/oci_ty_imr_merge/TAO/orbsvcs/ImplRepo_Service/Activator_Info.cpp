// $Id$
#include "Activator_Info.h"

Activator_Info::Activator_Info
(const ACE_CString& aname,
 CORBA::Long atoken,
 const ACE_CString& aior,
 ImplementationRepository::Activator_ptr act)
 : name(aname)
 , token(atoken)
 , ior(aior)
 , activator (ImplementationRepository::Activator::_duplicate (act))
 , waiting_clients(0)
{
}
void
Activator_Info::reset()
{
  ior = "";
  activator = ImplementationRepository::Activator::_nil();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Strong_Bound_Ptr<Activator_Info, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Strong_Bound_Ptr<Activator_Info, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
