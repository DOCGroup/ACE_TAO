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
                                 CORBA::Long two)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void normal (CORBA::Long arg,
               CORBA::String_out msg)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Test_Interceptors::Visual::VarLenStruct * the_structure ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void user (void)
    ACE_THROW_SPEC ((CORBA::SystemException, Test_Interceptors::Silly));

  virtual void system (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA::ORB_var orb_;

};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
