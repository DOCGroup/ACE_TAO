
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_MT_SERVER_TEST_I_H
#define TAO_MT_SERVER_TEST_I_H

#include "testS.h"

/**
 * @class Simple_Server_i
 *
 * @brief Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Simple_Server_i : public POA_Simple_Server
{
public:
  /// ctor
  explicit Simple_Server_i (CORBA::ORB_ptr orb);
  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  void ping () override;

  void validate_protocol () override;

  CORBA::Long test_method (CORBA::Long x) override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_;

  /// Synchronizer
  TAO_SYNCH_MUTEX mutex_;

  enum
    {
      NOT_VALIDATED = 0,
      VALIDATED_SUCCESS,
      VALIDATED_NOSUCCESS
    };
  /// Flag to indicate status of validation
  CORBA::Octet validated_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_SERVER_TEST_I_H */
