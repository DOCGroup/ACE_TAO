// -*- C++ -*-

//=============================================================================
/**
 * @file client_i.h
 *
 * $Id$ *
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
  : public virtual POA_client,
    public virtual PortableServer::RefCountServantBase
{
public:

  client_i (server_ptr s);
  virtual ~client_i (void);

  virtual void test_reply (const char *msg
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  server_var server_;

};

#endif  /* CLIENT_I_H */
