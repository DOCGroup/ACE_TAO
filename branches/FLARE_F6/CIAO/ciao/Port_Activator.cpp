#include "Port_Activator.h"

ACE_RCSID (ciao,
           Servant_Activator,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "Port_Activator.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  Port_Activator::Port_Activator (const char *oid,
                                  const char *name,
                                  Type t)
    : oid_ (oid),
      name_ (name),
      t_ (t)
  {
  }

  Port_Activator::~Port_Activator (void)
  {
  }
}
