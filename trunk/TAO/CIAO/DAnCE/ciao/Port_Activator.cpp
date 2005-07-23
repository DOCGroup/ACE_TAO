#include "Port_Activator.h"

ACE_RCSID (ciao,
           Servant_Activator,
           "$Id$")

namespace CIAO
{
  Port_Activator::Port_Activator (const char *oid,
                                  const char *name,
                                  Type t)
    : oid_ (oid)
    , name_ (name)
    , t_ (t)
  {
  }

  Port_Activator::~Port_Activator (void)
  {
  }

  const char*
  Port_Activator::name (void) const
  {
    // @@ TODO, need to be inlined.
    return this->oid_.in ();
  }

  void
  Port_Activator::set_name (const char* name)
  {
    this->oid_ = name;
  }
}
