// @(#) $Id$

#include "Default_Servant_Dispatcher.h"

ACE_RCSID(tao, Default_Acceptor_Filter, "$Id$")

TAO_Default_Servant_Dispatcher::~TAO_Default_Servant_Dispatcher (void)
{
}

void
TAO_Default_Servant_Dispatcher::dispatch (
  TAO_Object_Adapter::Servant_Upcall &servant_upcall,
  TAO_ServerRequest &req,
  CORBA::Environment &ACE_TRY_ENV)
{
  servant_upcall.servant ()->_dispatch (req,
                                        &servant_upcall,
                                        ACE_TRY_ENV);
}
