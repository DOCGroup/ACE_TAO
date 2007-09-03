// $Id$
#include "orbsvcs/Notify/MonitorControl/Statistic_Registry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Statistic_Registry*
TAO_Statistic_Registry::instance (void)
{
  return TAO_Singleton<TAO_Statistic_Registry, TAO_SYNCH_MUTEX>::instance();
}

TAO_Statistic*
TAO_Statistic_Registry::get (const ACE_CString& name) const
{
  return dynamic_cast<TAO_Statistic*> (this->getobj (name));
}

TAO_END_VERSIONED_NAMESPACE_DECL
