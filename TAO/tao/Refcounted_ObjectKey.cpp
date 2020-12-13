// -*- C++ -*-
#include "Refcounted_ObjectKey.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "Refcounted_ObjectKey.inl"
#endif /* defined INLINE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Refcounted_ObjectKey::Refcounted_ObjectKey (const TAO::ObjectKey &key)
  : object_key_ (key)
  , refcount_ (1)
{
}

TAO::Refcounted_ObjectKey::~Refcounted_ObjectKey ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
