// $Id$

#ifndef ACE_MANAGED_OBJECT_CPP
#define ACE_MANAGED_OBJECT_CPP

#include "Managed_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "Managed_Object.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Managed_Object, "$Id$")

template <class TYPE>
ACE_Cleanup_Adapter<TYPE>::~ACE_Cleanup_Adapter (void)
{
}

#endif /* ACE_MANAGED_OBJECT_CPP */
