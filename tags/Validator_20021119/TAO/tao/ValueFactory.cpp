// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ValueFactory.cpp
//
// = AUTHOR
//     Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================


#include "tao/ValueFactory.h"

#if (TAO_HAS_VALUETYPE == 1)

#if !defined (__ACE_INLINE__)
# include "tao/ValueFactory.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           ValueFactory, "$Id$")

CORBA_ValueFactoryBase::~CORBA_ValueFactoryBase (void)
{
}

// No-op. This should never be called, but it can't be pure virtual.
CORBA::AbstractBase *
CORBA_ValueFactoryBase::create_for_unmarshal_abstract (void)
{
  return 0;
}

#endif /* TAO_HAS_VALUETYPE == 1*/
