// $Id$

#include "Registry_i.h"

/*
registry_impl::registry_impl ()
{
}

registry_impl::registry_impl (CORBA::Long x)
{
  this->value (x);
} */

registry_factory::registry_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (registry)
registry_factory::create_for_unmarshal ()
{
  return new registry_impl;
}

void print_registries (const registries &v)
{
  ACE_DEBUG ((LM_DEBUG, "sequence<registry>: "));

  for (unsigned int i = 0 ; i < v.length (); i++)
    {
      ACE_DEBUG ((LM_DEBUG, "%d ", v[i]->value ()));
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));
}
