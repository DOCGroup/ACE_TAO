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

  virtual void normal (CORBA::Long arg
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Long calculate (CORBA::Long one,
                                 CORBA::Long two
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Test_Interceptors::Visual::VarLenStruct * the_structure (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void user (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, Test_Interceptors::Silly));

  virtual void system (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA::ORB_var orb_;

};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
