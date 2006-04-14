// -*- C++ -*-

//=============================================================================
/**
 * @file LB_ClientComponent.h
 *
 * $Id$

 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_LB_CLIENTCOMPONENT_H
#define TAO_LB_CLIENTCOMPONENT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/LoadBalancing/LoadBalancing_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LB_ORTC.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

/**
 * @class TAO_LB_ClientComponent
 *
 * @brief Service_Object that can be used to add load balancer support
 *        to a CORBA-based client.
 *
 * CORBA applications wishing to transparently add load shedding support
 * can load this object via the ACE_Service_Configurator (e.g. by
 * listing it in a `svc.conf' file).
 */
class TAO_LoadBalancing_Export TAO_LB_ClientComponent
  : public ACE_Service_Object
{
public:

  /// Initializes object when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Terminates object when dynamic unlinking occurs.
  virtual int fini (void);

protected:

  /// Register the LB_ClientComponent's ORBInitializer.
  int register_orb_initializer (void);

};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_LoadBalancing, TAO_LB_ClientComponent)
ACE_STATIC_SVC_REQUIRE (TAO_LB_ClientComponent)
ACE_FACTORY_DECLARE (TAO_LoadBalancing, TAO_LB_ClientComponent)


#include /**/ "ace/post.h"

#endif  /* TAO_LB_CLIENTCOMPONENT_H */
