// -*- C++ -*-

//=============================================================================
/**
 * @file  LB_GenericFactory.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_GENERIC_FACTORY_H
#define TAO_LB_GENERIC_FACTORY_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServerC.h"
#include "orbsvcs/PortableGroupC.h"

#include "LB_Location_Map.h"

/// Forward declarations.
class TAO_LB_ObjectGroup_Map;
class TAO_LB_PropertyManager;

struct TAO_LB_ObjectGroup_Map_Entry;


/**
 * @class TAO_LB_GenericFactory
 *
 * @brief PortableGroup::GenericFactory implementation used by the
 *        load balancer when creating object groups.
 *
 * This GenericFactory creates an object group reference for given set
 * of replicas.  Those replicas will be created by this GenericFactory
 * if the "infrastructure-controlled" membership style is configured.
 */
class TAO_LB_GenericFactory
  : public virtual PortableGroup::GenericFactory
{
public:

  /// Constructor.
  TAO_LB_GenericFactory (TAO_LB_Location_Map &location_map,
                         TAO_LB_ObjectGroup_Map &object_group_map,
                         TAO_LB_PropertyManager &property_manager);

  /**
   * @name TAO_LoadBalancer::GenericFactory methods
   */
  //@{

  /**
   * Create an object of the specified type that adheres to the
   * restrictions defined by the provided Criteria.  The out
   * FactoryCreationId parameter may be passed to the delete_object()
   * method to delete the object.
   */
  virtual CORBA::Object_ptr create_object (
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      PortableGroup::GenericFactory::FactoryCreationId_out
        factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::NoFactory,
                     PortableGroup::ObjectNotCreated,
                     PortableGroup::InvalidCriteria,
                     PortableGroup::InvalidProperty,
                     PortableGroup::CannotMeetCriteria));

  /**
   * Delete the object corresponding to the provided
   * FactoryCreationId.  If the object is actually an ObjectGroup,
   * then all members within the ObjectGroup will be deleted.
   * Afterward, the ObjectGroup itself will be deleted.
   */
  virtual void delete_object (
      const PortableGroup::GenericFactory::FactoryCreationId &
        factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectNotFound));

  //@}

  /// Set the POA to use when creating object references.
  void poa (PortableServer::POA_ptr p);

private:

  /// Populate the object group being created.  Called when the
  /// infrastructure-controlled membership style is used for the
  /// object group being created.
  void populate_object_group (
         TAO_LB_ObjectGroup_Map_Entry *entry,
         const PortableGroup::FactoryInfos &factory_infos,
         CORBA::Environment &ACE_TRY_ENV);

  /// Get a new ObjectId to be used when creating a new ObjectGroup.
  /**
   * An ObjectId created by this method will never be reused within
   * the scope of a given ReplicationManager.  A value suitable for
   * use in a map association <ext_id> is also returned.
   */
  void get_ObjectId (CORBA::ULong fcid,
                     PortableServer::ObjectId_out oid);


private:

  /// Reference to the POA used to create object group references.
  PortableServer::POA_var poa_;

  /// Reference to the Location map.
  TAO_LB_Location_Map &location_map_;

  /// Reference to the ObjectGroup map.
  TAO_LB_ObjectGroup_Map &object_group_map_;

  /// Reference to the PropertyManager.
  TAO_LB_PropertyManager &property_manager_;

  /// The FactoryCreationId that will be assigned to the next object
  /// group that is created.
  /**
   * Value that is used when assigning a FactoryCreationId to the
   * factory that was used to create a given ObjectGroup.  The
   * FactoryCreationId is typically comprised of this value in
   * addition to another value that makes it unique to a given Load
   * Balancer.
   */
  CORBA::ULong next_fcid_;

  /// Lock used to synchronize access to the factory creation id
  /// index (i.e. next_fcid_).
  TAO_SYNCH_MUTEX lock_;

};

#endif  /* TAO_LB_GENERIC_FACTORY_H */
