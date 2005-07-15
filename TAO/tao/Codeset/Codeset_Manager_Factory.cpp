/* -*- C++ -*- */

// =================================================================
/**
 * @file Codeset_Manager_Factory.cpp
 *
 * $Id$
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 *
 */
// =================================================================

#include "Codeset_Manager_Factory.h"
#include "ace/Service_Config.h"
#include "Codeset_Manager_i.h"
#include "tao/Resource_Factory.h"
#include "tao/ORB_Core.h"

ACE_RCSID (Codeset, Codeset_Manager_Factory, "$Id$")

TAO_Codeset_Manager_Factory::~TAO_Codeset_Manager_Factory ()
{
}

TAO_Codeset_Manager *
TAO_Codeset_Manager_Factory::create (TAO_ORB_Core *oc)
{
  TAO_Codeset_Manager_i *csm = 0;
  ACE_NEW_RETURN (csm, TAO_Codeset_Manager_i, 0);

  const TAO_Codeset_Descriptor *cd =
    oc->resource_factory()->get_codeset_descriptor(0); // char
  if (cd->ncs_set())
    csm->set_ncs_c(cd->ncs());
  const TAO_Codeset_Descriptor::Translator_Node *tlist =
    cd->translators();
  while (tlist != 0)
    {
      csm->add_char_translator (tlist->name_);
      tlist = tlist->next_;
    }

  cd = oc->resource_factory()->get_codeset_descriptor(1); // wchar
  if (cd->ncs_set())
    csm->set_ncs_w(cd->ncs(), cd->max_bytes());
  tlist = cd->translators();
  while (tlist != 0)
    {
      csm->add_wchar_translator (tlist->name_);
      tlist = tlist->next_;
    }

  return csm;
}

ACE_FACTORY_DEFINE (TAO_Codeset, TAO_Codeset_Manager_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Codeset_Manager_Factory,
                       ACE_TEXT ("TAO_Codeset"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Codeset_Manager_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
