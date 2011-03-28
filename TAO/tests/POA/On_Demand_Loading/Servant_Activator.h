// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Servant_Activator.h
 *
 *  $Id$
 *
 *   Defines a <ServantActivator_i> class, which uses a
 *   Servant_Manager to activate a servant by loading it and
 *   associates it with an object on-demand.
 *
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "Servant_Manager.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/LocalObject.h"

class ServantActivator_i :
  public virtual PortableServer::ServantActivator,
  public virtual ::CORBA::LocalObject

{
  //= TITLE
  //   Servant Activator for the test servant.
  //
  //= DESCRIPTION
  //   This class associates an unassociated servant with an object in
  //   the POA Active Object Map.
public:
  /// Initialization.
  ServantActivator_i (CORBA::ORB_ptr orb);

  /**
   * This method is invoked by a POA with USE_SERVANT_MANAGER and
   * RETAIN policies, whenever it receives a request for a test object
   * that is not currently active. When an servant pointer
   * corresponding to objectId is not found in the Active Object Map,
   * the POA hands over the job of obtaining the servant to the
   * Servant Manager. Depending upon whether the POA is created with
   * RETAIN or NON_RETAIN as the servant_retention policy, the Servant
   * Activator or the Servant Locator interface is invoked
   * respectively.
   */
  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa);

  /**
   * This method is invoked whenever a test servant is
   * deactivated. This occurs when the POA is destroyed or the Object
   * is deactivated. When the POA is getting destroyed, it needs to
   * deactivate every object in the Active Object Map and on that call
   * the ServantActivator invokes this method which will destroy the
   * servant associated with the object.
   */
  virtual void etherealize (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant servant,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations);

  /// Returns an ObjectId when given a DLL name and the factory
  /// function in the DLL that will create an appropriate Servant.
  PortableServer::ObjectId_var create_dll_object_id (const char *dllname,
                                                     const char *factory_function);

private:
  /**
   * An ServantManager object that provides utility methods.  The
   * methods include obtaining the servant using an ACE_DLL object,
   * destroying the servant and extracting the dllname and factory
   * function from the ObjectId.
   */
  ServantManager_i servant_manager_;
};

#endif /* SERVANT_ACTIVATOR_H */
