/* -*- C++ -*- */
#include "orbsvcs/IFRService/PublishesDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"

TAO_PublishesDef_i::TAO_PublishesDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Contained_i (repo),
    TAO_EventPortDef_i (repo)
{
}

TAO_PublishesDef_i::~TAO_PublishesDef_i ()
{
}

CORBA::DefinitionKind
TAO_PublishesDef_i::def_kind ()
{
  return CORBA::dk_Publishes;
}

