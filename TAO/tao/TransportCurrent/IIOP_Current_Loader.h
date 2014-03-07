/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    IIOP_Current_Loader.h
 *
 *  $Id$
 *
 *  @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================

#ifndef TAO_TRANSPORT_IIOP_TRAITS_LOADER_H
#define TAO_TRANSPORT_IIOP_TRAITS_LOADER_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_TRANSPORT_CURRENT == 1

#include "tao/TransportCurrent/Current_Loader.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Transport
  {

    namespace IIOP
    {
      class TAO_Transport_Current_Export Current_Loader
        : public TAO::Transport::Current_Loader
      {
      public:
        /// Initializes object when dynamic linking occurs.
        virtual int init (int argc, ACE_TCHAR *argv[]);
        virtual ~Current_Loader ();
        Current_Loader ();
      };
    }
  }
}

#if defined (TAO_AS_STATIC_LIBS)
namespace TAO
{
  namespace Transport
  {
    namespace IIOP
    {
      int current_static_initializer (void);
    }
  }
}

#endif /* defined (TAO_AS_STATIC_LIBS) */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Transport_Current,
                               TAO_Transport_IIOP_Current_Loader)

ACE_FACTORY_DECLARE (TAO_Transport_Current,
                     TAO_Transport_IIOP_Current_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

#endif /* TAO_TRANSPORT_IIOP_TRAITS_LOADER_H */
