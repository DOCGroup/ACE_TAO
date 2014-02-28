
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_TIMEOUT_TEST_I_H
#define TAO_TIMEOUT_TEST_I_H

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
  CORBA::Long echo (CORBA::Long x,
                    CORBA::Long msecs);
  void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* TAO_TIMEOUT_TEST_I_H */
