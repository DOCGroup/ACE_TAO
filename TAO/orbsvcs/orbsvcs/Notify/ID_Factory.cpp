#include "ID_Factory.h"

#if ! defined (__ACE_INLINE__)
#include "ID_Factory.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ID_Factory::TAO_Notify_ID_Factory ()
  : seed_ (0)
{
}

TAO_Notify_ID_Factory::~TAO_Notify_ID_Factory ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
