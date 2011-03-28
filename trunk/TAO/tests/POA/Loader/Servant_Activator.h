// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Servant_Activator.h
 *
 *  $Id$
 *
 *   Defines a <ServantActivator> class, which activates a servant by
 *   obtaining it and associates it with an object on-demand.
 *
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/LocalObject.h"

#include "tao/ORB.h"

#include "ace/DLL.h"
#include "ace/Log_Msg.h"

class ServantActivator :
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
  /**
   * This typedef is used to typecast the void* obtained when finding
   * a symbol in the dll. Invoking the function pointer obtained would
   * get a servant.
   */
  typedef PortableServer::Servant
           (*SERVANT_FACTORY) (const PortableServer::ObjectId &oid,
                               PortableServer::POA_ptr poa,
                               CORBA::ORB_ptr orb);

  /**
   * This typedef is used to obtain the garbage_collection_function symbol
   * in the dll. Invoking the function pointer obtained would then destroy
   * the servant.
   */
  typedef void
          (*SERVANT_GARBAGE_COLLECTOR) (const PortableServer::ObjectId &oid,
                                        PortableServer::POA_ptr,
                                        PortableServer::Servant servant);

  /// Initialization.
  ServantActivator (CORBA::ORB_ptr orb,
                    const ACE_TCHAR *dllname,
                    const ACE_TCHAR *factory_function,
                    const ACE_TCHAR *garbage_collection_function);

  /**
   * This method is invoked by a POA with USE_SERVANT_MANAGER and
   * RETAIN policies, whenever it receives a request for a
   * test object that is not currently active. When an servant
   * pointer corresponding to objectId is not found in the Active
   * Object Map, the POA hands over the job of obtaining the servant
   * to the Servant Manager. Depending upon whether the POA is created
   * with RETAIN or NON_RETAIN as the servant_retention policy, the
   * Servant Activator or the Servant Locator interface is invoked
   * respectively.
   */
  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa);

  /**
   * This method is invoked whenever a test object is
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

private:
  /// The ACE_DLL object which performs the task of loading the dll
  /// and accessing it.
  ACE_DLL dll_;

  /// The function pointer of factory_function type.
  SERVANT_FACTORY servant_supplier_;

  /// The function pointer of garbage_collection_function type.
  SERVANT_GARBAGE_COLLECTOR servant_garbage_collector_;

  /// A reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif /* SERVANT_ACTIVATOR_H */
