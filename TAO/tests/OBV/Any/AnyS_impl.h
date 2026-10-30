
//=============================================================================
/**
 *  @file   AnyS_impl.h
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef TAO_ANY_S_IMPL_H
#define TAO_ANY_S_IMPL_H

#include "AnyS.h"

/**
 * @class Test_impl
 *
 * @brief Implementation of Test interface
 *
 * This interface is provided to produce valuetypes and test
 * marshaling
 */
class Test_impl : public POA_OBV_AnyTest::Test
{
public:
  /// ctor
  explicit Test_impl (CORBA::ORB_ptr orb);

  Test_impl (Test_impl const&) = delete;
  Test_impl (Test_impl&&) = delete;
  Test_impl& operator= (Test_impl const&) = delete;
  Test_impl& operator= (Test_impl&&) = delete;

  CORBA::Any *get_something (CORBA::Boolean need_vb) override;

  OBV_AnyTest::VB *get_vb () override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* TAO_ANY_S_IMPL_H */
