// -*- C++ -*-

//=============================================================================
/**
 * @file server_i.h
 *
 * Header for the SSLIOP Callback test server implementation.
 *
 * @author Robert Martin <martin_r@ociweb.com>
 */
//=============================================================================

#ifndef SERVER_I_H
#define SERVER_I_H

#include "serverS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class server_i
  : public virtual POA_server
{
public:
  explicit server_i (CORBA::ORB_ptr orb);
  ~server_i () override = default;

  server_i (server_i const &) = delete;
  server_i (server_i &&) = delete;
  server_i &operator= (server_i const &) = delete;
  server_i &operator= (server_i &&) = delete;

  void set_client (client_ptr c) override;

  void test_request (char const *msg) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;

  client_var client_ {};
};

#endif  /* SERVER_I_H */
