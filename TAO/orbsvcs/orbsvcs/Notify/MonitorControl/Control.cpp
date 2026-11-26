#include "orbsvcs/Notify/MonitorControl/Control.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_NS_Control::~TAO_NS_Control () = default;

TAO_NS_Control::TAO_NS_Control (char const *name)
 : name_ (name)
{
}

ACE_CString const&
TAO_NS_Control::name () const
{
  return this->name_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
