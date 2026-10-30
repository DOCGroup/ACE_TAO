
//=============================================================================
/**
 *  @file   test_i.h
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
  explicit Simple_Server_i (CORBA::ORB_ptr orb);

  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  // = The Simple_Server methods.
  void sendCharSeq (Char_Seq const &charSeq) override;

  void sendOctetSeq (Octet_Seq const &octetSeq) override;

  CORBA::Long get_number (CORBA::Long num) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_ {};
};

#endif /* TAO_MT_CLIENT_TEST_I_H */
