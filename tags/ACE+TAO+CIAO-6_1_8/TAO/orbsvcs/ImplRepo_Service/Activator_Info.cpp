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
{
}
void
Activator_Info::reset()
{
  ior = "";
  activator = ImplementationRepository::Activator::_nil();
}
