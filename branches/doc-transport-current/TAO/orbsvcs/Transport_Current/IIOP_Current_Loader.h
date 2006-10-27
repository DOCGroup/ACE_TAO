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

#include "orbsvcs/Transport_Current/Current_Loader.h"

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


TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Transport_Current,
                               TAO_Transport_IIOP_Current_Loader)

ACE_FACTORY_DECLARE (TAO_Transport_Current,
                     TAO_Transport_IIOP_Current_Loader)

#endif /* TAO_TRANSPORT_IIOP_TRAITS_LOADER_H */
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

#include "orbsvcs/Transport_Current/Current_Loader.h"

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


TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Transport_Current,
                               TAO_Transport_IIOP_Current_Loader)

ACE_FACTORY_DECLARE (TAO_Transport_Current,
                     TAO_Transport_IIOP_Current_Loader)

#endif /* TAO_TRANSPORT_IIOP_TRAITS_LOADER_H */
