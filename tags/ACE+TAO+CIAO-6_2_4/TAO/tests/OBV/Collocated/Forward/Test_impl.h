
//=============================================================================
/**
 *  @file   Test_impl.h
 *
 *  $Id$
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef TAO_TEST_IMPL_H
#define TAO_TEST_IMPL_H

#include "TreeControllerS.h"
#include "TreeBaseS.h"
#include "TreeNodeS.h"

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
  Test_impl (CORBA::ORB_ptr orb);

  virtual TreeController * reflect (
      TreeController * tc);

  virtual void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* TAO_TEST_IMPL_H */
