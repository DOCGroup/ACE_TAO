// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    SSLIOP_Acceptor.h
//
// = DESCRIPTION
//   IIOP/SSL specific acceptor processing
//
// = AUTHOR
//    Fred Kuhns
//
// ============================================================================

#ifndef TAO_SSLIOP_ACCEPTOR_H
#define TAO_SSLIOP_ACCEPTOR_H

#include "tao/IIOP_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSL_SOCK_Acceptor.h"
#include "SSLIOP_Connect.h"
#include "SSLIOPC.h"

// TAO SSLIOP_Acceptor concrete call definition

class TAO_Export TAO_SSLIOP_Acceptor : public TAO_IIOP_Acceptor
{
  // = TITLE
  //   TAO_SSLIOP_Acceptor
  //
  // = DESCRIPTION
  //   The IIOP-specific bridge class for the concrete acceptor.
  //
public:
  TAO_SSLIOP_Acceptor (void);
  // Constructor.

  ~TAO_SSLIOP_Acceptor (void);
  // Destructor.

  typedef ACE_Strategy_Acceptor<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR> TAO_SSLIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_SSLIOP_Server_Connection_Handler> TAO_SSLIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_SSLIOP_Server_Connection_Handler> TAO_SSLIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR> TAO_SSLIOP_ACCEPT_STRATEGY;

  // = The TAO_Acceptor methods, check the documentation in
  //   Pluggable.h for details.
  virtual int open (TAO_ORB_Core *orb_core,
                    int version_major,
                    int version_minor,
                    const char *address,
                    const char *options = 0);
  virtual int open_default (TAO_ORB_Core *orb_core,
                            const char *options = 0);
  virtual int close (void);
  virtual int create_mprofile (const TAO_ObjectKey &object_key,
                               TAO_MProfile &mprofile);
  virtual int is_collocated (const TAO_Profile* profile);

private:
  int open_i (TAO_ORB_Core* orb_core,
              const ACE_INET_Addr& addr);
  // Implement the common part of the open*() methods.

  int parse_options (const char *options);
  // Parse protocol specific options.

private:
  TAO_SSLIOP_BASE_ACCEPTOR ssl_acceptor_;
  // The concrete acceptor, as a pointer to it's base class.

  TAO_SSLIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_SSLIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_SSLIOP_ACCEPT_STRATEGY *accept_strategy_;
  // Acceptor strategies.

  SSLIOP::SSL ssl_component_;
  // The SSL component.
};

#if defined(__ACE_INLINE__)
#include "SSLIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_SSLIOP_ACCEPTOR_H */
