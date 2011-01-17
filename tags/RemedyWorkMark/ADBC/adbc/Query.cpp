// $Id$

#include "Query.h"

#if !defined (__ADBC_INLINE__)
#include "Query.inl"
#endif

namespace ADBC
{
//
// destroy
//
void Query::destroy (void)
{
  delete this;
}
}
