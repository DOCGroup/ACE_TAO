// -*- C++ -*-

//=============================================================================
/**
 * @file ObjectReferenceFactory.h
 *
 * $Id$
 *
 * This is the implementation of the
 * PortableInterceptor::ObjectReferenceFactory interface.
 *
 * @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_OBJECT_REFERENCE_FACTORY_H
#define TAO_OBJECT_REFERENCE_FACTORY_H

#include "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Default_ORTC.h"

#include "tao/PortableServer/PortableServerC.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_POA;

/**
 * @class TAO_ObjectReferenceFactory
 *
 * @brief Implementation of the PortableInterceptor::ObjectReferenceFactory
 *        interface.
 */
class TAO_PortableServer_Export TAO_ObjectReferenceFactory
  : public virtual CORBA::DefaultValueRefCountBase
  , public virtual OBV_TAO_Default_ORT::ObjectReferenceFactory
{
 public:

  /// Constructor
  TAO_ObjectReferenceFactory (TAO_POA *poa);

  virtual CORBA::Object_ptr make_object (
      const char * repository_id,
      const PortableInterceptor::ObjectId & id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void set_servant (PortableServer::Servant servant);

  void make_object_invoker (CORBA::ULong identifier);

 private:

  TAO_POA *poa_;

  PortableServer::Servant servant_;

  CORBA::ULong make_object_invoker_;
};

#include "ace/post.h"

#endif /* TAO_OBJECT_REFERENCE_FACTORY_H */
