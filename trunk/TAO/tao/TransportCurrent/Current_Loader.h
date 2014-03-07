/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Current_Loader.h
 *
 *  $Id$
 *
 *  @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================


#ifndef TAO_TRANSPORT_CURRENT_LOADER_H
#define TAO_TRANSPORT_CURRENT_LOADER_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "tao/Versioned_Namespace.h"

#if TAO_HAS_TRANSPORT_CURRENT == 1

#include "tao/TransportCurrent/Transport_Current_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Transport
  {
    class TAO_Transport_Current_Export Current_Loader
      : public ACE_Service_Object
    {
    public:
      /// Initializes object when dynamic linking occurs.
      virtual int init (int argc, ACE_TCHAR *argv[]);

    protected:
      /// Protected destructor to enforce reference counting discipline
      virtual ~Current_Loader (void);
    };

  }
}


#if defined (TAO_AS_STATIC_LIBS)
namespace TAO
{
  namespace Transport
  {
    int current_static_initializer (void);
  }
}

#endif /* defined (TAO_AS_STATIC_LIBS) */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Transport_Current,
                               TAO_Transport_Current_Loader)

ACE_FACTORY_DECLARE (TAO_Transport_Current,
                     TAO_Transport_Current_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_CURRENT_LOADER_H */
