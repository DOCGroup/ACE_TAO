// $Id$

#define ACE_BUILD_DLL

#if !defined (ACE_MANAGED_OBJECT_CPP)
#define ACE_MANAGED_OBJECT_CPP

#include "ace/Managed_Object.h"

#if !defined (__ACE_INLINE__)
#include "ace/Managed_Object.i"
#endif /* __ACE_INLINE__ */

template <class TYPE>
ACE_Cleanup_Adapter<TYPE>::~ACE_Cleanup_Adapter (void)
{
}

#endif /* ACE_MANAGED_OBJECT_CPP */
