
// $Id$

#include "Server.h"

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

sig_atomic_t Server::finished_ = 0;

Server::Server(void)
{
    ;
}

Server::~Server(void)
{
    ;
}

int Server::open(void)
{
    if (acceptor_.open (ACE_INET_Addr (PORT), ACE_Reactor::instance()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

    return(0);
}

int Server::run(void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting up server daemon\n"));

    while (!finished_)
        ACE_Reactor::instance()->handle_events ();

    acceptor_.close();

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) shutting down server daemon\n"));

    return 0;
}

int Server::close(void)
{
    finished_ = 1;
    return(0);
}
