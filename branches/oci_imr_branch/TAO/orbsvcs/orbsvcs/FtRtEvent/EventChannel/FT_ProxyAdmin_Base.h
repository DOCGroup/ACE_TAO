// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_ProxyAdmin_Base.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef FT_PROXYADMIN_BASE_H
#define FT_PROXYADMIN_BASE_H

#include /**/ "ace/pre.h"
#include "tao/corba.h"
#include "tao/PortableServer/Servant_Base.h"
#include "orbsvcs/orbsvcs/FtRtecEventCommC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class FT_ProxyAdmin_Base
{
public:
  FT_ProxyAdmin_Base(PortableServer::ServantBase* servant,
              PortableServer::POA_var poa);
  virtual ~FT_ProxyAdmin_Base();
  void activate(const FtRtecEventComm::ObjectId& oid
              ACE_ENV_ARG_DECL);

  const FtRtecEventComm::ObjectId& object_id(ACE_ENV_SINGLE_ARG_DECL) const;

  /// Returns an CORBA object reference when the servant is activated
  CORBA::Object_var reference(ACE_ENV_SINGLE_ARG_DECL) const;

protected:

  PortableServer::ServantBase* servant_;
  /// Store the default POA.
  PortableServer::POA_var poa_;
  /// store the object id
  FtRtecEventComm::ObjectId object_id_;
};



#include /**/ "ace/post.h"
#endif
