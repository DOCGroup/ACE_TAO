// -*- C++ -*-

//=============================================================================
/**
 *  @file    Local_Servant_Base.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_LOCAL_SERVANT_BASE_H
#define TAO_LOCAL_SERVANT_BASE_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Local_ServantBase
 *
 * @brief Base class for local servants.
 *
 * This servant does not register with the POA and does not
 * produce a valid stub, i.e., object references of this servant
 * cannot be exported.  The (collocated) stubs of these servants
 * will always be direct, i.e., call directly to the servant and
 * don't call through the POA since this servant is not
 * registered with the POA.
 */
class TAO_PortableServer_Export TAO_Local_ServantBase
  : public virtual TAO_ServantBase
{
protected:
  /// This is an auxiliary method for _this().  Make sure *not* to
  /// register with the default POA.
  TAO_Stub *_create_stub (void);

  /// Throws CORBA::BAD_OPERATION exception.
  void _dispatch (
    TAO_ServerRequest &request,
    TAO::Portable_Server::Servant_Upcall* servant_upcall);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Local_Servant_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_LOCAL_SERVANT_BASE_H */
