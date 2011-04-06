
//=============================================================================
/**
 *  @file   AnyS_impl.h
 *
 *  $Id$
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
  Test_impl (CORBA::ORB_ptr orb);

  virtual CORBA::Any * get_something (
      CORBA::Boolean need_vb);

  virtual OBV_AnyTest::VB * get_vb (void);

  virtual void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;
  CORBA::Boolean even_;
};

#endif /* TAO_ANY_S_IMPL_H */
