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

#include "orbsvcs/FtRtecEventCommC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class FT_ProxyAdmin_Base
{
public:
  FT_ProxyAdmin_Base(PortableServer::ServantBase* servant,
              PortableServer::POA_var poa);
  virtual ~FT_ProxyAdmin_Base();
  void activate(const FtRtecEventComm::ObjectId& oid);

  const FtRtecEventComm::ObjectId& object_id(void) const;

  /// Returns an CORBA object reference when the servant is activated
  CORBA::Object_var reference(void) const;

protected:

  PortableServer::ServantBase* servant_;
  /// Store the default POA.
  PortableServer::POA_var poa_;
  /// store the object id
  FtRtecEventComm::ObjectId object_id_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif
