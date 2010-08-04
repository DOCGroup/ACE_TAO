#include "Port_Activator.h"

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

  char*
  Port_Activator_i::oid (void)
  {
    return CORBA::string_dup(this->oid_.in ());
  }

  void
  Port_Activator_i::oid (const char* oid)
  {
    this->oid_ = oid;
  }

  char*
  Port_Activator_i::name (void)
  {
    return CORBA::string_dup(this->name_.in ());
  }
}
