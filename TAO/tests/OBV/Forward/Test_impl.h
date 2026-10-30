
//=============================================================================
/**
 *  @file   Test_impl.h
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef TAO_TEST_IMPL_H
#define TAO_TEST_IMPL_H

#include "TreeControllerS.h"

/**
 * @class Test_impl
 *
 * @brief Implementation of Test interface
 *
 * This interface is provided to produce valuetypes and test
 * marshaling
 */
class Test_impl : public POA_Test
{
public:
  /// ctor
  explicit Test_impl (CORBA::ORB_ptr orb);

  Test_impl (Test_impl const&) = delete;
  Test_impl (Test_impl&&) = delete;
  Test_impl& operator= (Test_impl const&) = delete;
  Test_impl& operator= (Test_impl&&) = delete;

  TreeController *reflect (TreeController *tc) override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* TAO_TEST_IMPL_H */
