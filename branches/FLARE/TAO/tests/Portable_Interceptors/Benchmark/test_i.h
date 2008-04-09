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

  virtual CORBA::Short ready (void);

  virtual void authenticate (const char * user);
  // Passwd sent in the service context list

  virtual CORBA::Long update_records (CORBA::Long id,
                                      const Test_Interceptors::Secure_Vault::Record & val);

  virtual void shutdown (void);

 private:
  CORBA::ORB_var orb_;
  // The ORB pointer (for shutdown.)
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
