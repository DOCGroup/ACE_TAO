// $Id$

#include "RMCast_Singleton_Factory.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Singleton_Factory.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Singleton_Factory, "$Id$")

ACE_RMCast_Singleton_Factory::~ACE_RMCast_Singleton_Factory (void)
{
}

ACE_RMCast_Module*
ACE_RMCast_Singleton_Factory::create (void)
{
  return this->singleton_;
}

void
ACE_RMCast_Singleton_Factory::destroy (ACE_RMCast_Module *)
{
}
