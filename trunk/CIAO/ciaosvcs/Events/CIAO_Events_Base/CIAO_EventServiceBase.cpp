//=============================================================================
/**
 *  @file CIAO_EventServiceBase.cpp
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#include "CIAO_EventServiceBase.h"

namespace CIAO
{
  EventServiceBase::EventServiceBase (void)
  {
  }

  EventServiceBase::~EventServiceBase (void)
  {
  }

  void
  EventServiceBase::ciao_push_event (Components::EventBase * /* evt */,
                                     const char * /* source_id */,
                                     CORBA::TypeCode_ptr /* tc */)
  {
  }
}
