// -*- C++ -*-

//=============================================================================
/**
 * @file LB_Component.h
 *
 * $Id$

 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_LB_COMPONENT_H
#define TAO_LB_COMPONENT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/LoadBalancing/LoadBalancing_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LB_ORTC.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

/**
 * @class TAO_LB_Component
 *
 * @brief Service_Object that can be used to add load balancer support
 *        to a CORBA-based server.
 *
 * Servers wishing to transparently add load balancing/management
 * support load this object via the ACE_Service_Configurator (e.g. by
 * listing it in a `svc.conf' file).
 */
class TAO_LoadBalancing_Export TAO_LB_Component
  : public ACE_Service_Object
{
public:

  /// Initializes object when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Terminates object when dynamic unlinking occurs.
  virtual int fini (void);

protected:

  /// Register the LB_Component's ORBInitializer.
  int register_orb_initializer (const CORBA::StringSeq & object_groups,
                                const CORBA::StringSeq & repository_ids,
                                const char * location);

};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_LoadBalancing, TAO_LB_Component)
  //ACE_STATIC_SVC_REQUIRE (TAO_LB_Component)
ACE_FACTORY_DECLARE (TAO_LoadBalancing, TAO_LB_Component)


#include /**/ "ace/post.h"

#endif  /* TAO_LB_COMPONENT_H */
