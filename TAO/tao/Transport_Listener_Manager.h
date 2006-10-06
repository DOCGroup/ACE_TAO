// -*- C++ -*-

//=============================================================================
/**
 *  @file Transport_Current_Listener.h
 *
 *  $Id$
 *
 *  @author  Iliyan Jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================

#ifndef TAO_TRANSPORT_CURRENT_LISTENER_H
#define TAO_TRANSPORT_CURRENT_LISTENER_H

#include /**/ "ace/pre.h"
#include "ace/Unbounded_Set.h"
#include "ace/Service_Object.h"
#include "tao/TAO_Export.h"
#include "tao/Versioned_Namespace.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;
class TAO_ORB_Core;

namespace TAO {

  namespace Transport {

    class TAO_Export Listener
    {
    public:
      virtual ~Listener (void);
      virtual int notify (const TAO_Transport*) = 0;
    };

    class TAO_Export Listener_Manager : public ACE_Service_Object
    {
    public:
      virtual ~Listener_Manager (void);

    public:
      int notify (const TAO_Transport*);
      int insert (Listener*);
      int remove (Listener*);

    private:
      typedef ACE_Unbounded_Set<Listener*> Listener_Collection;

      Listener_Collection collection_;
    };
  };
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Transport_Listener_Manager)
ACE_FACTORY_DECLARE (TAO, TAO_Transport_Listener_Manager)

/* #if defined (__ACE_INLINE__) */
/* # include "Transport_Current_Listener.inl" */
/* #endif /\* __ACE_INLINE__ *\/ */

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_CURRENT_LISTENER_H */
