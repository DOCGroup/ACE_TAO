// $Id$

#ifndef FT_DETECTORFACTORY_I_H_
#define FT_DETECTORFACTORY_I_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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

class  FT_FaultDetectorFactory_i : public virtual POA_FT::FaultDetectorFactory
{
  //////////////////////
  // non-CORBA interface
public:
  FT_FaultDetectorFactory_i ();

  virtual ~FT_FaultDetectorFactory_i ();

  int parse_args (int argc, char * argv[]);
  int self_register (TAO_ORB_Manager & orbManager);
  void identity (ostream & out) const;

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

  ACE_Thread_Manager threadManager_;
};

#endif /* FT_DETECTORFACTORY_I_H_  */
