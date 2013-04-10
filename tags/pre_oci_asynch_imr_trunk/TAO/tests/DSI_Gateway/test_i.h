
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
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
  Simple_Server_i (CORBA::ORB_ptr orb);

  // = The Simple_Server methods.
  CORBA::Long test_method (CORBA::Long x,
                           const Structure& the_in_structure,
                           Structure_out the_out_structure,
                           char *&name);

   CORBA::Long test_val (void);

   void test_val (CORBA::Long test_val);

  void raise_user_exception (void);

  void raise_system_exception (void);

  void shutdown (void);

private:
  // The ORB
  CORBA::ORB_var orb_;
  CORBA::Long  vlong_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_DSI_GATEWAY_TEST_I_H */
