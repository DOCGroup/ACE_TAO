// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 * @file HasherFactory.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_HASHER_FACTORY_H
#define TAO_HASHER_FACTORY_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Functor.h"
#include "ace/Hash_Map_Manager_T.h"

#include "orbsvcs/LoadBalancingS.h"

#include "Hasher_i.h"


/**
 * @class HasherFactory
 *
 * @brief GenericFactory implementation used to create instances of
 *        "Hasher" objects.
 *
 * This factory is only designed to create Hasher objects.  However, a
 * GenericFactory need not be limited creating only one type of
 * object.  This HasherFactory has been designed specifically for this
 * test, and is designed to create objects at multiple logical
 * locations.  For example, the all replicas will reside in the same
 * process but each will reside at different logical locations.
 */
class HasherFactory
  : public virtual POA_LoadBalancing::GenericFactory
{
public:

  /// Constructor.
  HasherFactory (CORBA::ORB_ptr orb,
                 PortableServer::POA_ptr root_poa);

  /// Destructor.
  ~HasherFactory (void);

  /// Return a reference to a "Hasher" object.
  virtual CORBA::Object_ptr create_object (
      const char *type_id,
      const LoadBalancing::Criteria &the_criteria,
      LoadBalancing::GenericFactory::FactoryCreationId_out
        factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::NoFactory,
                     LoadBalancing::ObjectNotCreated,
                     LoadBalancing::InvalidCriteria,
                     LoadBalancing::InvalidProperty,
                     LoadBalancing::CannotMeetCriteria));

  /// Destroy a "Hasher" object created by this factory that
  /// corresponds to the given FactoryCreationId.
  virtual void delete_object (
     const LoadBalancing::GenericFactory::FactoryCreationId
       &factory_creation_id,
     CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectNotFound));

  /// Return the RepositoryId of the type of object this Factory
  /// creates.
  static const char *replica_type_id (void);

  /// This Factory internally implements the FactoryCreationId as a
  /// CORBA::ULong for efficiency reasons.  Interactions with this
  /// Factory from objects is still done via the
  /// LoadBalancing::GenericFactory::FactoryCreationId type.
  typedef CORBA::ULong FactoryCreationId;

  typedef ACE_Hash_Map_Manager_Ex<FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex> Table;

private:

  /// Initialize the HasherFactory if it hasn't already been
  /// initialized.
  void init (CORBA::Environment &ACE_TRY_ENV);

  /// Helper method that parses any Criteria passed to this Factory.
  void parse_criteria (const LoadBalancing::Criteria &criteria,
                       CORBA::Environment &ACE_TRY_ENV);

  /// Generate a FactoryCreationId for the given ObjectId.
  /**
   * This method also binds the generated FactoryCreationId to the
   * ObjectId in the underlying table.
   */
  FactoryCreationId bind_fcid (const PortableServer::ObjectId &oid,
                               CORBA::Environment &ACE_TRY_ENV);

  /// Unbind the ObjectId associated with the given
  /// FactoryCreationId from the underlying table.  The ObjectId is
  /// returned as an "out" argument.
  void unbind_fcid (const FactoryCreationId &fcid,
                    PortableServer::ObjectId_out oid,
                    CORBA::Environment &ACE_TRY_ENV);

private:

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

  /// Reference to the RootPOA.
  PortableServer::POA_var root_poa_;

  /// POA responsible for creation of the replica references.
  PortableServer::POA_var poa_;

  /// The shared Hasher servant.
  /*
   * Multiple ObjectIds map to this servant.  Only one servant
   * instance is necessary for this test, hence multiple object
   * references point to this servant.
   */
  Hasher_i hasher_;

  /// Lock used to provide synchronization.
  TAO_SYNCH_MUTEX lock_;

  /// Flag that specifies whether or not the Factory has been
  /// initialized.
  CORBA::Boolean initialized_;

  /// The FactoryCreationId that will be assigned to the object
  /// created by this factory.
  HasherFactory::FactoryCreationId next_fcid_;

  /// Table that maps FactoryCreationId to ObjectId.
  Table table_;

};

#endif  /* TAO_HASHER_FACTORY_H */
