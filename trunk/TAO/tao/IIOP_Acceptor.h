// This may look like C, but it's really -*- C++ -*-
// $Id$


//============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    IIOP_Acceptor.h
//
// = DESCRIPTION
//    IIOP specific acceptor processing
//
// = AUTHOR
//    Fred Kuhns <fredk@cs.wustl.edu>
//    Ossama Othman <ossama@uci.edu>
//
//============================================================================

#ifndef TAO_IIOP_ACCEPTOR_H
#define TAO_IIOP_ACCEPTOR_H
#include "ace/pre.h"

#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "tao/GIOP_Message_State.h"

// TAO IIOP_Acceptor concrete call defination

class TAO_Export TAO_IIOP_Acceptor : public TAO_Acceptor
{
  // = TITLE
  //   TAO_IIOP_Acceptor
  //
  // = DESCRIPTION
  //   The IIOP-specific bridge class for the concrete acceptor.
  //
public:
  TAO_IIOP_Acceptor (CORBA::Boolean flag = 0);
  // Constructor.

  ~TAO_IIOP_Acceptor (void);
  // Destructor.

  const ACE_INET_Addr& address (void) const;
  // @@ Helper method for the implementation repository, should go
  //    away

  typedef ACE_Strategy_Acceptor<TAO_IIOP_Connection_Handler, ACE_SOCK_ACCEPTOR> TAO_IIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_IIOP_Connection_Handler> TAO_IIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_IIOP_Connection_Handler> TAO_IIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_IIOP_Connection_Handler, ACE_SOCK_ACCEPTOR> TAO_IIOP_ACCEPT_STRATEGY;

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
  virtual int is_collocated (const TAO_Endpoint *endpoint);
  virtual CORBA::ULong endpoint_count (void);

  virtual int object_key (IOP::TaggedProfile &profile,
                          TAO_ObjectKey &key);

protected:

  virtual int open_i (const ACE_INET_Addr &addr);
  // Implement the common part of the open*() methods.  This method is
  // virtual to allow a derived class implementation to be invoked
  // instead.

  int hostname (TAO_ORB_Core *orb_core,
                ACE_INET_Addr &addr,
                char *&host);
  // Set the host name for the given addr.

  int probe_interfaces (TAO_ORB_Core *orb_core);
  // Probe the system for available network interfaces, and initialize
  // the <addrs_> array with an ACE_INET_Addr for each network
  // interface.  The port for each initialized ACE_INET_Addr will be
  // set in the open_i() method.  This method only gets invoked when
  // no explicit hostname is provided in the specified endpoint.

  virtual int parse_options (const char *options);
  // Parse protocol specific options.

  int init_tcp_properties (void);
  // Obtain tcp properties that must be used by this acceptor, i.e.,
  // initialize <tcp_properties_>.

  int create_rt_mprofile (const TAO_ObjectKey &object_key,
                          TAO_MProfile &mprofile);
  // Rather than creating a separate profile for each endpoint, this
  // version of <create_mprofile> method adds all endpoints to a
  // single IIOP profile, i.e., <mprofile> will not contain more than
  // one IIOP_Profile, no matter how many acceptors there are.

protected:

  /// Array of ACE_INET_Addr instances, each one corresponding to a
  /// given network interface.
  ACE_INET_Addr *addrs_;

  char **hosts_;
  // Cache the information about the endpoints serviced by this
  // acceptor.
  // There may in fact be multiple hostnames for this endpoint. For
  // example, if the IP address is INADDR_ANY (0.0.0.0) then there
  // will be possibly a different hostname for each interface.

  size_t endpoint_count_;
  // The number of host names cached in the hosts_ array (equivalent
  // to the number of endpoints opened by this Acceptor).

  TAO_GIOP_Version version_;
  // The GIOP version for this endpoint
  // @@ Theoretically they shouldn't be here!! We need to look at a
  // way to move this out

  TAO_ORB_Core *orb_core_;
  // ORB Core.

  TAO_IIOP_Properties tcp_properties_;
  // TCP configuration properties to be used for all
  // connections opened by this acceptor.

  CORBA::Boolean lite_flag_;
  // Should we use GIOP lite??

private:

  TAO_IIOP_BASE_ACCEPTOR base_acceptor_;
  // the concrete acceptor, as a pointer to it's base class.

  TAO_IIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_IIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_IIOP_ACCEPT_STRATEGY *accept_strategy_;
  // Acceptor strategies.

};

#if defined(__ACE_INLINE__)
#include "tao/IIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_IIOP_ACCEPTOR_H */
