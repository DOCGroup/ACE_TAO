
//=============================================================================
/**
 *  @file   ami_test_i.h
 *
 *  $Id$
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef TAO_AMI_MSM_I_H
#define TAO_AMI_MSM_I_H

#include "ami_msmS.h"

class AMI_Msm_i : public POA_A::AMI_Msm
{
public:
  /// ctor
  AMI_Msm_i (CORBA::ORB_ptr orb);

  // The AMI_Test methods.
  void foo (const char* string, ::CORBA::String_out answer);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;

  long answer_count_;
};

#endif /* TAO_AMI_TEST_I_H */

