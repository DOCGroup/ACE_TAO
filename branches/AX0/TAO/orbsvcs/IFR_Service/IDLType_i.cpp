/* -*- C++ -*- */
// $Id$

#include "IDLType_i.h"

ACE_RCSID(IFR_Service, IDLType_i, "$Id$")

TAO_IDLType_i::TAO_IDLType_i (TAO_Repository_i *repo,
                              ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key)
{
}

TAO_IDLType_i::~TAO_IDLType_i (void)
{
}

