// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Portable_Interceptors/Benchmark
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Nanbor Wang
//
// ============================================================================

#ifndef TAO_INTERCEPTOR_TEST_I_H
#define TAO_INTERCEPTOR_TEST_I_H

#include "testS.h"

class Secure_Vault_i : public POA_Test_Interceptors::Secure_Vault
{
  // = DESCRIPTION
  //   Implements the Secure_Vault interface in test.idl

public:
  Secure_Vault_i (CORBA::ORB_ptr orb);
  // ctor
  
  virtual CORBA::Short ready (CORBA::Environment &ACE_TRY_ENV = 
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void authenticate (const char * user,
                             CORBA::Environment &ACE_TRY_ENV = 
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Test_Interceptors::Invalid));
  // Passwd sent in the service context list

  virtual CORBA::Long update_records (CORBA::Long id,
                                      const Test_Interceptors::Secure_Vault::Record & val,
                                      CORBA::Environment &ACE_TRY_ENV = 
                                      TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV = 
                         TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));
  
 private:
  CORBA::ORB_var orb_;
  // The ORB pointer (for shutdown.)
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
