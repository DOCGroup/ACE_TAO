// -*- C++ -*-

//=============================================================================
/**
 *  @file Load_Protocol_Factory_T.h
 *
 *  $Id$
 *
 *  Function templates to load a protocol factory.
 *
 *  @author  Johnny Willemsen
 */
//=============================================================================

#ifndef TAO_LOAD_PROTOCOL_FACTORY_T_H
#define TAO_LOAD_PROTOCOL_FACTORY_T_H

#include /**/ "ace/pre.h"

#include "tao/Protocol_Factory.h"
#include "tao/Resource_Factory.h"
#include "tao/debug.h"
#include "ace/Auto_Ptr.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace details
  {
    template<typename T>
    int
    load_protocol_factory (TAO_ProtocolFactorySet &protocol_set,
                           const char *name)
    {
      TAO_Protocol_Factory *protocol_factory = 0;
      auto_ptr<TAO_Protocol_Factory> safe_protocol_factory;

      TAO_Protocol_Item *item = 0;

      // If a protocol factory is obtained from the Service
      // Configurator then do not transfer ownership to the
      // TAO_Protocol_Item.
      bool transfer_ownership = false;

      protocol_factory =
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (
          ACE_TEXT_CHAR_TO_TCHAR (name));

      if (protocol_factory == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_WARNING,
                        ACE_TEXT("(%P|%t) WARNING - No <%C> found in Service")
                        ACE_TEXT(" Repository. Using default instance.\n"),
                        name));

          ACE_NEW_RETURN (protocol_factory,
                          T,
                          -1);

          ACE_auto_ptr_reset (safe_protocol_factory,
                              protocol_factory);

          transfer_ownership = true;
        }
      else
        {
          transfer_ownership = false;
        }

      ACE_NEW_RETURN (item, TAO_Protocol_Item (name), -1);
      // If the TAO_Protocol_Item retains ownership of the
      // TAO_Protocol_Factory then we used an auto_ptr<> above, so
      // release the TAO_Protocol_Factory from it.  Otherwise, we
      // obtained the TAO_Protocol_Factory from the Service
      // Configurator so an auto_ptr<> wasn't used since the Service
      // Configurator retains ownership, hence there was no need to
      // use an auto_ptr<> in this method.
      item->factory ((transfer_ownership ?
                      safe_protocol_factory.release () :
                      protocol_factory),
                      transfer_ownership);

      if (protocol_set.insert (item) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("TAO (%P|%t) Unable to add ")
                      ACE_TEXT("<%C> to protocol factory set.\n"),
                      item->protocol_name ().c_str ()));

          delete item;

          if (transfer_ownership == false)
            delete protocol_factory;

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Loaded default ")
                      ACE_TEXT("protocol <%C>\n"),
                      name));
        }

      return 0;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LOAD_PROTOCOL_FACTORY_T_H*/
