// $Id$
#include "orbsvcs/Notify/MonitorControl/Control_Registry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Control_Registry*
TAO_Control_Registry::instance (void)
{
  return TAO_Singleton<TAO_Control_Registry, TAO_SYNCH_MUTEX>::instance();
}

TAO_NS_Control*
TAO_Control_Registry::get (const ACE_CString& name) const
{
  return dynamic_cast<TAO_NS_Control*> (this->getobj (name));
}

TAO_END_VERSIONED_NAMESPACE_DECL
