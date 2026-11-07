
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Nanbor Wang
 */
//=============================================================================


#ifndef TAO_INTERCEPTOR_TEST_I_H
#define TAO_INTERCEPTOR_TEST_I_H

#include "testS.h"

/**
 * @class Secure_Vault_i
 *
 * Implements the Secure_Vault interface in test.idl
 */
class Secure_Vault_i : public POA_Test_Interceptors::Secure_Vault
{
public:
  /// ctor
  explicit Secure_Vault_i (CORBA::ORB_ptr orb);

  Secure_Vault_i (Secure_Vault_i const &) = delete;
  Secure_Vault_i (Secure_Vault_i &&) = delete;
  Secure_Vault_i &operator= (Secure_Vault_i const &) = delete;
  Secure_Vault_i &operator= (Secure_Vault_i &&) = delete;

  CORBA::Short ready () override;

  /// Passwd sent in the service context list
  void authenticate (char const *user) override;

  CORBA::Long update_records (CORBA::Long id,
                              Test_Interceptors::Secure_Vault::Record const &val) override;

  void shutdown () override;

 private:
  /// The ORB pointer (for shutdown.)
  CORBA::ORB_var orb_;
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
