// -*- C++ -*-

//=============================================================================
/**
 *  @file    ImR_Client_Adapter.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_IMR_CLIENT_ADAPTER_H
#define TAO_IMR_CLIENT_ADAPTER_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Root_POA;

namespace CORBA
{
  class Object;
  typedef Object* Object_ptr;
}

namespace TAO
{
  class ObjectKey;

  namespace Portable_Server
  {
    /**
     * @class ImR_Client_Adapter
     *
     * @brief IFR_Client_Adapter.
     *
     * Class that adapts various functions in the PortableServer library
     * which use the Implementation Repository. This is a base class for
     * the actual implementation in the TAO_IMR_Client library.
     */
    class TAO_PortableServer_Export ImR_Client_Adapter :
      public ACE_Service_Object
    {
    public:
      virtual ~ImR_Client_Adapter (void);

      /// ImplRepo helper method, notify the ImplRepo on startup
      virtual void imr_notify_startup (TAO_Root_POA* poa ) = 0;

      /// ImplRepo helper method, notify the ImplRepo on shutdown
      virtual void imr_notify_shutdown (TAO_Root_POA* poa ) = 0;

      /// ImplRepo helper method, create an IMR-ified object for a
      /// key with a given type
      virtual CORBA::Object_ptr imr_key_to_object(
        TAO_Root_POA* poa,
        const TAO::ObjectKey &key,
        const char *type_id) const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_IMR_CLIENT_ADAPTER_H */
