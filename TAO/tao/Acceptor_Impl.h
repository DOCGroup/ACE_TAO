// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Acceptor_Impl.h
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu>
//   Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ACCEPTOR_IMPL_H
#define TAO_ACCEPTOR_IMPL_H

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class TAO_IIOP_Connector;

template <class SVC_HANDLER>
class TAO_Creation_Strategy : public ACE_Creation_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Creates a Svc_Handler and set the ORB_Core pointer on it.
public:
  TAO_Creation_Strategy (TAO_ORB_Core *orb_core);
  // Constructor.

  int make_svc_handler (SVC_HANDLER *&sh);
  // Create a SVC_HANDLER  and set the ORB_Core pointer on it.

protected:
  TAO_ORB_Core *orb_core_;
  // Pointer to the ORB Core.
};

template <class SVC_HANDLER>
class TAO_Concurrency_Strategy : public ACE_Concurrency_Strategy<SVC_HANDLER>
{
  // = TITLE
  //     Activates the Svc_Handler, and then if specified by the
  //     TAO_Server_Strategy_Factory, it activates the Svc_Handler to
  //     run in its own thread.
public:
  TAO_Concurrency_Strategy (TAO_ORB_Core *orb_core);
  // Constructor.

  int activate_svc_handler (SVC_HANDLER *svc_handler,
                            void *arg);
  // Activates the Svc_Handler, and then if specified by the
  // TAO_Server_Strategy_Factory, it activates the Svc_Handler to run
  // in its own thread.

protected:
  TAO_ORB_Core *orb_core_;
  // Pointer to the ORB Core.
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1, class TAO_PEER_CONNECTOR>
class TAO_Accept_Strategy : public ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
  // = TITLE
  //    This strategy tunes the acceptance of connections. When the
  //    process is out of descriptors needed to accept a connection, it
  //    requests the Connector to remove entries from its conenction
  //    cache. This is done based on the caching strategy specified at
  //    runtime.

public:

  TAO_Accept_Strategy (TAO_ORB_Core *orb_core,
                       const CORBA::ULong &tag);
  // Constructor.

  virtual int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                    int restart = 0);
  // initialize acceptor for this address.

  virtual int accept_svc_handler (SVC_HANDLER *);
  // The default behavior delegates to the <accept> method of the
  // PEER_ACCEPTOR.

protected:

  typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY_BASE;

  TAO_PEER_CONNECTOR *get_connector (void);
  // Obtain the connector needed for access to the connection strategy
  // which removes entries from the connection cache on demand.

  int out_of_sockets_handler (void);
  // Takes care when the process runs out of descriptors.

private:

  TAO_ORB_Core *orb_core_;
  // ORB Core.

  CORBA::ULong tag_;
  // The OMG specified tag for the concrete Acceptor.
};

#if defined(__ACE_INLINE__)
#include "tao/Acceptor_Impl.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Acceptor_Impl.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Acceptor_Impl.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ACCEPTOR_IMPL_H */
