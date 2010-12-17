// -*- C++ -*-

// ===================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
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

  virtual CORBA::Long calculate (CORBA::Long one,
                                 CORBA::Long two);

  void normal (CORBA::Long arg,
               CORBA::String_out msg);

  virtual Test_Interceptors::Visual::VarLenStruct * the_structure ();

  virtual void user (void);

  virtual void system (void);

  virtual void shutdown (void);

private:

  CORBA::ORB_var orb_;

};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
