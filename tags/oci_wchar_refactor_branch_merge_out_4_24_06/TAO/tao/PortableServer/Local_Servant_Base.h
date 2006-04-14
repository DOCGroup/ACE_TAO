// This may look like C, but it's really -*- C++ -*-

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
  /// This is an auxiliar method for _this().  Make sure *not* to
  /// register with the default POA.
  TAO_Stub *_create_stub (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Throws CORBA::BAD_OPERATION exception.
  void _dispatch (TAO_ServerRequest &request,
                  void *servant_upcall
                  ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
# include "Local_Servant_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_LOCAL_SERVANT_BASE_H */
