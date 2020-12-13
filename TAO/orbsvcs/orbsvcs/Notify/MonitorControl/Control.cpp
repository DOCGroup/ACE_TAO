#include "Control.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_NS_Control::~TAO_NS_Control ()
{
}

TAO_NS_Control::TAO_NS_Control (const char* name)
 : name_ (name)
{
}

const ACE_CString&
TAO_NS_Control::name () const
{
  return this->name_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
