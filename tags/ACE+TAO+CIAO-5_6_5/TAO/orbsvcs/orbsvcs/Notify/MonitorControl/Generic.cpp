// $Id$
#include "orbsvcs/Notify/MonitorControl/Generic.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_NS_Generic::~TAO_NS_Generic (void)
{
}

const ACE_CString&
TAO_NS_Generic::name (void) const
{
  return this->name_;
}

TAO_NS_Generic::TAO_NS_Generic (const char* name)
 : name_ (name)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
