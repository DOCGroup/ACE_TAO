// -*- C++ -*-
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

#include /**/ "orbsvcs/Event/event_serv_export.h"
#include "orbsvcs/RtecUDPAdminS.h"
#include "orbsvcs/Event/EC_Lifetime_Utils.h"
#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ECG_Address_Server_Base
 *
 * @brief All implementations of RtecUDPAdmin::AddrServer idl
 * interface should inherit from this abstract base.
 *
 * Adds init () virtual method so that all AddrServer implementations can be
 * initialized in the same fasion.
 */
class TAO_RTEvent_Serv_Export TAO_ECG_Address_Server_Base
  : public virtual POA_RtecUDPAdmin::AddrServer
{
public:

  virtual int init (const char *arg) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ECG_ADDRESS_SERVER_BASE_H */
