// $Id$
//=============================================================================
/**
 *  @file    DetectorFactory_i.h
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

#ifndef FT_DETECTORFACTORY_I_H_
#define FT_DETECTORFACTORY_I_H_
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Vector_T.h"

//////////////////////////////////
// Classes declared in this header
class  FT_FaultDetectorFactory_i;

/////////////////////////////////
// Includes needed by this header
#include "orbsvcs/FT_DetectorFactoryS.h"
#include <ace/Thread_Manager.h>

/////////////////////
// Forward references
class TAO_ORB_Manager;
class Detector_i;

/**
 * Implement the FaultDetectorFactory interface.
 * 
 * Because each FaultDetector runs in a separate thread, care
 * must be taken to insure that the thread comes to an end, and
 * that the detector_i object is deleted at the right time.  Hence,
 * the FaultDetector life cycle:
 *  Creation:
 *  A dector_i object to implement a fault detector is created 
 *  in response to the create_object call on the factory.
 *  A pointer to the detector_i is stored in the detectors_ table.
 *  The start method of the detector_i is called to create a thread
 *  that will monotor the object-to-be-monitored.  An ACE_Thread_Manager
 *  supplied by the factory is used to track the thread.
 *
 *  Destruction:
 *  If the factory wants the detector to go away, it calls the
 *  quitRequested method of the detector which sets a flag that must
 *  be checked regularly by the detector.
 *  If the object being monitored faults, the detector sends the 
 *  notification message then sets its own quit requested flag.
 *  When a detector discovers the quit requested flag has been set
 *  it calls the removeDetector method of the factory, then ends
 *  the thread.
 *  The removeDetector method of the factory removes the detector from
 *  the detectors_ collection, then deletes the detector_i object.
 *  The factory closes the ACE_Thread_Manager to ensure that all
 *  detector threads have departed before the factory itself is
 *  deleted.
 *
 */
class  FT_FaultDetectorFactory_i : public virtual POA_FT::FaultDetectorFactory
{
  typedef ACE_Vector<Detector_i *> DetectorVec;

  //////////////////////
  // non-CORBA interface
public:
  FT_FaultDetectorFactory_i ();

  virtual ~FT_FaultDetectorFactory_i ();

  int parse_args (int argc, char * argv[]);
  int self_register (TAO_ORB_Manager & orbManager);
  const char * identity () const;

  void removeDetector(CORBA::ULong id, Detector_i * detector);

  void shutdown_i();

  //////////////////
  // CORBA interface
  virtual void change_properties (
      const FT::Properties & property_set
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void shutdown ()
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CORBA::Object_ptr create_object (
    const char * type_id,
    const FT::Criteria & the_criteria,
    FT::GenericFactory::FactoryCreationId_out factory_creation_id
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , FT::NoFactory
    , FT::ObjectNotCreated
    , FT::InvalidCriteria
    , FT::InvalidProperty
    , FT::CannotMeetCriteria
  ));

  virtual void delete_object (
    const FT::GenericFactory::FactoryCreationId & factory_creation_id
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , FT::ObjectNotFound
  ));

  /////////////////
  // Implementation
private:
  int write_IOR();
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
  typedef ACE_Guard<ACE_Mutex> InternalGuard;
  ACE_Mutex internals_;

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

  ACE_CString identity_;

  ACE_Thread_Manager threadManager_;

  DetectorVec detectors_;
};

#endif /* FT_DETECTORFACTORY_I_H_  */
