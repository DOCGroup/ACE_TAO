#include "ValueFactory.h"

#if !defined (__ACE_INLINE__)
# include "ValueFactory.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           ValueFactory,
           "$Id$")


CORBA::ValueFactoryBase::~ValueFactoryBase (void)
{
}

// No-op. This should never be called, but it can't be pure virtual.
CORBA::AbstractBase *
CORBA::ValueFactoryBase::create_for_unmarshal_abstract (void)
{
  return 0;
}

