
// $Id$

#include "client_acceptor.h"

/*
   Construct ourselves with the chosen concurrency strategy.  Notice that we also
   set our Thread_Pool reference to our private instance.
 */
Client_Acceptor::Client_Acceptor( int _concurrency )
 :  concurrency_(_concurrency)
   ,the_thread_pool_(private_thread_pool_)
{
}

/*
   Construct ourselves with a reference to somebody else' Thread_Pool.  Obvioulsy
   our concurrency strategy is "thread_pool_" at this point.
 */
Client_Acceptor::Client_Acceptor( Thread_Pool & _thread_pool )
 :  concurrency_(thread_pool_)
   ,the_thread_pool_(_thread_pool)
{
}

/*
   When we're destructed, we may need to cleanup after ourselves.  If we're running
   with a thread pool that we own, it is up to us to close it down.
 */
Client_Acceptor::~Client_Acceptor( void )
{
	if( this->concurrency() == thread_pool_ && thread_pool_is_private() )
	{
		thread_pool()->close();
	}
}

/*
   Similar to the destructor (and close() below) it is necessary for us to open the
   thread pool in some circumstances.

   Notice how we delegate most of the open() work to the open() method of our baseclass.
 */
int Client_Acceptor::open( const ACE_INET_Addr & _addr, ACE_Reactor * _reactor, int _pool_size )
{
	if( this->concurrency() == thread_pool_ && thread_pool_is_private() )
	{
		thread_pool()->open(_pool_size);
	}

	return inherited::open(_addr,_reactor);
}

/*
   Here again we find that we have to manage the thread pool.  Like open() we also delegate
   the other work to our baseclass.
 */
int Client_Acceptor::close(void)
{
	if( this->concurrency() == thread_pool_ && thread_pool_is_private() )
	{
		thread_pool()->close();
	}

	return inherited::close();
}

