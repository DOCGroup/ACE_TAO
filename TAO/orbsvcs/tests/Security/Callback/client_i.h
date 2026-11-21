// -*- C++ -*-

//=============================================================================
/**
 * @file client_i.h
 *
 *  *
 * Header for the SSLIOP Callback test client callback implementation.
 *
 * @author Robert Martin <martin_r@ociweb.com>
 */
//=============================================================================

#ifndef CLIENT_I_H
#define CLIENT_I_H

#include "clientS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "serverC.h"

class client_i
  : public virtual POA_client
{
public:
  explicit client_i (server_ptr s);
  ~client_i () override = default;

  client_i (client_i const &) = delete;
  client_i (client_i &&) = delete;
  client_i &operator= (client_i const &) = delete;
  client_i &operator= (client_i &&) = delete;

  void test_reply (char const *msg) override;

private:
  server_var server_;
};

#endif  /* CLIENT_I_H */
