/* -*- C++ -*- */
// $Id$

#include "IRObject_i.h"

ACE_RCSID(IFR_Service, IRObject_i, "$Id$")

TAO_IRObject_i::TAO_IRObject_i (TAO_Repository_i *repo,
                                ACE_Configuration_Section_Key section_key)
  : repo_ (repo),
    section_key_ (section_key)
{
}

TAO_IRObject_i::~TAO_IRObject_i (void)
{
}

char *
TAO_IRObject_i::int_to_string (CORBA::ULong number) const
{
  char retval[9];

  ACE_OS::sprintf (retval, "%8.8X", number);

  return CORBA::string_dup (retval);
}

