// Framework_Component_T.cpp
// $Id$
#ifndef ACE_FRAMEWORK_COMPONENT_T_CPP
#define ACE_FRAMEWORK_COMPONENT_T_CPP

#include "ace/Framework_Component_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Framework_Component_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Framework_Component_T, "$Id$")

template <class Concrete>
ACE_Framework_Component_T<Concrete>::ACE_Framework_Component_T (const Concrete *concrete)
  : ACE_Framework_Component ((const void *) concrete)
{
  ACE_TRACE ("ACE_Framework_Component_T<Concrete>::ctor");
}

template <class Concrete>
ACE_Framework_Component_T<Concrete>::~ACE_Framework_Component_T (void)
{
  ACE_TRACE ("ACE_Framework_Component_T<Concrete>::~ACE_Framework_Component_T");
  Concrete::close_singleton ();
}

#endif /*ACE_FRAWORK_COMPONENT_T_CPP*/
