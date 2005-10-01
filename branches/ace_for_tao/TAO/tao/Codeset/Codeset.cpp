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
#include "UTF8_Latin1_Factory.h"
#include "UTF16_BOM_Factory.h"

#include "ace/Dynamic_Service.h"


ACE_RCSID (Codeset, Codeset, "$Id$")


int
TAO_Codeset_Initializer::init (void)
{
  int result = 0;

  result += ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_UTF8_Latin1_Factory);

  result += ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_UTF16_BOM_Factory);

  TAO_Codeset_Manager_Factory *cmf = ACE_Dynamic_Service<TAO_Codeset_Manager_Factory>::instance("TAO_Codeset");

  if (cmf != 0)
    {
      return result;
    }

  result +=  ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_Codeset_Manager_Factory,1); // force replacement
  return result;
}
