#include "Port_Activator.h"

ACE_RCSID (ciao,
           Servant_Activator,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "Port_Activator_i.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  Port_Activator_i::Port_Activator_i (const char *oid,
                                      const char *name,
                                      Port_Activator_Types::Type t)
    : oid_ (oid),
      name_ (name),
      t_ (t)
  {
  }

  Port_Activator_i::~Port_Activator_i (void)
  {
  }
}
