// $Id$

#include "series_i.h"
#include "Registry_i.h"

ACE_RCSID(series, series_i, "$Id$")

// Constructor
series_i::series_i (void)
{
  // no-op
}

// Copy  Constructor
series_i::series_i (series_i &t)
  : POA_series (t)
{
  // no-op
}

// Destructor

series_i::~series_i (void)
{
  // no-op
}

// Set the ORB pointer.

void
series_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return the current date/time on the server.

registry *
series_i::cube (registry *v, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  long x = v->value ();
  x = x * x * x;

  registry_var retv = new registry_impl ();
  retv->value (x);
  return retv._retn ();
}

registries *
series_i::sum (const registries &v, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  long r = 0;

  registries_var retv;
  retv = new registries;
  retv->length (v.length ());

  for (unsigned int i = 0; i < v.length (); i++)
    {
      r += v[i]->value ();
      retv[i] = new registry_impl;
      retv[i]->value (r);
    }

  return retv._retn ();
}

// Shutdown.

void
series_i::shutdown (CORBA::Environment & )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "series_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
