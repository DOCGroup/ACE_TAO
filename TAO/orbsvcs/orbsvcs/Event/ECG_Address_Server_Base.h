/* -*- C++ -*- */
/**
 *  @file   ECG_Address_Server_Base.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@atdesk.com)
 *
 */

#ifndef TAO_ECG_ADDRESS_SERVER_BASE_H
#define TAO_ECG_ADDRESS_SERVER_BASE_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/event_export.h"
#include "orbsvcs/RtecUDPAdminS.h"
#include "EC_Lifetime_Utils.h"
#include "ace/Auto_Ptr.h"

/**
 * @class TAO_ECG_Address_Server_Base
 *
 * @brief All implementations of RtecUDPAdmin::AddrServer idl
 * interface should inherit from this abstract base.
 *
 * Adds init () virtual method so that all AddrServer implementations can be
 * initialized in the same fasion.
 */
class TAO_RTEvent_Export TAO_ECG_Address_Server_Base :
  public virtual POA_RtecUDPAdmin::AddrServer,
  public virtual PortableServer::RefCountServantBase
{
public:

  virtual int init (const char *arg) = 0;
};


#if defined(__ACE_INLINE__)
#include "ECG_Address_Server_Base.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ECG_ADDRESS_SERVER_BASE_H */
