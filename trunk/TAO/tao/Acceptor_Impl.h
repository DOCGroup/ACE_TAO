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

template<class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> class TAO_Acceptor_Impl : public ACE_Acceptor<SVC_HANDLER,ACE_PEER_ACCEPTOR_2>
{
  // = TITLE
  //   Helper class to implement the acceptors in TAO
  //
  // = DESCRIPTION
  //   TAO pluggable protocols framework provide an abstraction to
  //   represent any kind of acceptor object, the implementation of
  //   that acceptor is left for the pluggable protocol implementor,
  //   but the most common case would be to use an ACE_Acceptor<>
  //   instantiated over the right Svc_Handlers.
  //   But the Svc_Handlers must inherit the <orb_core> that owns the
  //   acceptor, though this could be implemented in each pluggable
  //   protocol we believe that this class would simplify that task
  //   and work in most cases.  Pluggable protocol implementors are,
  //   of course, free to use something else.
  //
public:
  // = Initialization and termination methods.
  TAO_Acceptor_Impl (ACE_Reactor * = 0,
                     int use_select = 1);
  // Constructor.

  TAO_Acceptor_Impl (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                     ACE_Reactor * = ACE_Reactor::instance (),
                     int flags = 0,
                     int use_select = 1,
                     int reuse_addr = 1);
  // The constructors, just delegate to the base class.

  int open (TAO_ORB_Core* orb_core,
            const ACE_PEER_ACCEPTOR_ADDR &,
            int flags = 0,
            int use_select = 1,
            int reuse_addr = 1);
  // Initialize the ORB_Core.

protected:
  // = See $ACE_ROOT/ace/Acceptor.h for the documentation.
  virtual int make_svc_handler (SVC_HANDLER *&sh);
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);

private:
  TAO_ORB_Core *orb_core_;
  // Pointer to the ORB Core.
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
