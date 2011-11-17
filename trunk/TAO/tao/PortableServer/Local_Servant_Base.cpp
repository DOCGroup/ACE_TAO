// $Id$

#include "tao/PortableServer/Local_Servant_Base.h"
#include "tao/SystemException.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Local_Servant_Base.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Local_ServantBase::_dispatch (
  TAO_ServerRequest &,
  TAO::Portable_Server::Servant_Upcall*)
{
  throw ::CORBA::BAD_OPERATION ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
