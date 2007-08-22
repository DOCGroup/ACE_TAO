// $Id$

#include "tao/Transport_Acceptor.h"

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Acceptor.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Acceptor,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Acceptor::TAO_Acceptor (CORBA::ULong tag)
  : tag_ (tag)
{
}

TAO_Acceptor::~TAO_Acceptor (void)
{

  //@@ TAO_ACCEPTOR_DESTRUCTOR_ADD_HOOK
}

//@@ TAO_ACCEPTOR_SPL_METHODS_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
