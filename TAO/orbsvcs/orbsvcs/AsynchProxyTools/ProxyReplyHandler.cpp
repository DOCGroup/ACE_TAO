// -*- C++ -*-

//
// $Id$
//

#include "orbsvcs/AsynchProxyTools/ProxyReplyHandler.h"

TAO_ProxyReplyHandler::TAO_ProxyReplyHandler ()
{
}

TAO_ProxyReplyHandler::~TAO_ProxyReplyHandler ()
{
}


  // Callback method for deferred synchronous requests.
void
TAO_ProxyReplyHandler::handle_response (TAO_InputCDR &incoming)
{
}

void
TAO_ProxyReplyHandler::handle_excep (TAO_InputCDR &incoming,
                                     CORBA::ULong reply_status)
{
}
