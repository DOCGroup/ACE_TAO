/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "PublishesDef_i.h"

ACE_RCSID(IFR_Service, PublishesDef_i, "$Id$")

TAO_PublishesDef_i::TAO_PublishesDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_EventDef_i (repo, section_key)
{
}

TAO_PublishesDef_i::~TAO_PublishesDef_i (void)
{
}

IR::DefinitionKind
TAO_PublishesDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Publishes;
}

void 
TAO_PublishesDef_i::destroy (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_PublishesDef_i::describe (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

