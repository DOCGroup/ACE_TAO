// $Id$

#include "ace/Get_Opt.h"

#include "JAWS/Server.h"
#include "JAWS/Data_Block.h"
#include "JAWS/Concurrency.h"
#include "JAWS/IO_Handler.h"

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

}

int
JAWS_Server::open (JAWS_Pipeline_Handler *protocol)
{
  JAWS_Synch_IO_Handler_Factory synch_factory;
#if defined (ACE_WIN32)
  JAWS_Asynch_IO_Handler_Factory asynch_factory;
#else
  JAWS_Synch_IO_Handler_Factory &asynch_factory = synch_factory;
#endif /* defined (ACE_WIN32) */

  JAWS_IO_Handler_Factory *factory;
  JAWS_IO_Handler *handler;
  JAWS_Data_Block *db;

  ACE_INET_Addr inet_addr (this->port_);

  // initialize an IO_Handler
  factory = (this->dispatch_ == 0) ? &synch_factory : &asynch_factory;
  handler = factory->create_io_handler ();
  if (handler == 0)
    {
      factory->destroy_io_handler (handler, 0);
      ACE_DEBUG ((LM_DEBUG, "JAWS_Server::open, can't create handler\n"));
      return -1;
    }
  // handler->task (protocol);

  // initialize data block
  db = new JAWS_Data_Block;
  if (db == 0)
    {
      factory->destroy_io_handler (handler, 0);
      ACE_DEBUG ((LM_DEBUG, "JAWS_Server::open, can't create data block\n"));
      return -1;
    }

  // db->addr (&inet_addr);
  db->handler (handler);
  db->task (JAWS_Pipeline_Accept_Task_Singleton::instance ());

  // The message block should contain an INET_Addr, and call the
  // io->accept (INET_Addr) method!

  ACE_Message_Block mb (db);

  JAWS_Concurrency_Base *concurrency;
  concurrency = (this->concurrency_ == 0)
    ? JAWS_Thread_Pool_Singleton::instance ()
    : JAWS_Thread_Per_Singleton::instance ()
    ;

  concurrency->put (&mb);

  while (ACE_OS::thr_join (0, NULL) != -1)
    ;

  return 0;
}

void
JAWS_Server::parse_args (int argc, char *argv[])
{
  int c;

  ACE_Get_Opt getopt (argc, argv, "p:c:d:n:m:f:");
  while ((c = getopt ()) != -1)
    switch (c)
      {
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

  if (this->port_ == 0) this->port_ = 5432;
  if (this->nthreads_ == 0) this->nthreads_ = 5;
  if (this->maxthreads_ == 0) this->maxthreads_ = 20;
}
