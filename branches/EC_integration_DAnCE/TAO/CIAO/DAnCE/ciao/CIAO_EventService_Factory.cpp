// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventService_Factory.cpp
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#include "CIAO_EventService_Factory.h"
#include "ace/Dynamic_Service.h"

CIAO::EventService_Factory::~EventService_Factory (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<EventService_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<EventService_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
