// -*- C++ -*-

//=============================================================================
/**
 * @file server_i.h
 *
 * $Id$
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
  : public virtual POA_server,
    public virtual PortableServer::RefCountServantBase
{
public:

  server_i (CORBA::ORB_ptr orb);
  virtual ~server_i (void);

  virtual void set_client (client_ptr c
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void test_request (const char *msg
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA::ORB_var orb_;

  client_var client_;

};

#endif  /* SERVER_I_H */
