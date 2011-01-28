// -*- C++ -*-
//=============================================================================
/**
 *  @file   Adapter_Activator.h
 *
 *  $Id$
 *
 *  @brief  This class implements ImR's Adapter Activator.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef IMR_ADAPTER_ACTIVATOR_H
#define IMR_ADAPTER_ACTIVATOR_H

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/AdapterActivatorC.h"
#include "tao/PortableServer/ServantLocatorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"


/**
 * @class ImR_Adapter_Activator
 *
 * @brief Implementation Repository Adapter Activator
 *
 * Part of the ServantLocator/AdapterActivator combination that is used to
 * receive forwardable requests from clients.  The Adapter Activator creates
 * the POA structure that the request expects.  For each POA created, the
 * same ServantLocator will be registered in each one.
 */
class ImR_Adapter
  : public PortableServer::AdapterActivator,
    public CORBA::LocalObject
{
public:
  ImR_Adapter();

  /// Called when a POA needs to be created.
  virtual CORBA::Boolean unknown_adapter (
    PortableServer::POA_ptr parent,
    const char *name
  );

  void init(PortableServer::ServantLocator_ptr servant);
private:
  /// The ServantLocator registered in each new POA.
  PortableServer::ServantLocator_ptr servant_locator_;
};

#endif /* IMR_ADAPTER_ACTIVATOR_H */
