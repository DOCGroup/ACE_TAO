// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_SSLIOP
//
// = FILENAME
//    SSLIOP_Acceptor.h
//
// = DESCRIPTION
//     IIOP/SSL specific acceptor processing
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_ACCEPTOR_H
#define TAO_SSLIOP_ACCEPTOR_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IIOP_SSL_Acceptor.h"
#include "SSLIOP_Connection_Handler.h"
#include "ace/SSL/SSL_SOCK_Acceptor.h"
#include "orbsvcs/SSLIOPC.h"


// TAO SSLIOP_Acceptor concrete call definition

class TAO_SSLIOP_Export TAO_SSLIOP_Acceptor
  : public TAO_IIOP_SSL_Acceptor
{
  // = TITLE
  //   TAO_SSLIOP_Acceptor
  //
  // = DESCRIPTION
  //   The SSLIOP-specific bridge class for the concrete acceptor.
  //
public:
  TAO_SSLIOP_Acceptor (int support_no_protection);
  // Constructor.

  ~TAO_SSLIOP_Acceptor (void);
  // Destructor.

  typedef ACE_Strategy_Acceptor<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR> TAO_SSLIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_SSLIOP_Connection_Handler> TAO_SSLIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler> TAO_SSLIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR> TAO_SSLIOP_ACCEPT_STRATEGY;

  // = The TAO_Acceptor methods, check the documentation in
  //   Pluggable.h for details.
  virtual int open (TAO_ORB_Core *orb_core,
                    int version_major,
                    int version_minor,
                    const char *address,
                    const char *options = 0);
  virtual int open_default (TAO_ORB_Core *orb_core,
                            int version_major,
                            int version_minor,
                            const char *options = 0);
  virtual int close (void);
  virtual int create_mprofile (const TAO_ObjectKey &object_key,
                               TAO_MProfile &mprofile);

  virtual int create_endpoint_for_mprofile (const TAO_ObjectKey &object_key,
                                            TAO_MProfile &mprofile);
  // Rather than creating a separate profile for each endpoint, this
  // version of <create_mprofile> method adds all endpoints to a
  // single SSLIOP profile, i.e., <mprofile> will not contain more than
  // one SSLIOP_Profile, no matter how many acceptors there are.

  virtual int is_collocated (const TAO_Endpoint* endpoint);

private:
  int ssliop_open_i (TAO_ORB_Core *orb_core, const ACE_INET_Addr& addr);
  // Implement the common part of the open*() methods.

  int parse_options (const char *options);
  // Parse protocol specific options.


  int verify_secure_configuration (TAO_ORB_Core *orb_core,
                                   int major,
                                   int minor);
  // Ensure that neither the endpoint configuration nor the ORB
  // configuration violate security measures.

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

#include "ace/post.h"

#endif  /* TAO_SSLIOP_ACCEPTOR_H */
