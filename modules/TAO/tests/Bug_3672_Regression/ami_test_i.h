
//=============================================================================
/**
 *  @file   ami_test_i.h
 *
 *  $Id$
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef TAO_AMI_TEST_I_H
#define TAO_AMI_TEST_I_H

#include "ami_testS.h"

/**
 * @class AMI_Test_i
 *
 * @brief AMI Test implementation
 *
 * Implements the AMI_Test interface in test.idl
 */
class AMI_Test_i : public POA_A::AMI_Test
{
public:
  /// ctor
  AMI_Test_i (CORBA::ORB_ptr orb);

  // The AMI_Test methods.
  CORBA::Long foo (CORBA::Long_out out_l);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
};

#endif /* TAO_AMI_TEST_I_H */

