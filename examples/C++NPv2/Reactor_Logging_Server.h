/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _REACTOR_LOGGING_SERVER_H
#define _REACTOR_LOGGING_SERVER_H

#include "ace/ACE.h"
#include "ace/Reactor.h"

template <class ACCEPTOR>
class Reactor_Logging_Server : public ACCEPTOR
{
public:
  Reactor_Logging_Server (int argc, char *argv[],
                          ACE_Reactor *reactor);
};

#endif /* _REACTOR_LOGGING_SERVER_H */
