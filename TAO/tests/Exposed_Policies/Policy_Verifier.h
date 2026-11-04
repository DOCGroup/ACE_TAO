//=============================================================================
/**
 *  @file    Policy_Verifier.h
 *
 *   This class verifies that the policy are correctly embedded
 *   in the IOR.
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
#include <memory>

class Policy_Verifier
{
public:
  // -- Ctor/Dtor --
  Policy_Verifier () = default;
  Policy_Verifier (Policy_Verifier const &) = delete;
  Policy_Verifier (Policy_Verifier &&) = delete;
  Policy_Verifier &operator= (Policy_Verifier const &) = delete;
  Policy_Verifier &operator= (Policy_Verifier &&) = delete;

  bool init (int argc, ACE_TCHAR *argv[]);

  void run ();

private:
  // -- Helper methods --
  void verify_reference (Counter_ptr counter,
                         RT_Properties const &rt_properties);

  CORBA::Boolean check_reference (CORBA::Object_ptr object,
                                  char const *msg);

private:
  CORBA::ORB_var orb_ {};

  std::unique_ptr<RT_Properties> rt_object_properties_ {};
  std::unique_ptr<RT_Properties> rt_poa_properties_ {};
  CORBA::ULong priority_bands_ {};
  Counter_var base_object_ {};
  Counter_var overridden_object_ {};

  ACE_TString base_object_ref_ {ACE_TEXT ("file://default.ior")};
  ACE_TString overridden_object_ref_ {ACE_TEXT ("file://overridden.ior")};
};

#endif /* POLICY_VERIFIER_H_ */
