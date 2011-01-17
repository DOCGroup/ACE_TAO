/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicaFactory_i.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  It declares the implementation of ReplicaFactory which
 *  creates and manages replicas as an agent for
 *  the ReplicationManager as defined in the FT CORBA specification.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef FT_REPLICAFACTORY_H_
#define FT_REPLICAFACTORY_H_
#include <ace/ACE.h>
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//////////////////////////////////
// Classes declared in this header
class  FT_ReplicaFactory_i;

/////////////////////////////////
// Includes needed by this header
#include <ace/Vector_T.h>
#include "FT_TestReplicaS.h"
#include <ace/Thread_Manager.h>
#include <ace/SString.h>
#include <orbsvcs/FT_ReplicationManagerC.h>

/////////////////////
// Forward references
class TAO_ORB_Manager;
class FT_TestReplica_i;

/**
 * Implement the GenericFactory interface.
 */
class  FT_ReplicaFactory_i
//FT_TEST::ReplicaFactory
  : public virtual POA_PortableGroup::GenericFactory
{
  typedef ACE_Vector<FT_TestReplica_i *> ReplicaVec;
  typedef ACE_Vector<ACE_CString> StringVec;

  //////////////////////
  // non-CORBA interface
public:
  /**
   * Default constructor.
   */
  FT_ReplicaFactory_i ();

  /**
   * Virtual destructor.
   */
  virtual ~FT_ReplicaFactory_i ();

  /**
   * Parse command line arguments.
   * @param argc traditional C argc
   * @param argv traditional C argv
   * @return zero for success; nonzero is process return code for failure.
   */
  int parse_args (int argc, ACE_TCHAR * argv[]);

  /**
   * Initialize this object.
   * @param orb our ORB -- we keep var to it.
   * @return zero for success; nonzero is process return code for failure.
   */
  int init (CORBA::ORB_ptr orb);

  /**
   * Prepare to exit.
   * @return zero for success; nonzero is process return code for failure.
   */
  int fini (void);

  int idle(int & result);


  /**
   * Identify this replica factory.
   * @return a string to identify this object for logging/console message purposes.
   */
  const ACE_TCHAR * identity () const;

  const char * location () const;

  /**
   * Remove pointer to individual replica; delete FT_TestReplica_i.
   * See replica life cycle description.
   * @param id the numerical id assigned to this replica.
   * @param replica a pointer to the Replica object (redundant for safety.)
   */
  void remove_replica (CORBA::ULong id, FT_TestReplica_i * replica);

  //////////////////
  // CORBA interface
  // See IDL for documentation

  virtual void shutdown (void);

  /////////////////////////////////////////
  // CORBA interface GenericFactory methods
  virtual CORBA::Object_ptr create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id
  );

  virtual void delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId & factory_creation_id
  );

  //////////////////////////////////////////
  // CORBA interface PullMonitorable methods

  virtual CORBA::Boolean is_alive (void);

  /////////////////////////
  // Implementation methods
private:
  /**
   * Actual replica creation happens in this method.
   * @param name becomes part of the objects identity.
   */
  FT_TestReplica_i * create_replica(const char * name);

  /**
   * Find or allocate an ID for a new replica
   */
  CORBA::ULong allocate_id();

  /**
   * Write this factory's IOR to a file
   */
  int write_ior (const ACE_TCHAR * outputFile, const char * ior);

  /**
   * Clean house for factory shut down.
   */
  void shutdown_i ();

  ///////////////
  // Data Members
private:

  /**
   * Protect internal state.
   * Mutex should be locked by corba methods, or by
   * external (public) methods before calling implementation
   * methods.
   * Implementation methods should assume the mutex is
   * locked if necessary.
   */
  TAO_SYNCH_MUTEX internals_;

  /**
   * The orb
   */
  CORBA::ORB_var orb_;

  /**
   * The POA used to activate this object.
   */
  PortableServer::POA_var poa_;

  /**
   * The CORBA object id assigned to this object.
   */
  PortableServer::ObjectId_var object_id_;

  /**
   * IOR of this object as assigned by poa
   */
  CORBA::String_var ior_;

  /**
   * A file to which the factory's IOR should be written.
   */
  const ACE_TCHAR * ior_output_file_;

  /**
   * A human-readable string to distinguish this from other Notifiers.
   */
  ACE_TString identity_;

  /**
   * bool: true if we found a replication manager
   */
  int have_replication_manager_;

  /**
   * The replication manager
   */

  ::FT::ReplicationManager_var replication_manager_;


  /**
   * The factory registry IOR
   */
  const ACE_TCHAR * factory_registry_ior_;

  /**
   * The factory registry with which to register.
   */
  PortableGroup::FactoryRegistry_var factory_registry_;

  /**
   * true if registered with FactoryRegistry
   */
  int registered_; // bool

  /**
   * A file to which the test replica's IOR will be written
   */
  const ACE_TCHAR * test_output_file_;

  /**
   * A name to be used to register the factory with the name service.
   */
  ACE_CString ns_name_;

  CosNaming::NamingContext_var naming_context_;

  CosNaming::Name this_name_;

  /////////////////
  // The roles used to register types
  StringVec roles_;

  /**
   * the PortableGroup::Location within the domain
   */
  ACE_CString location_;

  /**
   * bool: quit on idle flag.
   */
  int quit_on_idle_;

  /**
   * bool: use a single call to unregister.
   */
  int unregister_by_location_;

  /**
   * A vector of Replicas.  Note that the Replica ID
   * is an index into this vector.
   */
  ReplicaVec replicas_;

  /**
   * count of entries in Replicas_ that have been deleted.
   * Used to determine when the factory is idle and to avoid futile
   * searches for empty slots.
   */
  size_t empty_slots_;

  /**
   * boolean: starts false.  Set to true when it's time to quit.
   */
  int quit_requested_;

  /**
  * A file that use by FT_TestReplica_i object
  */
  const ACE_TCHAR* name_persistent_file_;

};

#endif /* FT_REPLICAFACTORY_H_  */
