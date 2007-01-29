// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 * @file Gateway_i.h
 *
 * $Id$
 *
 * Implementation header used for forwarding the requests from the
 * gateway to the server and reply to the client
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 * @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef GATEWAY_I_H
#define GATEWAY_I_H

#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/DynamicInterface/DII_CORBA_methods.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/PortableServer/PortableServer.h"

class Gateway_i
  : public virtual PortableServer::DynamicImplementation
{
public:
  Gateway_i (CORBA::ORB_ptr orb,
             PortableServer::Current_ptr poa_current);

  virtual void invoke (CORBA::ServerRequest_ptr request);

  virtual CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa);

 private:

  CORBA::ORB_ptr orb_;

  PortableServer::Current_ptr poa_current_;
};

#endif /* GATEWAY_I_H */
