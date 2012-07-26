// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_Condition_Attributes::ACE_Condition_Attributes (int type)
{
  (void) ACE_OS::condattr_init (this->attributes_, type);
}

ACE_INLINE
ACE_Condition_Attributes::~ACE_Condition_Attributes (void)
{
  ACE_OS::condattr_destroy (this->attributes_);
}

ACE_END_VERSIONED_NAMESPACE_DECL
