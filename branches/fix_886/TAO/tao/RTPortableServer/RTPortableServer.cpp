/* -*- C++ -*- */

// $Id$
// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    RTPortableServer.cpp
//
// = AUTHOR
//    Frank Hunleth <fhunleth@cs.wustl.edu>
//
// =================================================================

#include "RTPortableServer.h"
#include "RT_Object_Adapter_Factory.h"

ACE_RCSID (RTPortableServer, RTPortableServer, "$Id$")

int
TAO_RT_POA_Initializer::init (void)
{
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_RT_Object_Adapter_Factory);

  return 0;
}
