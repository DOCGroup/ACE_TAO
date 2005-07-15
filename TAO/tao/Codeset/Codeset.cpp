/* -*- C++ -*- */

// =================================================================
/**
 * @file Codeset.cpp
 *
 * $Id$
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 *
 */
// =================================================================

#include "Codeset.h"
#include "Codeset_Manager_Factory.h"

ACE_RCSID (Codeset, Codeset, "$Id$")


int
TAO_Codeset_Initializer::init (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Codeset_Manager_Factory);
}
