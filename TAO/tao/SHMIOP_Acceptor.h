// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    SHMIOP_Acceptor.h
//
// = DESCRIPTION
//    SHMIOP specific acceptor processing
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#ifndef TAO_SHMIOP_ACCEPTOR_H
#define TAO_SHMIOP_ACCEPTOR_H

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Pluggable.h"
#include "tao/SHMIOP_Connect.h"
#include "tao/Acceptor_Impl.h"
#include "ace/Acceptor.h"
#include "ace/MEM_Acceptor.h"

// TAO SHMIOP_Acceptor concrete call defination

class TAO_Export TAO_SHMIOP_Acceptor : public TAO_Acceptor
{
  // = TITLE
  //   TAO_SHMIOP_Acceptor
  //
  // = DESCRIPTION
  //   The IIOP-specific bridge class for the concrete acceptor.
  //
public:
  TAO_SHMIOP_Acceptor (void);
  // Constructor.

  ~TAO_SHMIOP_Acceptor (void);
  // Destructor.

  const ACE_INET_Addr& address (void) const;
  // @@ Helper method for the implementation repository, should go
  //    away

  typedef ACE_Strategy_Acceptor<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR> TAO_SHMIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_SHMIOP_Server_Connection_Handler> TAO_SHMIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_SHMIOP_Server_Connection_Handler> TAO_SHMIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_SHMIOP_Server_Connection_Handler, ACE_MEM_ACCEPTOR> TAO_SHMIOP_ACCEPT_STRATEGY;

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
  virtual CORBA::ULong endpoint_count (void);

private:
  int open_i (TAO_ORB_Core* orb_core,
              const ACE_INET_Addr& addr);
  // Implement the common part of the open*() methods.

  virtual int parse_options (const char *options);
  // Parse protocol specific options.

protected:
  ACE_INET_Addr address_;
  ACE_CString host_;
  // Cache the information about the endpoint serviced by this
  // acceptor.
  // @@ TODO there may in fact be multiple hostnames for this
  //    endpoint. For example it the IP address is INADDR_ANY
  //    (0.0.0.0) then there will be possibly a different hostname for
  //    each interface.

  TAO_GIOP_Version version_;
  // The GIOP version for this endpoint

  TAO_ORB_Core *orb_core_;
  // ORB Core.

private:
  TAO_SHMIOP_BASE_ACCEPTOR base_acceptor_;
  // the concrete acceptor, as a pointer to it's base class.

  TAO_SHMIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_SHMIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_SHMIOP_ACCEPT_STRATEGY *accept_strategy_;
  // Acceptor strategies.

};

#if defined(__ACE_INLINE__)
#include "tao/SHMIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#endif  /* TAO_SHMIOP_ACCEPTOR_H */
