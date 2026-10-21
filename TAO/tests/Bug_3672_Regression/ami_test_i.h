
//=============================================================================
/**
 *  @file   ami_test_i.h
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
  AMI_Test_i (AMI_Test_i const &) = delete;
  AMI_Test_i (AMI_Test_i &&) = delete;
  AMI_Test_i &operator= (AMI_Test_i const &) = delete;
  AMI_Test_i &operator= (AMI_Test_i &&) = delete;

  // The AMI_Test methods.
  CORBA::Long foo (CORBA::Long_out out_l) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* TAO_AMI_TEST_I_H */

