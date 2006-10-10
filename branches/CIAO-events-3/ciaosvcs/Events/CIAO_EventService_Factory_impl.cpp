// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventService_Factory_impl.cpp
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#include "CIAO_EventService_Factory_impl.h"
#include "ciaosvcs/Events/CIAO_RTEC/CIAO_RTEvent.h"

namespace CIAO
{
  EventService_Factory_impl::EventService_Factory_impl (void)
  {
  }

  EventService_Factory_impl::EventService_Factory_impl (
        CORBA::ORB_ptr orb,
        PortableServer::POA_ptr poa) :
    orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
  {
  }

  EventService_Factory_impl::~EventService_Factory_impl (void)
  {
  }

  CIAO_Event_Service_ptr
  EventService_Factory_impl::create (EventServiceType type)
  {
    ACE_DEBUG ((LM_DEBUG, "CIAO::EventService_Factory_impl::create_event_service\n"));

    EventServiceBase * event_service = 0;

    switch (type)
    {
      case RTEC:
        ACE_NEW_RETURN (event_service,
                        RTEventService (this->orb_.in (),
                                        this->poa_.in ()),
                        0);
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR, "CIAO::EventService_Factory_impl::"
                          "create_event_service: unsuppoted type.\n"),
                          0);

    }

    // Activate the servant
    CIAO_Event_Service_var service = event_service->_this ();
    return service._retn ();
  }

  int
  EventService_Factory_impl::Initializer (void)
  {
    return
      ACE_Service_Config::process_directive (
          ace_svc_desc_EventService_Factory_impl
        );
  }

  void EventService_Factory_impl::initialize (
    CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
  {
    this->orb_ = CORBA::ORB::_duplicate (orb);
    this->poa_ = PortableServer::POA::_duplicate (poa);
  }

ACE_STATIC_SVC_DEFINE (
    EventService_Factory_impl,
    ACE_TEXT ("CIAO_EventService_Factory"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (EventService_Factory_impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (CIAO_EVENTS, EventService_Factory_impl)

} // namespace CIAO
