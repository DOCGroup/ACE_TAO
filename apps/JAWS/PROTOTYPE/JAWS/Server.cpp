// $Id$

#include "ace/Get_Opt.h"

#if (ACE_NTRACE != 1)
#include "ace/Trace.h"
#endif /* (ACE_NTRACE != 1) */

#include "JAWS/Server.h"
#include "JAWS/Data_Block.h"
#include "JAWS/Concurrency.h"
#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/IO_Acceptor.h"
#include "JAWS/Pipeline_Tasks.h"

JAWS_Server::JAWS_Server (void)
  : port_ (5432),
    concurrency_ (0),
    dispatch_ (0),
    nthreads_ (5),
    maxthreads_ (20),
    flags_ (THR_NEW_LWP)
{
}

JAWS_Server::JAWS_Server (int argc, char *argv[])
  : port_ (5432),
    concurrency_ (0),
    dispatch_ (0),
    nthreads_ (5),
    maxthreads_ (20),
    flags_ (THR_NEW_LWP)
{
  this->init (argc, argv);
}

void
JAWS_Server::init (int argc, char *argv[])
{
  this->parse_args (argc, argv);

  JAWS_Thread_Pool_Singleton::instance ()->open (this->flags_,
                                                 this->nthreads_,
                                                 this->maxthreads_);

  JAWS_Thread_Per_Singleton::instance ()->open (this->flags_,
                                                this->maxthreads_);

  if (this->concurrency_ == 1)
    this->policy_.concurrency (JAWS_Thread_Per_Singleton::instance ());
  else
    this->policy_.concurrency (JAWS_Thread_Pool_Singleton::instance ());

#if !defined (ACE_WIN32)
  this->dispatch_ = 0;
#endif /* !defined (ACE_WIN32) */

  if (this->dispatch_ == 1)
    {
#if defined (ACE_WIN32)
      this->policy_.io (JAWS_Asynch_IO_Singleton::instance ());
      this->policy_.ioh_factory
        (JAWS_Asynch_IO_Handler_Factory_Singleton::instance ());
      this->policy_.acceptor (JAWS_IO_Asynch_Acceptor_Singleton::instance ());
#endif /* defined (ACE_WIN32) */
    }
  else
    {
      this->policy_.io (JAWS_Synch_IO_Singleton::instance ());
      this->policy_.ioh_factory
        (JAWS_Synch_IO_Handler_Factory_Singleton::instance ());
      this->policy_.acceptor (JAWS_IO_Synch_Acceptor_Singleton::instance ());
    }

}

int
JAWS_Server::open (JAWS_Pipeline_Handler *protocol,
                   JAWS_Dispatch_Policy *policy)
{
  if (policy == 0)
    policy = &this->policy_;

  JAWS_Data_Block *db = new JAWS_Data_Block;
  if (db == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) JAWS_Server::open, could not create Data_Block\n"));
      return -1;
    }

  ACE_INET_Addr inet_addr (this->port_);
  JAWS_IO_Synch_Acceptor_Singleton::instance ()->open (inet_addr);
  JAWS_IO_Asynch_Acceptor_Singleton::instance ()->open (inet_addr);

  // initialize data block

  db->task (JAWS_Pipeline_Accept_Task_Singleton::instance ());
  db->policy (policy);

  db->task ()->next (protocol);

  // The message block should contain an INET_Addr, and call the
  // io->accept (INET_Addr) method!

  policy->concurrency ()->put (db);

  while (ACE_OS::thr_join (0, NULL) != -1)
    ;

  db->release ();

  return 0;
}

void
JAWS_Server::parse_args (int argc, char *argv[])
{
  int c;
  int t = 1;

  ACE_Get_Opt getopt (argc, argv, "t" "p:c:d:n:m:f:");
  while ((c = getopt ()) != -1)
    switch (c)
      {
      case 't':
        t = !t;
        break;
      case 'p':
        this->port_ = ACE_OS::atoi (getopt.optarg);
        break;
      case 'c':
        if (ACE_OS::strcmp (getopt.optarg, "PER_REQUEST") == 0)
          this->concurrency_ = 1;
        else this->concurrency_ = 0;
        break;
      case 'd':
        if (ACE_OS::strcmp (getopt.optarg, "ASYNCH") == 0)
          this->dispatch_ = 1;
        else this->dispatch_ = 0;
        break;
      case 'n':
        this->nthreads_ = ACE_OS::atoi (getopt.optarg);
        break;
      case 'm':
        this->maxthreads_ = ACE_OS::atoi (getopt.optarg);
        break;
      case 'f':
        if (ACE_OS::strcmp (getopt.optarg, "THR_BOUND") == 0)
          this->flags_ |= THR_BOUND;
        else if (ACE_OS::strcmp (getopt.optarg, "THR_DAEMON") == 0)
          this->flags_ |= THR_DAEMON;
        else if (ACE_OS::strcmp (getopt.optarg, "THR_DETACHED") == 0)
          this->flags_ |= THR_DETACHED;
        break;
      }

  if (t)
    ACE_Trace::start_tracing ();
  else
    ACE_Trace::stop_tracing ();

  if (this->port_ == 0) this->port_ = 5432;
  if (this->nthreads_ == 0) this->nthreads_ = 5;
  if (this->maxthreads_ == 0) this->maxthreads_ = 20;
}
