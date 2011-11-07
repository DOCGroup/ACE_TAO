
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
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
  Visual_i (CORBA::ORB_ptr orb);

  void normal (CORBA::Long arg);

  CORBA::Long calculate (
                         CORBA::Long one,
                         CORBA::Long two);

  void user (void);

  void system (void);

  void shutdown (void);

private:
  /// The ORB pointer (for shutdown.)
  CORBA::ORB_var orb_;
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
