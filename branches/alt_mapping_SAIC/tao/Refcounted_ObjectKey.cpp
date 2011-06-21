// -*- C++ -*-
// $Id$

#include "tao/Refcounted_ObjectKey.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "tao/Refcounted_ObjectKey.inl"
#endif /* defined INLINE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Refcounted_ObjectKey::Refcounted_ObjectKey (const TAO::ObjectKey &key)
  : object_key_ (key)
  , ref_count_ (1)
{
}

TAO::Refcounted_ObjectKey::~Refcounted_ObjectKey (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
