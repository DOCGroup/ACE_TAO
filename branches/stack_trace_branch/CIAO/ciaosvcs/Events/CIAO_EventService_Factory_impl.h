// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventService_Factory_impl.h
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_EVENTSERVICE_FACTORY_IMPL_H
#define CIAO_EVENTSERVICE_FACTORY_IMPL_H

#include /**/ "ace/pre.h"

#include "CIAO_Events_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciaosvcs/Events/CIAO_Events_Base/CIAO_EventServiceBase.h"

namespace CIAO
{
  class CIAO_EVENTS_Export EventService_Factory_impl
    : public ACE_Service_Object
  {
  public:
    EventService_Factory_impl (void);

    EventService_Factory_impl (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa);

    virtual ~EventService_Factory_impl (void);

    /// A factory method which creates an CIAO_Event_Service object.
    virtual CIAO_Event_Service_ptr create (EventServiceType type,
                                           const char * ec_name);

    virtual void initialize (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

    /// Used to force the initialization.
    static int Initializer (void);

  private:
    /// Reference to the ORB.
    CORBA::ORB_var orb_;

    /// Reference to the Root POA
    PortableServer::POA_var poa_;
  };

  ACE_STATIC_SVC_DECLARE (EventService_Factory_impl)
  ACE_FACTORY_DECLARE (CIAO_EVENTS, EventService_Factory_impl)
}

#if defined (ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*CIAO_Module_Initializer) (void);

static CIAO_Module_Initializer
CIAO_Requires_EventService_Initializer =
  &CIAO::EventService_Factory_impl::Initializer;

#else

static int
CIAO_Requires_EventService_Initializer =
  CIAO::EventService_Factory_impl::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include /**/ "ace/post.h"

#endif /* CIAO_EVENTSERVICE_FACTORY_IMPL_H */
