/* -*- C++ -*- */
// $Id$

#include "PublishesDef_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           PublishesDef_i, 
           "$Id$")

TAO_PublishesDef_i::TAO_PublishesDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_EventPortDef_i (repo)
{
}

TAO_PublishesDef_i::~TAO_PublishesDef_i (void)
{
}

CORBA::DefinitionKind
TAO_PublishesDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Publishes;
}

