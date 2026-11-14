
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Nanbor Wang
 */
//=============================================================================

#ifndef TAO_INTERCEPTOR_TEST_I_H
#define TAO_INTERCEPTOR_TEST_I_H

#include "testS.h"

/**
 * @class Visual_i
 *
 * Implements the Visual interface in test.idl
 */
class Visual_i : public POA_Test_Interceptors::Visual
{
public:
  /// ctor
  explicit Visual_i (CORBA::ORB_ptr orb);
  Visual_i (Visual_i const &) = delete;
  Visual_i (Visual_i &&) = delete;
  Visual_i &operator= (Visual_i const &) = delete;
  Visual_i &operator= (Visual_i &&) = delete;

  void normal (CORBA::Long arg) override;

  CORBA::Long calculate (
                         CORBA::Long one,
                         CORBA::Long two) override;

  void user () override;

  void system () override;

  void shutdown () override;

private:
  /// The ORB pointer (for shutdown.)
  CORBA::ORB_var orb_;
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
