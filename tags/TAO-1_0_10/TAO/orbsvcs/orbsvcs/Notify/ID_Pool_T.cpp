// $Id$

#ifndef ID_POOL_T_C
#define ID_POOL_T_C

#include "ID_Pool_T.h"

#if !defined (__ACE_INLINE__)
#include "ID_Pool_T.i"
#endif /* __ACE_INLINE__ */

template <class ID_TYPE> void
ID_Pool<ID_TYPE>::put (ID_TYPE /* id */)
{
  // TBD:
}

template <class ID_TYPE> ID_TYPE
ID_Pool<ID_TYPE>::get (void)
{
   return ++id_;
}

#endif /* ID_POOL_T_C */
