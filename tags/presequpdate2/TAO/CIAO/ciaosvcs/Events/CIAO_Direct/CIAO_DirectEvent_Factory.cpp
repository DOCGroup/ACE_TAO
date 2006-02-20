// $Id$

#include "CIAO_DirectEvent_Factory.h"
#include "CIAO_DirectEvent.h"

/// Initialize the DirectEventService object. Will eventually be moved to a separate init ()
/// method so exceptions can be handled properly.

// @@ George, an init () method in the base class sounds like a better option.

namespace CIAO
{

  CIAO_DirectEvent_Factory::CIAO_DirectEvent_Factory (void)
  {
  }

  CIAO_DirectEvent_Factory::~CIAO_DirectEvent_Factory (void)
  {
  }

  Consumer_Config_ptr
  CIAO_DirectEvent_Factory::create_consumer_config ()
  {
    Direct_Consumer_Config_impl * consumer_config = 0;
    ACE_NEW_RETURN (consumer_config,
                    Direct_Consumer_Config_impl (this->root_poa_.in ()),
                    Consumer_Config::_nil ());
    Direct_Consumer_Config_var return_direct =
      consumer_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    return return_direct._retn ();
  }


  Supplier_Config_ptr
  CIAO_DirectEvent_Factory::create_supplier_config ()
  {
    Direct_Supplier_Config_impl * supplier_config = 0;
    ACE_NEW_RETURN (supplier_config,
                    Direct_Supplier_Config_impl (this->root_poa_.in ()),
                    Supplier_Config::_nil ());
    Direct_Supplier_Config_var return_direct =
      supplier_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    return return_direct._retn ();
  }


  EventServiceBase *
  CIAO_DirectEvent_Factory::create ()
  {
    ACE_DEBUG ((LM_DEBUG, "CIAO::EventService_Factory_impl::create_event_service\n"));

    DirectEventService * event_service = 0;
    ACE_NEW_RETURN (event_service,
                    DirectEventService (this->orb_.in (),
                                        this->root_poa_.in ()),
                    0);
    return event_service;
  }

  int
  CIAO_DirectEvent_Factory::Initializer (void)
  {
    return
      ACE_Service_Config::process_directive (
          ace_svc_desc_CIAO_DirectEvent_Factory
        );
  }

  void CIAO_DirectEvent_Factory::init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
  {
    this->orb_ = CORBA::ORB::_duplicate (orb);
    this->root_poa_ = PortableServer::POA::_duplicate (poa);
  }

  ACE_STATIC_SVC_DEFINE (
      CIAO_DirectEvent_Factory,
      ACE_TEXT ("CIAO_DirectEvent_Factory"),
      ACE_SVC_OBJ_T,
      &ACE_SVC_NAME (CIAO_DirectEvent_Factory),
      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
      0
    )

  ACE_FACTORY_DEFINE (CIAO_EVENTS, CIAO_DirectEvent_Factory)

} // namespace CIAO
