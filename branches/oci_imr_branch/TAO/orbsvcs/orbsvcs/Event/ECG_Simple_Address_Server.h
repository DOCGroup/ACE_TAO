/* -*- C++ -*- */
/**
 *  @file ECG_Simple_Address_Server.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@atdesk.com)
 *
 */

#ifndef TAO_ECG_SIMPLE_ADDRESS_SERVER_H
#define TAO_ECG_SIMPLE_ADDRESS_SERVER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecUDPAdminS.h"
#include "EC_Lifetime_Utils_T.h"
#include "ace/INET_Addr.h"

/**
 * @class TAO_ECG_Simple_Address_Server
 *
 * @brief Basic implementation of RtecUDPAdmin idl interface.
 *
 * This simple server always returns multicast address it was
 * initialized with, regardless of the header.
 */
class TAO_RTEvent_Export TAO_ECG_Simple_Address_Server :
  public virtual POA_RtecUDPAdmin::AddrServer,
  public virtual PortableServer::RefCountServantBase
{
public:

  /// Create a new TAO_ECG_Simple_Address_Server object.
  /// (Constructor access is restricted to insure that all
  /// TAO_ECG_Simple_Address_Server objects are heap-allocated.)
  static TAO_EC_Servant_Var<TAO_ECG_Simple_Address_Server> create (void);

  /// Destructor
  virtual ~TAO_ECG_Simple_Address_Server (void);

  int init (const char *arg);

  // = The RtecUDPAdmin::AddrServer methods
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr_out addr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Constructor (protected).  Clients can create new
  /// TAO_ECG_Simple_Address_Server objects using the static create()
  /// method.
  TAO_ECG_Simple_Address_Server (void);

private:
  ACE_INET_Addr addr_;
};

#if defined(__ACE_INLINE__)
#include "ECG_Simple_Address_Server.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ECG_SIMPLE_ADDRESS_SERVER_H */
