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

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

ACE_RCSID (RTPortableServer, RTPortableServer, "$Id$")

TAO_RTPortableServer_Initializer::TAO_RTPortableServer_Initializer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Object_Adapter_Factory);
}

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
