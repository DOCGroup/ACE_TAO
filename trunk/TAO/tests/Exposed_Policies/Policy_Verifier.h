// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO/tests/Exposed_Policies
//
// = FILENAME
//    Policy_Verifier.h
//
// = DESCRIPTION
//     This class verifies that the policy are correctly embedded
//     in the IOR.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef POLICY_VERIFIER_H_
#define POLICY_VERIFIER_H_

// -- App. Specific Include --
#include "CounterC.h"
#include "RT_Properties.h"

// -- ACE Include --
#include "ace/Arg_Shifter.h"

// -- TAO Include --
#include "tao/corba.h"

class Policy_Verifier
{
public:

  // -- Ctor/Dtor --
  Policy_Verifier (void);
  ~Policy_Verifier (void);

  void init (int argc,
             char *argv[]
             TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  void run (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

private:
  // -- Helper methods --
  void verify_reference (Counter_ptr counter,
                         RT_Properties *rt_properties
                         TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  CORBA::Boolean check_reference (CORBA::Object_ptr object,
                                  const char *msg);

private:

  CORBA::ORB_var orb_;

  RT_Properties *rt_object_properties_;
  RT_Properties *rt_poa_properties_;
  unsigned int priority_bands_;
  Counter_var base_object_;
  Counter_var overridden_object_;

  char base_object_ref_[256];
  char overridden_object_ref_[256];
};

#endif /* POLICY_VERIFIER_H_ */
