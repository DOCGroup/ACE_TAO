/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/Log_Msg.h"

#include "Reactor_Logging_Server_Adapter.h"
#include "Logging_Acceptor_Ex.h"
#include "SLDEX_export.h"

typedef Reactor_Logging_Server_Adapter<Logging_Acceptor_Ex>
        Server_Logging_Daemon_Ex;

ACE_FACTORY_DEFINE (SLDEX, Server_Logging_Daemon_Ex)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Reactor_Logging_Server_Adapter<Logging_Acceptor_Ex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Reactor_Logging_Server_Adapter<Logging_Acceptor_Ex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

