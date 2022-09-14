#include "Activator_Info.h"

Activator_Info::Activator_Info ()
  : name ("")
  , token (0)
  , ior ("")
  , activator ()
{
}

Activator_Info::Activator_Info (const Activator_Info &other)
  : name (other.name)
  , token (other.token)
  , ior (other.ior)
  , activator (other.activator)
{
}
Activator_Info::Activator_Info (const ACE_CString& aname,
                                CORBA::Long atoken,
                                const ACE_CString& aior,
                                ImplementationRepository::Activator_ptr act)
  : name (aname)
  , token (atoken)
  , ior (aior)
  , activator (ImplementationRepository::Activator::_duplicate (act))
{
}

void
Activator_Info::clear ()
{
  this->name = "";
  this->token = 0;
  this->ior = "";
  this->activator = ImplementationRepository::Activator::_nil ();
}

void
Activator_Info::reset_runtime ()
{
  this->ior = "";
  this->activator = ImplementationRepository::Activator::_nil ();
}
