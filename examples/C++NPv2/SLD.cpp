/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/Log_Msg.h"

#include "Reactor_Logging_Server_Adapter.h"
#include "Logging_Acceptor.h"
#include "SLD_export.h"

typedef Reactor_Logging_Server_Adapter<Logging_Acceptor>
        Server_Logging_Daemon;

ACE_FACTORY_DEFINE (SLD, Server_Logging_Daemon)

