
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
  explicit AMI_Test_i (CORBA::ORB_ptr orb,
              CORBA::Long in_l = 0,
              char const *in_str = nullptr,
              bool check_params = false);
  AMI_Test_i (AMI_Test_i const &) = delete;
  AMI_Test_i &operator= (AMI_Test_i const &) = delete;
  AMI_Test_i (AMI_Test_i &&) = delete;
  AMI_Test_i &operator= (AMI_Test_i &&) = delete;

  // The AMI_Test methods.
  CORBA::Long foo (CORBA::Long_out out_l,
                   CORBA::Long in_l,
                   char const *in_str) override;

  void shutdown () override;

  CORBA::Long yadda () override;

  void yadda (CORBA::Long yadda) override;

  void inout_arg_test (char *&str) override;

private:
  CORBA::ORB_var orb_;

  CORBA::Long yadda_ {};

  CORBA::Long in_l_ {};

  CORBA::String_var in_str_;

  bool check_params_ {};
};

#endif /* TAO_AMI_TEST_I_H */

