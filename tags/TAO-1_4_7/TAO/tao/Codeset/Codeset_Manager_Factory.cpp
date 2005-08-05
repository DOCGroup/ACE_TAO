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

ACE_RCSID (Codeset, Codeset_Manager_Factory, "$Id$")

TAO_Codeset_Manager_Factory::~TAO_Codeset_Manager_Factory ()
{
}

int
TAO_Codeset_Manager_Factory::is_default() const
{
  return 0;
}


TAO_Codeset_Manager *
TAO_Codeset_Manager_Factory::create ()
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
