/* -*- C++ -*- */

// $Id$
// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    PortableServer.cpp
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// =================================================================

#include "PortableServer.h"
#include "Object_Adapter.h"

ACE_RCSID (PortableServer, PortableServer, "$Id$")

int
TAO_POA_Initializer::init (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Object_Adapter_Factory);
}
