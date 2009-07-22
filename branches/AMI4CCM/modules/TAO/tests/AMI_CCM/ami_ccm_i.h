
//=============================================================================
/**
 *  @file   ami_ccm_i.h
 *
 *  $Id$
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef TAO_AMI_CCM_I_H
#define TAO_AMI_CCM_I_H

#include "ami_ccmS.h"

class AMI_CCM_i : public POA_A::AMI_CCM
{
public:
  /// ctor
  AMI_CCM_i (CORBA::ORB_ptr orb);

  // The AMI_Test methods.
  CORBA::Long asynch_foo (const char* string, ::CORBA::Long client_id,
      ::CORBA::String_out answer);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;

  long answer_count_;
};

#endif /* TAO_AMI_CCM_I_H */

