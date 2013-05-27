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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_ServantBase;
TAO_END_VERSIONED_NAMESPACE_DECL

/**
 * @class ImR_Adapter_Activator
 *
 * @brief Implementation Repository Adapter Activator
 *
 * Part of the DefaultServant/AdapterActivator combination that is used to
 * receive forwardable requests from clients.  The Adapter Activator creates
 * the POA structure that the request expects.  For each POA created, the
 * same DefaultServant will be registered in each one.
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

  void init(TAO_ServantBase * servant);
private:
  /// the default servant used.
  TAO_ServantBase *default_servant_;
};

#endif /* IMR_ADAPTER_ACTIVATOR_H */
