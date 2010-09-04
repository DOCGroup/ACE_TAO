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

#include "tao/Codeset/Codeset_Manager_Factory.h"
#include "ace/Service_Config.h"
#include "tao/Codeset/Codeset_Manager_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Codeset_Manager_Factory::~TAO_Codeset_Manager_Factory (void)
{
}

bool
TAO_Codeset_Manager_Factory::is_default (void) const
{
  return false;
}


TAO_Codeset_Manager *
TAO_Codeset_Manager_Factory::create (void)
{
  TAO_Codeset_Manager_i *csm = 0;
  ACE_NEW_RETURN (csm, TAO_Codeset_Manager_i, 0);

  return csm;
}

ACE_FACTORY_DEFINE (TAO_Codeset, TAO_Codeset_Manager_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Codeset_Manager_Factory,
                       ACE_TEXT ("TAO_Codeset"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Codeset_Manager_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL

