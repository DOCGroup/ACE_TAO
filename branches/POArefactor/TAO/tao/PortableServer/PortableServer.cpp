/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PortableServer.cpp
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#include "PortableServer.h"
#include "Object_Adapter.h"
#include "Thread_Policy_Value.h"
#include "Loadable_Thread_Policy.h"

ACE_RCSID (PortableServer,
           PortableServer,
           "$Id$")

int
TAO_POA_Initializer::init (void)
{
  ACE_Service_Config::process_directive (
    TAO::ace_svc_desc_ORB_CTRL_Thread_Policy);

  ACE_Service_Config::process_directive (
    TAO::ace_svc_desc_SINGLE_THREAD_Thread_Policy);

  ACE_Service_Config::process_directive (
    TAO::ace_svc_desc_Loadable_Thread_Policy);

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_Object_Adapter_Factory
      );
}
