
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_MT_CLIENT_TEST_I_H
#define TAO_MT_CLIENT_TEST_I_H

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
  void sendCharSeq (const Char_Seq & charSeq);

  void sendOctetSeq (const Octet_Seq & octetSeq);

  CORBA::Long get_number (CORBA::Long num);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
};

#endif /* TAO_MT_CLIENT_TEST_I_H */
