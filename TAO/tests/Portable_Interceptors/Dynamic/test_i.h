// -*- C++ -*-

// ===================================================================
/**
 *  @file   test_i.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_INTERCEPTOR_TEST_I_H
#define TAO_INTERCEPTOR_TEST_I_H

#include "testS.h"

class Visual_i : public POA_Test_Interceptors::Visual
{
public:
  Visual_i (CORBA::ORB_ptr orb);

  CORBA::Long calculate (CORBA::Long one, CORBA::Long two) override;

  void normal (CORBA::Long arg, CORBA::String_out msg)  override;

  Test_Interceptors::Visual::VarLenStruct * the_structure () override;

  void user () override;

  void system () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
