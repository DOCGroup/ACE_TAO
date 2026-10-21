/*
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/Log_Msg.h"

#include "Reactor_Logging_Server_Adapter.h"
#include "Logging_Acceptor_Ex.h"
#include "SLDEX_export.h"

using Server_Logging_Daemon_Ex = Reactor_Logging_Server_Adapter<Logging_Acceptor_Ex>;

ACE_FACTORY_DEFINE (SLDEX, Server_Logging_Daemon_Ex)

