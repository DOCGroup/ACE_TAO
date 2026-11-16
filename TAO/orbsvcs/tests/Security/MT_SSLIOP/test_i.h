
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
  explicit Simple_Server_i (CORBA::ORB_ptr orb);
  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  /// @name Simple_Server operations
  //@{
  CORBA::Long test_method (CORBA::Long x) override;

  void shutdown () override;
  //@}

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

/**
 * @class Another_One_i
 *
 * @brief Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Another_One_i : public POA_Another_One
{
public:
  explicit Another_One_i (CORBA::ORB_ptr orb);
  Another_One_i (Another_One_i const &) = delete;
  Another_One_i (Another_One_i &&) = delete;
  Another_One_i &operator= (Another_One_i const &) = delete;
  Another_One_i &operator= (Another_One_i &&) = delete;

  /// @name Simple_Server operations
  //@{
  CORBA::Long test_method (CORBA::Long x) override;
  //@}

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_SERVER_TEST_I_H */
