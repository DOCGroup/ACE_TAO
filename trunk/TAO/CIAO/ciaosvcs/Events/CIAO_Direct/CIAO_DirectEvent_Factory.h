// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventService_Factory_impl.h
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_EVENTSERVICE_FACTORY_IMPL_H
#define CIAO_EVENTSERVICE_FACTORY_IMPL_H
#include /**/ "ace/pre.h"

#include "CIAO_EventService_Factory_Base.h"

#include "tao/PortableServer/PortableServer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{

  class CIAO_EVENTS_Export CIAO_DirectEvent_Factory
    : public virtual EventService_Factory
  {

  public:
    // @@ George, why don't you initialize with the POA pointer. Just
    // curious.
    CIAO_DirectEvent_Factory (void);

    virtual ~CIAO_DirectEvent_Factory (void);

    virtual Consumer_Config_ptr create_consumer_config (void);

    virtual Supplier_Config_ptr create_supplier_config (void);

    virtual EventServiceBase * create (void);

    virtual void init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

    // Used to force the initialization.
    static int Initializer (void);

  private:

    /// Reference to the ORB
    CORBA::ORB_var orb_;

    /// Reference to the Root POA
    PortableServer::POA_var root_poa_;
  };

ACE_STATIC_SVC_DECLARE (CIAO_DirectEvent_Factory)
ACE_FACTORY_DECLARE (CIAO_EVENTS, CIAO_DirectEvent_Factory)

}

#if defined (ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*CIAO_Module_Initializer) (void);

static CIAO_Module_Initializer
CIAO_Requires_EventService_Initializer =
  &CIAO::CIAO_DirectEvent_Factory::Initializer;

#else

static int
CIAO_Requires_EventService_Initializer =
  CIAO::CIAO_DirectEvent_Factory::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include /**/ "ace/post.h"
#endif /* CIAO_EVENTSERVICE_FACTORY_IMPL_H */
