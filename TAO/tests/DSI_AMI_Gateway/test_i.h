
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_DSI_GATEWAY_TEST_I_H
#define TAO_DSI_GATEWAY_TEST_I_H

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
  CORBA::Long test_method (CORBA::Long x,
                           Structure const &the_in_structure,
                           Structure_out the_out_structure,
                           char *&name) override;

  CORBA::Long test_val () override;

  void test_val (CORBA::Long test_val) override;

  void raise_user_exception () override;

  void raise_system_exception () override;

  void shutdown () override;

private:
  // The ORB
  CORBA::ORB_var orb_ {};
  CORBA::Long vlong_ {};
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_DSI_GATEWAY_TEST_I_H */
