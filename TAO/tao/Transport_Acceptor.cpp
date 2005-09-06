// -*- C++ -*-
//
// $Id$

#include "Transport_Acceptor.h"

#if !defined (__ACE_INLINE__)
# include "Transport_Acceptor.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Acceptor,
           "$Id$")

TAO_Acceptor::TAO_Acceptor (CORBA::ULong tag)
  : tag_ (tag)
{
}

TAO_Acceptor::~TAO_Acceptor (void)
{

  //@@ TAO_ACCEPTOR_DESTRUCTOR_ADD_HOOK
}

//@@ TAO_ACCEPTOR_SPL_METHODS_ADD_HOOK
