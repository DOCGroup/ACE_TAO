// -*- C++ -*-

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

  Visual_i (CORBA::ORB_ptr orb);

  void normal (CORBA::Long arg,
               CORBA::Environment &ACE_TRY_ENV =
                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void nothing (CORBA::Environment &ACE_TRY_ENV =
                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void user (CORBA::Environment &ACE_TRY_ENV =
               TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Test_Interceptors::Silly));

  void system (CORBA::Environment &ACE_TRY_ENV =
                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &ACE_TRY_ENV =
                   TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// The ORB pseudo-reference (for shutdown).
  CORBA::ORB_var orb_;

};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
