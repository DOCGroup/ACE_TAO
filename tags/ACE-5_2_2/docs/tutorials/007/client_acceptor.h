// $Id$

#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

/* The ACE_Acceptor<> template lives in the ace/Acceptor.h header
   file. You'll find a very consitent naming convention between the
   ACE objects and the headers where they can be found.  In general,
   the ACE object ACE_Foobar will be found in ace/Foobar.h.  */

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* Since we want to work with sockets, we'll need a SOCK_Acceptor to
   allow the clients to connect to us.  */
#include "ace/SOCK_Acceptor.h"

/* The Client_Handler object we develop will be used to handle clients
   once they're connected.  The ACE_Acceptor<> template's first
   parameter requires such an object.  In some cases, you can get by
   with just a forward declaration on the class, in others you have to
   have the whole thing.  */
#include "client_handler.h"

/* Parameterize the ACE_Acceptor<> such that it will listen for socket
   connection attempts and create Client_Handler objects when they
   happen. In Tutorial 001, we wrote the basic acceptor logic on our
   own before we realized that ACE_Acceptor<> was available.  You'll
   get spoiled using the ACE templates because they take away a lot of
   the tedious details!  */
typedef ACE_Acceptor <Client_Handler, ACE_SOCK_ACCEPTOR> Client_Acceptor_Base;

#include "thread_pool.h"

/* This time we've added quite a bit more to our acceptor.  In
   addition to providing a choice of concurrency strategies, we also
   maintain a Thread_Pool object in case that strategy is chosen.  The
   object still isn't very complex but it's come a long way from the
   simple typedef we had in Tutorial 5.

   Why keep the thread pool as a member?  If we go back to the inetd
   concept you'll recall that we need several acceptors to make that
   work.  We may have a situation in which our different client types
   requre different resources.  That is, we may need a large thread
   pool for some client types and a smaller one for others.  We could
   share a pool but then the client types may have undesirable impact
   on one another.

   Just in case you do want to share a single thread pool, there is a
   constructor below that will let you do that.  */
class Client_Acceptor : public Client_Acceptor_Base
{
public:
  typedef Client_Acceptor_Base inherited;

  /* Now that we have more than two strategies, we need more than a
    boolean to tell us what we're using.  A set of enums is a good
    choice because it allows us to use named values.  Another option
    would be a set of static const integers.  */
  enum concurrency_t
  {
    single_threaded_,
    thread_per_connection_,
    thread_pool_
  };

  /* The default constructor allows the programmer to choose the
    concurrency strategy.  Since we want to focus on thread-pool,
    that's what we'll use if nothing is specified.  */
  Client_Acceptor (int concurrency = thread_pool_);

  /* Another option is to construct the object with an existing thread
    pool.  The concurrency strategy is pretty obvious at that point.  */
  Client_Acceptor (Thread_Pool &thread_pool);

  /* Our destructor will take care of shutting down the thread-pool if
    applicable.  */
  ~Client_Acceptor (void);

  /* Open ourselves and register with the given reactor.  The thread
    pool size can be specified here if you want to use that
    concurrency strategy.  */
  int open (const ACE_INET_Addr &addr,
            ACE_Reactor *reactor,
            int pool_size = Thread_Pool::default_pool_size_);

  /* Close ourselves and our thread pool if applicable */
  int close (void);

  /* What is our concurrency strategy?  */
  int concurrency (void)
  {
    return this->concurrency_;
  }

  /* Give back a pointer to our thread pool.  Our Client_Handler
    objects will need this so that their handle_input() methods can
    put themselves into the pool.  Another alternative would be a
    globally accessible thread pool.  ACE_Singleton<> is a way to
    achieve that.  */
  Thread_Pool *thread_pool (void)
  {
    return &this->the_thread_pool_;
  }

  /* Since we can be constructed with a Thread_Pool reference, there
    are times when we need to know if the thread pool we're using is
    ours or if we're just borrowing it from somebody else.  */
  int thread_pool_is_private (void)
  {
    return &the_thread_pool_ == &private_thread_pool_;
  }

protected:
  int concurrency_;

  Thread_Pool private_thread_pool_;

  Thread_Pool &the_thread_pool_;
};

#endif /* CLIENT_ACCEPTOR_H */
