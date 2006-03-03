// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Faults
//
// = FILENAME
//   ping_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_PING_I_H
#define TAO_PING_I_H

#include "pingS.h"

class PingObject_i : public POA_PingObject
{
  // = TITLE
  //   A simple implementation of the <ping> interface
  //
  // = DESCRIPTION
  //   Used to verify that the client can tolerate server crashes with
  //   oneway invocations and that there are no problems with
  //   persistent object references after the crash.
  //
public:
  PingObject_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr the_poa);
  // ctor

  // = The PingObject interface methods
  void ping (PingObject_ptr callback
             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void pong (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The orb

  PortableServer::POA_var poa_;
  // The poa
};

#if defined(__ACE_INLINE__)
#include "ping_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PING_I_H */
