// -*- C++ -*-

//=============================================================================
/**
 *  @file    CIAO_Events.cpp
 *
 *  $Id$
 *
 *  Method definitions for CIAO's event classes
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

::Components::Cookie * CIAO::CIAO_RTEventService::specify_event_service (
    const char * event_name,
    const char * publisher_name,
    const char * service_name
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
}

::Components::Cookie * CIAO::CIAO_RTEventService::connect_event_supplier (
    CIAO_EventServiceInfo service_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
}

::Components::Cookie * CIAO::CIAO_RTEventService::connect_event_consumer (
    ::Components::EventConsumerBase_ptr c,
    CIAO_EventServiceInfo service_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
}

void CIAO::CIAO_RTEventService::disconnect_event_consumer (
    ::Components::Cookie *ck
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::InvalidName,
    ::Components::InvalidConnection))
{
}

void CIAO::CIAO_RTEventService::disconnect_event_supplier (
    ::Components::Cookie *ck
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::InvalidName,
    ::Components::InvalidConnection))
{
}

void CIAO::CIAO_RTEventService::push_event (
    ::Components::EventBase *ev,
    ::Components::Cookie *ck
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
}
