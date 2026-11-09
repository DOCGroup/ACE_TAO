#include "Signature.h"

Signature::Signature (ACE_CString const &name)
  : name_ (name)
{
}

void
Signature::used ()
{
  used_++;
}

int
Signature::used_count() const
{
  return used_;
}

ACE_CString const &
Signature::name() const
{
  return name_;
}
