
// $Id$

#include "Server_i.h"

/* We have to allocate space for our static finished_ flag.  We also
   initialize it to 'false' so that we don't exit immediately.
*/
sig_atomic_t Server::finished_ = 0;

/* The simple constructor and destructor don't do anything but give us
   a place to expand in the future if we want.
*/
Server::Server(void)
{
    ;
}

Server::~Server(void)
{
    ;
}

/* Opening the server is as simple as opening the acceptor with the
   default ACE_Reactor instance.  If we want to allow multiple
   instances of Server objects then we should have an ACE_Reactor
   member variable that we can register with.
*/
int Server::open(void)
{
    if (acceptor_.open (ACE_INET_Addr (ACE_DEFAULT_SERVER_PORT), ACE_Reactor::instance()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

    return(0);
}

/* Running the server just means that we execute the basic event
   loop for the reactor.  Again, if we had a private reactor then we
   could have multiple server's in their run() method.
*/
int Server::run(void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting up server daemon\n"));

        // Here's the basic event loop.  I have a 2-second timeout on
        // the handle_events() so that we don't have to wait too long
        // when we set the finished_ flag.
    while (!finished_)
    {
         // Some (all?) platforms return the "remaining time" in the
         // timeout parameter.  If we don't reset it each time, we
         // will end up with a 100% CPU spin loop!
        ACE_Time_Value timeout(2);

        ACE_Reactor::instance()->handle_events (&timeout);
    }

        // Close the acceptor when we're done.  This may be handled by
        // the framework but it's good practice to be explicit about things.
    acceptor_.close();

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) shutting down server daemon\n"));

    return 0;
}

/* The close() method simply sets the finished_ flag so that run()
   will leave the event loop and exit.
*/
int Server::close(void)
{
    finished_ = 1;
    ACE_Reactor::instance()->notify();
    return(0);
}
