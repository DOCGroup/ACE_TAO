//=============================================================================
/**
 *  @file    Policy_Verifier.h
 *
 *  $Id$
 *
 *   This class verifies that the policy are correctly embedded
 *   in the IOR.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef POLICY_VERIFIER_H_
#define POLICY_VERIFIER_H_

// -- App. Specific Include --
#include "CounterC.h"
#include "RT_Properties.h"

// -- ACE Include --
#include "ace/Arg_Shifter.h"
#include "ace/Log_Msg.h"

// -- TAO Include --
#include "tao/Basic_Types.h"
#include "tao/ORB_Constants.h"

class Policy_Verifier
{
public:

  // -- Ctor/Dtor --
  Policy_Verifier (void);
  ~Policy_Verifier (void);

  bool init (int argc, ACE_TCHAR *argv[]);

  void run (void);

private:
  // -- Helper methods --
  void verify_reference (Counter_ptr counter,
                         RT_Properties *rt_properties);

  CORBA::Boolean check_reference (CORBA::Object_ptr object,
                                  const char *msg);

private:

  CORBA::ORB_var orb_;

  RT_Properties *rt_object_properties_;
  RT_Properties *rt_poa_properties_;
  unsigned int priority_bands_;
  Counter_var base_object_;
  Counter_var overridden_object_;

  ACE_TCHAR base_object_ref_[256];
  ACE_TCHAR overridden_object_ref_[256];
};

#endif /* POLICY_VERIFIER_H_ */
