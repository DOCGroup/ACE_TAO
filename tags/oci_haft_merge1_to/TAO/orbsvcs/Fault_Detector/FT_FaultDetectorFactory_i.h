/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_FaultDetectorFactory_i.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  It declares the implementation of FaultDetectorFactory which
 *  creates and manages FaultDetectors as an agent for
 *  the ReplicationManager as defined in the FT CORBA specification.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef FT_FAULTDETECTORFACTORY_I_H_
#define FT_FAULTDETECTORFACTORY_I_H_
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//////////////////////////////////
// Classes declared in this header
class  FT_FaultDetectorFactory_i;

/////////////////////////////////
// Includes needed by this header
#include <ace/Vector_T.h>
#include <orbsvcs/FT_FaultDetectorFactoryS.h>
#include <ace/Thread_Manager.h>

/////////////////////
// Forward references
class TAO_ORB_Manager;
class Fault_Detector_i;

/**
 * Implement the FaultDetectorFactory interface.
 *
 * Because each FaultDetector runs in a separate thread, care
 * must be taken to insure that the thread comes to an end, and
 * that the Fault_Detector_i object is deleted at the right time.
 * Hence, the FaultDetector life cycle:
 *  Creation:
 *  The create_object method of the factory creates a Fault_Detector_i
 *
 *  A pointer to the Fault_Detector_i is stored in the detectors_ table.
 *
 *  The start method of the Fault_Detector_i is called to create a thread
 *  that will monotor the object-to-be-monitored.  An ACE_Thread_Manager
 *  supplied by the factory is used to keep track of the thread.
 *
 *  Destruction:
 *
 *  If the factory wants the detector to go away, it calls the
 *  quitRequested method of the detector which sets a flag that must
 *  be checked regularly by the detector.
 *
 *  If the object being monitored faults, the detector sends the
 *  notification message then sets its own quit requested flag.
 *
 *  When a detector discovers the quit requested flag has been set
 *  it calls the removeDetector method of the factory, then ends
 *  the thread.
 *
 *  The removeDetector method of the factory removes the detector from
 *  the detectors_ collection, then deletes the Fault_Detector_i object.
 *
 *  Shutdown:
 *
 *  The destructor of the factory calls the shutdown method of all
 *  remaining Fault_Detector_i objects.
 *
 *  It then closes the ACE_Thread_Manager to ensure that all
 *  detector threads have departed before the factory itself is
 *  deleted.
 *
 */
class  FT_FaultDetectorFactory_i : public virtual POA_FT::FaultDetectorFactory
{
  typedef ACE_Vector<Fault_Detector_i *> DetectorVec;

  //////////////////////
  // non-CORBA interface
public:
  /**
   * Default constructor.
   */
  FT_FaultDetectorFactory_i ();

  /**
   * Virtual destructor.
   */
  virtual ~FT_FaultDetectorFactory_i ();

  /**
   * Parse command line arguments.
   * @param argc traditional C argc
   * @param argv traditional C argv
   * @return zero for success; nonzero is process return code for failure.
   */
  int parse_args (int argc, char * argv[]);

  /**
   * Publish this objects IOR, and otherwise start things rolling.
   * @param orbManager our ORB -- we keep var to it.
   * @return zero for success; nonzero is process return code for failure.
   */
  int self_register (TAO_ORB_Manager & orbManager ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * Revoke the publication of this objects IOR.
   * @return zero for success; nonzero is process return code for failure.
   */
  int self_unregister (ACE_ENV_SINGLE_ARG_DECL);


  /**
   * Identify this fault detector factory.
   * @return a string to identify this object for logging/console message purposes.
   */
  const char * identity () const;

  /**
   * Remove pointer to individual detector; delete Fault_Detector_i.
   * See FaultDetector life cycle description.
   * @param id the numerical id assigned to this FaultDetector.
   * @param detector a pointer to the detector object (redundant for safety.)
   */
  void removeDetector (CORBA::ULong id, Fault_Detector_i * detector);

  //////////////////
  // CORBA interface
  // See IDL for documentation

  ///////////////////////////////////////////////
  // CORBA interface FaultDetectorFactory methods
  virtual void change_properties (
      const FT::Properties & property_set
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      , PortableGroup::InvalidProperty
    ));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  /////////////////////////////////////////
  // CORBA interface GenericFactory methods
  virtual CORBA::Object_ptr create_object (
    const char * type_id,
    const FT::Criteria & the_criteria,
    FT::GenericFactory::FactoryCreationId_out factory_creation_id
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::NoFactory
    , PortableGroup::ObjectNotCreated
    , PortableGroup::InvalidCriteria
    , PortableGroup::InvalidProperty
    , PortableGroup::CannotMeetCriteria
  ));

  virtual void delete_object (
    const FT::GenericFactory::FactoryCreationId & factory_creation_id
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::ObjectNotFound
  ));

  //////////////////////////////////////////
  // CORBA interface PullMonitorable methods

  virtual CORBA::Boolean is_alive (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /////////////////////////
  // Implementation methods
private:
  /**
   * Write this factory's IOR to a file
   */
  int write_IOR ();

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
  ACE_Mutex internals_;
  typedef ACE_Guard<ACE_Mutex> InternalGuard;

  /**
   * The orb
   */
  CORBA::ORB_var orb_;

  /**
   * IOR of this object as assigned by orb.
   */
  CORBA::String_var ior_;

  /**
   * A file to which the factory's IOR should be written.
   */
  const char * ior_output_file_;

  /**
   * A name to be used to register the factory with the name service.
   */
  const char * nsName_;

  CosNaming::NamingContext_var naming_context_;

  CosNaming::Name this_name_;

  /**
   * Quit on idle flag.
   */
  int quitOnIdle_;

  /**
   * A human-readable string to distinguish this from other Notifiers.
   */
  ACE_CString identity_;

  /**
   * A manager for all FaultDetector threads.
   */
  ACE_Thread_Manager threadManager_;

  /**
   * A vector of FaultDetectors.  Note that the FaultDetector ID
   * is an index into this vector.  Vector slots are not reused.
   */
  DetectorVec detectors_;

  /**
   * count of entries in detectors_ that have been deleted.
   * Used to determine when the factory is idle.
   */
  size_t removed_;
};

#endif /* FT_FAULTDETECTORFACTORY_I_H_  */
